#include "stdafx.h"
#include "MainGame.h"
#include "Device.h"
#include "Camera.h"
#include <vector>
#include <fstream>

ID3DXMesh*		Mesh = 0;//일반 메쉬를 받음
//ID3DXPMesh*		pMesh = 0;//프로그레시브 메쉬를 받아옴
std::vector<D3DMATERIAL9> Mtrls(0);
std::vector<IDirect3DTexture9*> Textures(0);

ID3DXMesh*		SphereMesh = 0;
ID3DXMesh*		BoxMesh = 0;

bool	RenderBoundingSphere = true;

std::ofstream  OutFile;
float dx, dy, dz;

CCamera*	Camera;

bool	ComputeBoundingSphere(ID3DXMesh* mesh, BoundingSphere* sphere);
bool	ComputeBoundingBox(ID3DXMesh* mesh, BoundingBox*  box);

void dumpVertices(std::ofstream& outFile, ID3DXMesh* mesh);
void dumpIndices(std::ofstream& outFile, ID3DXMesh* mesh);
void dumpAttributeBuffer(std::ofstream& outFile, ID3DXMesh* mesh);
void dumpAdjacencyBuffer(std::ofstream& outFile, ID3DXMesh* mesh);
void dumpAttributeTable(std::ofstream& outFile, ID3DXMesh* mesh);

CMainGame::CMainGame()
	:m_pGraphicDev(CDevice::Get_Instance())
{
}


CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::ComputeNormal(D3DXVECTOR3 * p0, D3DXVECTOR3 * p1, D3DXVECTOR3 * p2, D3DXVECTOR3 * out)
{
	D3DXVECTOR3 u = *p1 - *p0;
	D3DXVECTOR3 v = *p2 - *p0;

	D3DXVec3Cross(out, &u, &v);
	D3DXVec3Normalize(out, out);
}

HRESULT CMainGame::Initialize()
{
	if (FAILED(m_pGraphicDev->InitDevice()))
	{
		ERR_MSG(L"Client Device Create Failed");
		return E_FAIL;
	}
	D3DXVECTOR3 vPos(0.f, 0.f, 20.f);
	Camera = new CCamera(CCamera::AIRCRAFT);
	Camera->setPositino(&vPos);

	ID3DXBuffer*	adjBuffer = 0;
	ID3DXBuffer*	mtrlBuffer = 0;
	DWORD numMtrls = 0;

	if (FAILED(D3DXLoadMeshFromX(
		L"..\\Data\\bigship1.x",
		D3DXMESH_MANAGED,
		m_pGraphicDev->Get_Device(),
		&adjBuffer,
		&mtrlBuffer,
		0,
		&numMtrls,
		&Mesh
	)))
	{
		ERR_MSG(L"Failed Load Mesh");
		return E_FAIL;
	}
	
	if (0 != mtrlBuffer && 0 != numMtrls)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for (int i = 0; i < numMtrls; ++i)
		{
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			Mtrls.push_back(mtrls[i].MatD3D);

			if (0 != mtrls[i].pTextureFilename)
			{
				IDirect3DTexture9* tex = 0;
				D3DXCreateTextureFromFile(
					m_pGraphicDev->Get_Device(),
					(LPCWSTR)mtrls[i].pTextureFilename,
					&tex
				);

				Textures.push_back(tex);
			}
			else
			{
				Textures.push_back(0);
			}
		}
	}

	Mesh->OptimizeInplace(
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)adjBuffer->GetBufferPointer(),
		(DWORD*)adjBuffer->GetBufferPointer(), 0,0
		);


	BoundingSphere  boundingSphere;
	BoundingBox		boundingBox;

	ComputeBoundingSphere(Mesh, &boundingSphere);
	ComputeBoundingBox(Mesh, &boundingBox);

	D3DXCreateSphere(
		m_pGraphicDev->Get_Device(),
		boundingSphere._fRadius,
		20,
		20,
		&SphereMesh,
		0);

	D3DXCreateBox(
		m_pGraphicDev->Get_Device(),
		boundingBox._max.x - boundingBox._min.x,
		boundingBox._max.y - boundingBox._min.y,
		boundingBox._max.z - boundingBox._min.z,
		&BoxMesh,
		0);

	OutFile.open("Mesh Dump.txt");

	dumpVertices(OutFile, Mesh);
	dumpIndices(OutFile, Mesh);
	dumpAttributeTable(OutFile, Mesh);
	dumpAttributeBuffer(OutFile, Mesh);
	dumpAdjacencyBuffer(OutFile, Mesh);

	OutFile.close();
	//메쉬 정보 메모장으로 저장


	mtrlBuffer->Release();
//	adjBuffer->Release();
		
	/*if (FAILED(D3DXGeneratePMesh(
		Mesh,
		(DWORD*)adjBuffer->GetBufferPointer(),
		0,
		0,
		1,
		D3DXMESHSIMP_FACE,
		&pMesh
	)))
	{
		ERR_MSG(L"Failed Generate pMesh");
		return E_FAIL;
	}
	Mesh->Release();
	*/adjBuffer->Release();

	/*DWORD maxFaces = pMesh->GetMaxFaces();
	pMesh->SetNumFaces(maxFaces);
*/


	m_pGraphicDev->Get_Device()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->Get_Device()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->Get_Device()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	D3DLIGHT9 light;
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f) * 0.4f;
	light.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	light.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f)*0.6f;
	D3DXVECTOR3 dir{ 1.f,-1.f,1.f};
	D3DXVec3Normalize(&dir, &dir);
	light.Direction = dir;

	m_pGraphicDev->Get_Device()->SetLight(0, &light);
	m_pGraphicDev->Get_Device()->LightEnable(0, true);
	m_pGraphicDev->Get_Device()->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	m_pGraphicDev->Get_Device()->SetRenderState(D3DRS_LIGHTING, true);
	m_pGraphicDev->Get_Device()->SetRenderState(D3DRS_SPECULARENABLE, true);




	return S_OK;
}

void CMainGame::Update()
{
	//int numFaces = pMesh->GetNumFaces();

	if (GetAsyncKeyState('A') && 0x8000)
	{
		Camera->strafe(-0.04f);// 카메라 위치 이동용
		
	//	pMesh->SetNumFaces(numFaces + 1);
	//	if (pMesh->GetNumFaces() ==  numFaces)//면의 수를 늘리기 위해서는 두개의 면을 한 번에 추가해야 할 수도 있음
	//		pMesh->SetNumFaces(numFaces + 2);
	}
	else if (GetAsyncKeyState('D') && 0x8000)
	{
		Camera->strafe(0.04f);
	}
	if (GetAsyncKeyState('S') && 0x8000)
	{
		Camera->walk(-0.04f);
	//	pMesh->SetNumFaces(numFaces - 1);

	}
	else if (GetAsyncKeyState('W') && 0x8000)
	{
		Camera->walk(0.04f);
		
	}
	if (GetAsyncKeyState('R') && 0x8000)
	{
		Camera->fly(0.04f);
	}
	else if (GetAsyncKeyState('F') && 0x8000)
	{
		Camera->fly(-0.04f);
	}

	if (GetAsyncKeyState(VK_UP) && 0x8000)
	{
		Camera->pitch(-0.001f);
	}
	else if (GetAsyncKeyState(VK_DOWN) && 0x8000)
	{
		Camera->pitch(0.001f);
	}
	if (GetAsyncKeyState(VK_RIGHT) && 0x8000)
	{
		Camera->yaw(0.001f);
	}
	else if (GetAsyncKeyState(VK_LEFT) && 0x8000)
	{
		Camera->yaw(-0.001f);
	}

	if (GetAsyncKeyState('N') && 0x8000)
	{
		Camera->roll(0.001f);
	}
	else if (GetAsyncKeyState('M') && 0x8000)
	{
		Camera->roll(-0.001f);
	}


	D3DXMATRIX View,proj;
	Camera->getViewMatrix(&View);


//	D3DXMatrixLookAtLH(&View, &position, &target, &up);
	m_pGraphicDev->Get_Device()->SetTransform(D3DTS_VIEW, &View);

	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI*0.5,
		800.f / 600.f,
		1.0f,
		1000.0f
	);

	m_pGraphicDev->Get_Device()->SetTransform(D3DTS_PROJECTION, &proj);

}

void CMainGame::Late_Update()
{

}

void CMainGame::Render()
{
		
	m_pGraphicDev->Render_Begin();
	//그리기 시작
	for (int i = 0; i < Mtrls.size(); ++i)
	{
		m_pGraphicDev->Get_Device()->SetMaterial(&Mtrls[i]);
		m_pGraphicDev->Get_Device() ->SetTexture(0, Textures[i]);
		Mesh->DrawSubset(i);

	}

	D3DMATERIAL9 mtrl;
	mtrl.Ambient = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);
	mtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 0.f, 0.10f);
	mtrl.Specular = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);
	mtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);

	m_pGraphicDev->Get_Device()->SetMaterial(&mtrl);
	m_pGraphicDev->Get_Device()->SetTexture(0, 0);

	
	m_pGraphicDev->Get_Device()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pGraphicDev->Get_Device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->Get_Device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	BoxMesh->DrawSubset(0);
	
	m_pGraphicDev->Get_Device()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	m_pGraphicDev->Render_End();
}

void CMainGame::Release()
{
	Mesh->Release();

	SphereMesh->Release();
	BoxMesh->Release();

	Safe_Delete(Camera);

	m_pGraphicDev->Destroy_Instance();

}

bool	ComputeBoundingSphere(ID3DXMesh* mesh, BoundingSphere* sphere)
{
	BYTE* v = 0;
	mesh->LockVertexBuffer(0, (void**)&v);

	if (FAILED(D3DXComputeBoundingSphere(
		(D3DXVECTOR3*)v,
		mesh->GetNumVertices(),
		D3DXGetFVFVertexSize(mesh->GetFVF()),
		&sphere->_vCenter,
		&sphere->_fRadius
	)))
	{
		ERR_MSG(L"Failed ComputeBoundingSphere");
		return false;
	}

	 mesh->UnlockVertexBuffer();

	return true;
}

bool	ComputeBoundingBox(ID3DXMesh* mesh, BoundingBox*  box)
{
	BYTE* v = 0;
	mesh->LockVertexBuffer(0, (void**)&v);

	if (FAILED(D3DXComputeBoundingBox(
		(D3DXVECTOR3*)v,
		mesh->GetNumVertices(),
		D3DXGetFVFVertexSize(mesh->GetFVF()),
		&box->_min,
		&box->_max
	)))
	{
		ERR_MSG(L"Failed ComputeBoundingBox");
		return false;
	}

	mesh->UnlockVertexBuffer();

	return true;

}


void dumpVertices(std::ofstream& outFile, ID3DXMesh* mesh)
{
	outFile << "Vertices:" << std::endl;
	outFile << "---------" << std::endl << std::endl;

	Vertex* v = 0;
	mesh->LockVertexBuffer(0, (void**)&v);
	for (int i = 0; i < mesh->GetNumVertices(); i++)
	{
		outFile << "Vertex " << i << ": (";
		outFile << v[i].vPos.x << ", " << v[i].vPos.y << ", " << v[i].vPos.z << ", ";
		outFile << v[i].vNormal.x << ", " << v[i].vNormal.y << ", " << v[i].vNormal.z << ", ";
		outFile << v[i]._u << ", " << v[i]._v << ")" << std::endl;
	}
	mesh->UnlockVertexBuffer();

	outFile << std::endl << std::endl;
}

void dumpIndices(std::ofstream& outFile, ID3DXMesh* mesh)
{
	outFile << "Indices:" << std::endl;
	outFile << "--------" << std::endl << std::endl;

	WORD* indices = 0;
	mesh->LockIndexBuffer(0, (void**)&indices);

	for (int i = 0; i < mesh->GetNumFaces(); i++)
	{
		outFile << "Triangle " << i << ": ";
		outFile << indices[i * 3] << " ";
		outFile << indices[i * 3 + 1] << " ";
		outFile << indices[i * 3 + 2] << std::endl;
	}
	mesh->UnlockIndexBuffer();

	outFile << std::endl << std::endl;
}

void dumpAttributeBuffer(std::ofstream& outFile, ID3DXMesh* mesh)
{
	outFile << "Attribute Buffer:" << std::endl;
	outFile << "-----------------" << std::endl << std::endl;

	DWORD* attributeBuffer = 0;
	mesh->LockAttributeBuffer(0, &attributeBuffer);

	// an attribute for each face
	for (int i = 0; i < mesh->GetNumFaces(); i++)
	{
		outFile << "Triangle lives in subset " << i << ": ";
		outFile << attributeBuffer[i] << std::endl;
	}
	mesh->UnlockAttributeBuffer();

	outFile << std::endl << std::endl;
}

void dumpAdjacencyBuffer(std::ofstream& outFile, ID3DXMesh* mesh)
{
	outFile << "Adjacency Buffer:" << std::endl;
	outFile << "-----------------" << std::endl << std::endl;

	// three enttries per face
	std::vector<DWORD> adjacencyBuffer(mesh->GetNumFaces() * 3);

	mesh->GenerateAdjacency(0.0f, &adjacencyBuffer[0]);

	for (int i = 0; i < mesh->GetNumFaces(); i++)
	{
		outFile << "Triangle's adjacent to triangle " << i << ": ";
		outFile << adjacencyBuffer[i * 3] << " ";
		outFile << adjacencyBuffer[i * 3 + 1] << " ";
		outFile << adjacencyBuffer[i * 3 + 2] << std::endl;
	}

	outFile << std::endl << std::endl;
}

void dumpAttributeTable(std::ofstream& outFile, ID3DXMesh* mesh)
{
	outFile << "Attribute Table:" << std::endl;
	outFile << "----------------" << std::endl << std::endl;

	// number of entries in the attribute table
	DWORD numEntries = 0;

	mesh->GetAttributeTable(0, &numEntries);

	std::vector<D3DXATTRIBUTERANGE> table(numEntries);

	mesh->GetAttributeTable(&table[0], &numEntries);

	for (int i = 0; i < numEntries; i++)
	{
		outFile << "Entry " << i << std::endl;
		outFile << "-----------" << std::endl;

		outFile << "Subset ID:    " << table[i].AttribId << std::endl;
		outFile << "Face Start:   " << table[i].FaceStart << std::endl;
		outFile << "Face Count:   " << table[i].FaceCount << std::endl;
		outFile << "Vertex Start: " << table[i].VertexStart << std::endl;
		outFile << "Vertex Count: " << table[i].VertexCount << std::endl;
		outFile << std::endl;
	}

	outFile << std::endl << std::endl;
}
