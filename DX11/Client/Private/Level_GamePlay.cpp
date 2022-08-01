#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"
#include "GameInstance.h"



CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
	//	return E_FAIL;

	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		
}

HRESULT CLevel_GamePlay::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("게임프렐이레벨임. "));

	return S_OK;
}


HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.Camera_Free */
	//CCamera::CAMERADESC			CameraDesc;
	//ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	//CameraDesc.iLevelIndex = LEVEL_STATIC;
	//CameraDesc.pTransformPrototypeTag = TEXT("Prototype_Component_Transform");
	//CameraDesc.vEye = _float3(5.0f, 5.f, -3.f);
	//CameraDesc.vAt = _float3(5.f, 0.f, 20.f);
	//CameraDesc.TransformDesc.fSpeedPerSec = 5.f;
	//CameraDesc.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	//CameraDesc.fFovy = D3DXToRadian(65.0f);
	//CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	//CameraDesc.fNear = 0.2f;
	//CameraDesc.fFar = 300.f;

	//if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_Camera_Free"), &CameraDesc)))
	//	return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}


HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.Terrain */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;

	/* For.Sky */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar * pLayerTag)
{	
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.Player */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Player"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	for (_uint i = 0; i < 20; ++i)
	{
		/* For.Monster */
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Monster"))))
			return E_FAIL;
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	for (_uint i = 0; i < 20; ++i)
	{
		/* For.Effect */
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Effect"))))
			return E_FAIL;
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_GamePlay*		pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();

}

