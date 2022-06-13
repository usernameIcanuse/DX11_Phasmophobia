#include "stdafx.h"
#include "MainApp.h"
#include "Device.h"
#include "Player.h"
#include "Camera.h"
#include "Monster.h"
#include "Terrain.h"
#include "ZFrustum.h"

CMainApp::CMainApp()
	:m_pGraphicDev(CDevice::Get_Instance()),m_pPlayer(nullptr)
{
}


CMainApp::~CMainApp()
{
	Release();
}

HRESULT CMainApp::Initialize()
{
	if (FAILED(m_pGraphicDev->InitDevice()))
	{
		return E_FAIL;
	}
	m_pDevice = m_pGraphicDev->Get_Device();

	if (!m_pPlayer)
	{
		m_pPlayer = new CPlayer;
		m_pPlayer->Initialize();
	}

	if (!m_pTerrain)
	{
		m_pTerrain = new CTerrain(64, 64, 10, 0.4f);
		m_pTerrain->genTexture(&D3DXVECTOR3(1.f,1.f,1.f));
	}

	/*if (!m_pMonster)
	{
		m_pMonster = new CMonster(m_pPlayer);
		m_pMonster->Initialize();
	}*/

	D3DXCreateSphere(m_pDevice, 0.1f, 10.f, 10.f, &m_pSphere, 0);
	D3DXMatrixIdentity(&m_matSphereWorld);
	m_matSphereWorld._42 = -2.f;

	CCamera::Get_Instance()->Initialize();
	m_pDevice->CreateVertexBuffer(
		6 * sizeof(Vertex_Tex),
		D3DUSAGE_WRITEONLY,
		VTXTEX_FVF,
		D3DPOOL_MANAGED,
		&m_pVB, 0);

	Vertex_Tex*  vx = 0;
	m_pVB->Lock(0, 0, (void**)&vx, 0);

	vx[0] = Vertex_Tex(D3DXVECTOR3(-25.f, -2.f ,-25.f), /*D3DXVECTOR3(0.0f, 1.f, 0.f), */0.f, 1.f);

	vx[1] = Vertex_Tex(D3DXVECTOR3(-25.f, -2.f, 25.f ), /*D3DXVECTOR3(0.0f, 1.f, 0.f),*/ 0.f, 0.f);

	vx[2] = Vertex_Tex(D3DXVECTOR3(25.f, -2.f,25.f ), /*D3DXVECTOR3(0.0f, 1.f, 0.f), */1.f, 0.f);

	vx[3] = Vertex_Tex(D3DXVECTOR3(-25.f, -2.f,-25.f ), /*D3DXVECTOR3(0.0f, 1.f, 0.f),*/ 0.f, 1.f);

	vx[4] = Vertex_Tex(D3DXVECTOR3(25.f, -2.f,25.f ), /*D3DXVECTOR3(0.0f, 1.f, 0.f), */1.f, 0.f);

	vx[5] = Vertex_Tex(D3DXVECTOR3(25.f, -2.f, -25.f), /*D3DXVECTOR3(0.0f, 1.f, 0.f),*/ 1.f, 1.f);

	m_pVB->Unlock();


	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj,
		D3DX_PI*0.5f,
		1280.f / 720.f,
		1.f,
		1000.f);

	m_pDevice->SetTransform(D3DTS_PROJECTION, &proj);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, false);


	return S_OK;
}

void CMainApp::Tick()
{
	m_pPlayer->Tick();
	//m_pMonster->Tick();

	if (GetAsyncKeyState(VK_SPACE))
	{
		m_bChange = true;
	}

	if (GetAsyncKeyState(VK_SHIFT))
	{
		m_bChange = false;
	}

	if (m_bChange)
	{
		if (35 > m_iTime)
		{
			m_matSphereWorld._11 += 10;
			m_matSphereWorld._22 += 10;
			m_matSphereWorld._33 += 10;
			++m_iTime;
		}
	}
	else
	{
		if (0 < m_iTime )
		{
			m_matSphereWorld._11 -= 10;
			m_matSphereWorld._22 -= 10;
			m_matSphereWorld._33 -= 10;
			--m_iTime;
		}
	}

}

void CMainApp::LateTick()
{
	CCamera::Get_Instance()->TransView();
	CZFrustum::Get_Instance()->LateTick();
	m_pPlayer->LateTick();
	D3DXMATRIX Temp= CCamera::Get_Instance()->GetCameraMatrix();
	D3DXMatrixIdentity(&matBillboard);
	matBillboard._11 == Temp._11;
	matBillboard._13 == Temp._13;
	matBillboard._31 == Temp._31;
	matBillboard._33 == Temp._33;
	D3DXMatrixInverse(&matBillboard, 0, &matBillboard);
	matBillboard._41 = matBillboard._42 = matBillboard._43 = 0.f;
	m_pTerrain->ProcessFrustumCull();

	//m_pMonster->LateTick();
}


void CMainApp::Render()
{
	m_pGraphicDev->Render_Begin();

	m_pTerrain->draw();

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);



		//m_pDevice->SetTexture(0, m_pTexture);
		//m_pDevice->SetTexture(1, m_pTexture2);
		////텍스처 인덱스 지정( 뭐 텍스처 인덱스 지정한거 없어서 둘다 0으로)
		//m_pDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
		//m_pDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
		////스테이지 0단계 지정
		//m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);//m_pTexture
		//m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		////스테이지 1단계 지정
		//m_pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		//m_pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);//m_pTexture2
		////m_pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);//0단계의 결과 텍스처를 사용


		////스테이지 2단계 지정(멈춤)
		//m_pDevice->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);//0단계의 결과 텍스처를 사용
		//m_pDevice->SetTextureStageState(2, D3DTSS_ALPHAOP, D3DTOP_DISABLE);//0단계의 결과 텍스처를 사용
	

	m_pPlayer->Render();
	//m_pMonster->Render();

	m_pGraphicDev->Render_End();
}

void CMainApp::Release()
{

	delete m_pPlayer;
	m_pPlayer = nullptr;

	delete m_pTerrain;
	m_pTerrain = nullptr;

	/*delete m_pMonster;
	m_pMonster= nullptr;*/

	m_pSphere->Release();

	m_pTexture->Release();
	m_pTexture2->Release();

	CCamera::Destroy_Instance();
	CZFrustum::Destroy_Instance();
	m_pGraphicDev->Destroy_Instance();
}
