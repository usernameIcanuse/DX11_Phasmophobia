#include "stdafx.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"
#include "BackGround.h"
#include "Lobby_Main.h"
#include "UIIcon.h"
#include "Lobby_WaitingRoom.h"
#include "Lobby_AddItems.h"
#include "Lobby_Store.h"

#include "Camera_Free.h"
#include "Camera_FPS.h"
//#include "Monster.h"
#include "Model.h"
#include "Terrain.h"
#include "Player.h"
//#include "Effect.h"
#include "Sky.h"
#include "DotsProjecter.h"

CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

unsigned int APIENTRY LoadingMain(void* pArg)
{
	CoInitializeEx(nullptr, 0);

	CLoader*		pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CS());

	HRESULT hr = 0;

	switch (pLoader->Get_NextLevelID())
	{
	case LEVEL_LOGO:
		hr = pLoader->Loading_ForLogoLevel();
		break;
	case LEVEL_LOBBY:
		hr = pLoader->Loading_ForLobbyLevel();
		break;
	case LEVEL_STAGE1:
		hr = pLoader->Loading_ForStage1Level();
		break;
	}	

	if (FAILED(hr))
		MSG_BOX("Failed to Loading");

	LeaveCriticalSection(&pLoader->Get_CS());

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevel)
{
	m_eNextLevel = eNextLevel;


	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_ForLogoLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	
#pragma region PROTOTYPE_GAMEOBJECT

	lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	///* For.Prototype_GameObject_Camera_Free*/
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
	//	CCamera_Free::Create(m_pGraphic_Device))))
	//	return E_FAIL;

#pragma endregion


	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중이비낟. "));
	/* For.Prototype_Component_Texture_Default */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Phasmophobia_Logo.dds")))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중이비낟. "));



	lstrcpy(m_szLoadingText, TEXT("로딩 끝 "));	

	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForLobbyLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

#pragma region PROTOTYPE_GAMEOBJECT

	lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));
	m_fLoadingRatio = 0.f;
	_float		fRatio = 1.f / 10.f;//로딩 퍼센테이지

	/*For, Prototype_GameObject_Lobby*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Lobby"),
		CLobby_Main::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	m_fLoadingRatio += fRatio;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_WaitingRoom"),
		CLobby_WaitingRoom::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	m_fLoadingRatio += fRatio;
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Store"),
		CLobby_Store::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	m_fLoadingRatio += fRatio;
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_AddItems"),
		CLobby_AddItems::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	m_fLoadingRatio += fRatio;
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_LobbyIcon"),
		CUIIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	m_fLoadingRatio += fRatio;

#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중이비낟. "));
	/*For.Prototype_Component_Texture_Lobby*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Lobby/Lobby_Main.dds")))))
		return E_FAIL;
	m_fLoadingRatio += fRatio;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_OutLine"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Lobby/OutLine.dds")))))
		return E_FAIL;
	m_fLoadingRatio += fRatio;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_WaitingRoom"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/WaitingRoom/WaitingRoom.dds")))))
		return E_FAIL;
	m_fLoadingRatio += fRatio;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Large_outline"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/WaitingRoom/Large_outline.dds")))))
		return E_FAIL;
	m_fLoadingRatio += fRatio;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Small_outline"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/WaitingRoom/Small_outline.dds")))))
		return E_FAIL;
	m_fLoadingRatio += fRatio;


	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중이비낟. "));

	///* For.Prototype_Component_VIBuffer_Terrain */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
	//	CVIBuffer_Terrain::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
	//	return E_FAIL;

	///* For.Prototype_Component_VIBuffer_Cube*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
	//	CVIBuffer_Cube::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩 끝 "));

	m_isFinished = true;

	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CLoader::Loading_ForStage1Level()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

#pragma region PROTOTYPE_GAMEOBJECT

	lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));

	/* For.Prototype_GameObject_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_FPS*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_FPS"),
		CCamera_FPS::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	///* For.Prototype_GameObject_Player */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_DotsProjecter*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_DotsProjecter"),
		CDotsProjecter::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	///* For.Prototype_GameObject_Monster */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster"),
	//	CMonster::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_Effect */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect"),
	//	CEffect::Create(m_pGraphic_Device))))
	//	return E_FAIL;


#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중이비낟. "));
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Grass_%d.dds"), 2))))
		return E_FAIL;

	///* For.Prototype_Component_Texture_Player */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Player/Player.jpg"), 1))))
	//	return E_FAIL;

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
		return E_FAIL;

	///* For.Prototype_Component_Texture_Monster */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Monster"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Snow/Snow.png"), 1))))
	//	return E_FAIL;

	///* For.Prototype_Component_Texture_Explosion */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Explosion"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Explosion/Explosion%d.png"), 90))))
	//	return E_FAIL;



	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중이비낟. "));

	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cube*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	_matrix			TransformMatrix;

	/* For.Prototype_Component_Model_Lighter*/
	TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Lighter"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Lighter/", "Lighter.fbx", TransformMatrix))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("셰이더를 로딩중이빈다. "));
	/* For.Prototype_Component_Shader_VtxNorTex */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Element, VTXNORTEX_DECLARATION::iNumElements))))
		return E_FAIL;


	/* For.Prototype_Component_Shader_VtxCubeTex */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Shader_VtxCubeTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCubeTex.hlsl"), VTXCUBETEX_DECLARATION::Element, VTXCUBETEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxModel */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Shader_VtxModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxModel.hlsl"), VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩 끝 "));

	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevel)
{
	CLoader*		pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	DeleteCriticalSection(&m_CriticalSection);
	CloseHandle(m_hThread);
}
