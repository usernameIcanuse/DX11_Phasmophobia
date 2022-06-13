#include "stdafx.h"
#include "..\public\Terrain.h"
#include "Device.h"
#include "ZFrustum.h"


const DWORD CTerrain::TerrainVertex::FVF = D3DFVF_XYZ | D3DFVF_TEX1;

CTerrain::CTerrain()
{
}

CTerrain::CTerrain( int numVertsPerRow, int numVertsPerCol, int cellSpacing, float heightScale)
	:m_pDevice(CDevice::Get_Instance()->Get_Device())
{
	m_iNumVertsPerRow = numVertsPerRow;
	m_iNumVertsPerCol = numVertsPerCol;
	m_iCellSpacing = cellSpacing;

	m_iNumCellsPerRow = numVertsPerRow - 1;
	m_iNumCellsPerCol = numVertsPerCol - 1;

	m_iWidth = m_iNumCellsPerRow * m_iCellSpacing;
	m_iDepth = m_iNumCellsPerCol* m_iCellSpacing;

	m_iNumVertices = m_iNumVertsPerCol * m_iNumVertsPerRow;
	m_iNumTriangles = m_iNumCellsPerCol * m_iNumCellsPerRow * 2;

	m_fHeightScale = heightScale;

	if (!readRawFile())
	{
		MessageBox(0, L"ReadRawFile - FAILED", 0, 0);
		PostQuitMessage(0);
	}

	for (int i = 0; i < m_vheightmap.size(); ++i)
		m_vheightmap[i] *= m_fHeightScale;

	if (!computeVertices())
	{
		MessageBox(0, L"ComputeVertices - FAILED", 0, 0);
		PostQuitMessage(0);
	}

	if (!computeIndices())
	{
		MessageBox(0, L"ComputeVertices - FAILED", 0, 0);
		PostQuitMessage(0);
	}

	
}

CTerrain::~CTerrain()
{
	Release();
}


int CTerrain::getHeightmapEntry(int row, int col)
{
	return m_vheightmap[row*m_iNumVertsPerRow + col];
}

void CTerrain::setHeightmapEntry(int row, int col, int value)
{
	m_vheightmap[row*m_iNumVertsPerRow + col] = value;
}

float CTerrain::getHeight(float x, float y)
{
	return 0.0f;
}


bool CTerrain::genTexture(D3DXVECTOR3 * directionToLight)//if you don't have texture files, then create Texture
{
	int texWidth = m_iNumCellsPerRow;
	int texHeight= m_iNumCellsPerCol;

	if (FAILED(D3DXCreateTexture(
		m_pDevice,
		texWidth, texHeight,
		0,
		0,
		D3DFMT_X8R8G8B8,
		D3DPOOL_MANAGED, &m_pTexture
	)))
	{
		MessageBox(0, L"CreateTexture - FAILED", 0, 0);
		return false;
	}

	D3DSURFACE_DESC textureDesc;
	m_pTexture->GetLevelDesc(0, &textureDesc);

	if (textureDesc.Format != D3DFMT_X8R8G8B8)
	{
		MessageBox(0, L"TextureFormat unMatched", 0, 0);
		return false;
	}

	D3DLOCKED_RECT lockedRect;
	m_pTexture->LockRect(0, &lockedRect, 0, 0);

	DWORD* ImageData = (DWORD*)lockedRect.pBits;

	for (int i = 0; i < texHeight; ++i)
	{
		for (int j = 0; j < texWidth; ++j)
		{
			ImageData[i*lockedRect.Pitch / 4 + j] = (D3DCOLOR)D3DXCOLOR(0.f, 0.5f, 0.f, 0.f);
		}
	}

	m_pTexture->UnlockRect(0);

	/*if (!lightTerrain(directionToLight))
	{
		MessageBox(0, L"LightTerrain-Failed", 0, 0);
		return false;
	}ºû ¿¬»ê*/

	if (FAILED(D3DXFilterTexture(
		m_pTexture,
		0, 0,
		D3DX_DEFAULT)))
	{
		MessageBox(0, L"D3DXFilterTExture()-Failed", 0, 0);
	}

	return true;
}

void CTerrain::draw(bool drawTris)
{
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	world._42 = -25.f;
	m_pDevice->SetTransform(D3DTS_WORLD, &world);

	m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(TerrainVertex));
	m_pDevice->SetFVF(TerrainVertex::FVF);
	m_pDevice->SetIndices(m_pIB);

	m_pDevice->SetTexture(0, m_pTexture);

	//m_pDevice->SetRenderState(D3DRS_LIGHTING, false);
	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, 0, m_iNumTriangles);

	if (drawTris)
	{
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, 0, m_iNumTriangles);

		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}

void CTerrain::Release()
{
	m_pTexture->Release();
	m_pVB->Release();
	m_pIB->Release();
}

bool CTerrain::readRawFile()
{
	std::vector<BYTE> in(m_iNumVertices);

	std::ifstream inFile("../../Data/coastMountain64.raw",std::ifstream::binary);

	if (inFile)
	{

		inFile.read((char*)&in[0], in.size());

		inFile.close();

		m_vheightmap.resize(m_iNumVertices);

		for (int i = 0; i < in.size(); ++i)
			m_vheightmap[i] = in[i];
		return true;
	}
	return false;
}

bool CTerrain::computeVertices()
{
	if (FAILED(m_pDevice->CreateVertexBuffer(
		m_iNumVertices * sizeof(TerrainVertex),
		D3DUSAGE_WRITEONLY,
		TerrainVertex::FVF,
		D3DPOOL_MANAGED,
		&m_pVB, 0
	)))
	{	
		MessageBox(0, L"CreateVertexBuffer-FAILED", 0, 0);
		return false;
	}

	int iStartX = -m_iWidth / 2;
	int iStartZ = m_iDepth / 2;

	int iEndX = m_iWidth / 2;
	int iEndZ = -m_iDepth / 2;

	float uCoordIncrementSize = 1.f / (float)m_iNumCellsPerRow;
	float vCoordIncrementSize = 1.f / (float)m_iNumCellsPerCol;

	TerrainVertex* Vertices = nullptr;
	m_pVB->Lock(0, 0, (void**)&Vertices, 0);

	int i = 0;
	for (int z = iStartZ; z >= iEndZ; z -= m_iCellSpacing)
	{
		int j = 0;
		for (int x = iStartX; x <= iEndX; x += m_iCellSpacing)
		{
			int index = i*m_iNumVertsPerRow + j;

			Vertices[index] = TerrainVertex(
				(float)x,
				(float)m_vheightmap[index],
				(float)z,
				(float)j*uCoordIncrementSize,
				(float)i*vCoordIncrementSize);

			++j;
		}
		++i;
	}
	m_pVB->Unlock();

	return true;

}

bool CTerrain::computeIndices()
{
	if (FAILED(m_pDevice->CreateIndexBuffer(
		m_iNumTriangles * 3 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB, 0
	)))
	{
		MessageBox(0, L"CreateIndexBuffer-FAILED", 0, 0);
		return false;
	}

	WORD* Indices = nullptr;
	m_pIB->Lock(0, 0, (void**)&Indices, 0);

	int baseIndex = 0;

	for (int i = 0; i < m_iNumCellsPerCol; ++i)
	{
		for (int j = 0; j < m_iNumCellsPerRow; ++j)
		{


			Indices[baseIndex] = i*m_iNumVertsPerRow + j;
			Indices[baseIndex + 1] = i*m_iNumVertsPerRow + j + 1;
			Indices[baseIndex + 2] = (i+1)*m_iNumVertsPerRow + j;
			Indices[baseIndex+3] = (i+1)*m_iNumVertsPerRow + j;
			Indices[baseIndex + 4] = i*m_iNumVertsPerRow + j + 1;
			Indices[baseIndex + 5] = (i + 1)*m_iNumVertsPerRow + j+1;

			baseIndex += 6;
		}
	}

	m_pIB->Unlock();

	return true;
}

HRESULT CTerrain::ProcessFrustumCull()
{
	m_iNumTriangles = 0;
	WORD	Height[4];
	bool	b[4];
	WORD* Indices = nullptr;
	int baseIndex = 0;

	if (FAILED(m_pIB->Lock(0, 0, (void**)&Indices, 0)))
	{
		MessageBox(0, L"ProcessFrustumCull - FAILED", 0, 0);
		return E_FAIL;
	}

	for (int i = 0; i < m_iNumCellsPerCol; ++i)
	{
		for (int j = 0; j < m_iNumCellsPerRow; ++j)
		{
			Height[0] = (i*m_iNumVertsPerRow + j);
			Height[1] = (i*m_iNumVertsPerRow + j+1);
			Height[2] = ((i+1)*m_iNumVertsPerRow + j);
			Height[3] = ((i+1)*m_iNumVertsPerRow + j+1);

			b[0] = CZFrustum::Get_Instance()->IsIn(&D3DXVECTOR3(j,m_vheightmap[Height[0]],i));
			b[1] = CZFrustum::Get_Instance()->IsIn(&D3DXVECTOR3(j+1, m_vheightmap[Height[1]], i));
			b[2] = CZFrustum::Get_Instance()->IsIn(&D3DXVECTOR3(j, m_vheightmap[Height[2]], i+1));

			if (b[0] | b[1] | b[2])
			{
				Indices[baseIndex] = Height[0];
				Indices[baseIndex+1] = Height[1];
				Indices[baseIndex+2] = Height[2];
				++m_iNumTriangles;
				baseIndex += 3;

			}

			b[2] = CZFrustum::Get_Instance()->IsIn(&D3DXVECTOR3(j, m_vheightmap[Height[0]], i+1));
			b[1] = CZFrustum::Get_Instance()->IsIn(&D3DXVECTOR3(j + 1, m_vheightmap[Height[1]], i));
			b[3] = CZFrustum::Get_Instance()->IsIn(&D3DXVECTOR3(j+1, m_vheightmap[Height[2]], i + 1));

			if (b[2] | b[1] | b[3])
			{
				Indices[baseIndex] = Height[2];
				Indices[baseIndex + 1] = Height[1];
				Indices[baseIndex + 2] = Height[3];
				++m_iNumTriangles;
				baseIndex += 3;

			}

		}
	}

	m_pIB->Unlock();

	return S_OK;
}

bool CTerrain::lightTerrain(D3DXVECTOR3 * directionToLight)
{
	return false;
}

float CTerrain::computeShade(int cellRow, int cellCol, D3DXVECTOR3 * directionToLight)
{
	return 0.0f;
}
