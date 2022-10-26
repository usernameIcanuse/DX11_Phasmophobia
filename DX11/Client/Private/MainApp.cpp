#include "stdafx.h"
#include "..\Public\MainApp.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Loading.h"
#include "FirstLoading.h"
#include "Engine_Defines.h"


CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::Get_Instance())
	, m_pImguiMgr(CImguiMgr::Get_Instance())
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pImguiMgr);
}

HRESULT CMainApp::Initialize()
{
	//D3D11_SAMPLER_DESC
	
	GRAPHICDESC		GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(GRAPHICDESC));

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iWinCX = g_iWinCX;
	GraphicDesc.iWinCY = g_iWinCY;
	GraphicDesc.isWindowMode = GRAPHICDESC::MODE_WIN;

	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, LEVEL_END, GraphicDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;	

	m_pImguiMgr->Init(m_pDevice, m_pContext);
	
	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;
	
	if (FAILED(Ready_Loading_Background()))
		return E_FAIL;

	
	if (FAILED(Open_Level(LEVEL_LOBBY)))
		return E_FAIL;

#ifdef _DEBUG
	if (FAILED(GAMEINSTANCE->Add_Font(m_pDevice, m_pContext, TEXT("Font_Dream"), TEXT("../Bin/Resources/Fonts/128.spriteFont"))))
		return E_FAIL;
#endif
	

	return S_OK;
}

void CMainApp::Tick(float fTimeDelta)
{
	if (nullptr == m_pGameInstance)
		return;
#ifdef _DEBUG
	m_fTimeAcc += fTimeDelta;
#endif
	m_pGameInstance->Tick_Engine(fTimeDelta);
	m_fTimeDelta = fTimeDelta;
	m_pImguiMgr->Tick(fTimeDelta);
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 1.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();
	
	m_pGameInstance->Render_Engine(m_fTimeDelta);
#ifdef _DEBUG

	++m_iNumRender;


	if (m_fTimeAcc >= 1.f)
	{
		wsprintf(m_szFPS, TEXT("에프피에스 : %d"), m_iNumRender);
		m_fTimeAcc = 0.f;
		m_iNumRender = 0;
	}
	// MakeSpriteFont "폰트이름" /FontSize:32 /FastPack /CharacterRegion:0x0020-0x00FF /CharacterRegion:0x3131-0x3163 /CharacterRegion:0xAC00-0xD800 /DefaultCharacter:0xAC00 출력파일이름.spritefont
	m_pGameInstance->Render_Font(TEXT("Font_Dream"), m_szFPS, _float2(600.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
#endif

	m_pImguiMgr->Render();

	m_pGameInstance->Present();



	return S_OK;
}


HRESULT CMainApp::Open_Level(LEVEL eLevelID)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	CLevel_Loading*		pLevel_Loading = CLevel_Loading::Create(m_pDevice, m_pContext, eLevelID,true);
	if (nullptr == pLevel_Loading)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_ReserveLevel(LEVEL_LOADING, pLevel_Loading, eLevelID)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* For.Prototype_Component_Renderer */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		CRenderer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		CTransform::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxTex.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements))))
		return E_FAIL;


	return S_OK;
}

HRESULT CMainApp::Ready_Loading_Background()
{
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Loading"),
		CLoading::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_FirstLoading"),
		CFirstLoading::Create(m_pDevice, m_pContext))))
		return E_FAIL;


#pragma endregion
	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*		pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MessageBox(g_hWnd, TEXT("Failed to Created : CMainApp"), TEXT("System Error"), MB_OK);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);		
	Safe_Release(m_pImguiMgr);

	CImguiMgr::Destroy_Instance();
	CGameInstance::Release_Engine();	
}
