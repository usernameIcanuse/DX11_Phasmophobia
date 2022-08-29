#include "stdafx.h"
#include "..\Public\Level_Stage1.h"
#include "GameInstance.h"
#include "Camera_Free.h"



CLevel_Stage1::CLevel_Stage1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Stage1::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;
	/*메쉬 등 추가*/

	if (FAILED(Ready_Layer_SkyBox(TEXT("Layer_SkyBox"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if(FAILED(Ready_Lights()))
		return E_FAIL;

	/*if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;*/

	/*if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
		return E_FAIL;*/


	return S_OK;
}

void CLevel_Stage1::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		

	/*CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_LOBBY))))
		return;

	Safe_Release(pGameInstance);*/
}

HRESULT CLevel_Stage1::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("Level_Stage1. "));

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_SkyBox(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.Lobby*/
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1, pLayerTag, TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;

	/*if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Item"), TEXT("Prototype_GameObject_DotsProjecter"))))
		return E_FAIL;*/

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_Camera(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float4(0.0f, 20.f, -100.f, 1.f);
	CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.TransformDesc.fSpeedPerSec = 5.f;
	CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	CameraDesc.fFovy = XMConvertToRadians(65.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1, pLayerTag, TEXT("Prototype_GameObject_Camera_Free"), nullptr,&CameraDesc)))
		return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_Player(const _tchar* pLayertag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if(FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1, pLayertag, TEXT("Prototype_GameObject_Player"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Lights()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	/*ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = tagLightDesc::TYPE_POINT;
	LightDesc.vPosition = _float4(10.f, 5.f, 10.f, 0.f);
	LightDesc.fRange = 10.f;
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;*/

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_Terrain(const _tchar* pLayertag)
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1, pLayertag, TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;

	

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



CLevel_Stage1 * CLevel_Stage1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Stage1*		pInstance = new CLevel_Stage1(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Stage1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Stage1::Free()
{
	__super::Free();



}

