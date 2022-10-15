#include "stdafx.h"
#include "..\Public\Level_Lobby.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Level_Loading.h"
#include "Lobby_Main.h"
#include "Lobby_WaitingRoom.h"



CLevel_Lobby::CLevel_Lobby(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Lobby::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_SkyBox(TEXT("Layer_SkyBox"))))
		return E_FAIL;
	//임구이 안 쓸 때
	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;
	/////////////////////////

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Load_Stage()))
		return E_FAIL;

	/*if (FAILED(Ready_Layer_BackGround(TEXT("Layer_Lobby"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_WaitingRoom(TEXT("Layer_WaitingRoom"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Store(TEXT("Layer_Store"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_AddItems(TEXT("Layer_AddItems"))))
		return E_FAIL;*/


	SetWindowText(g_hWnd, TEXT("Level_Lobby_Main. "));
	return S_OK;
}

void CLevel_Lobby::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		
	
	GAMEINSTANCE->Add_Light(m_pBaseLight);

//	//이게 맞냐
//#pragma region Lobby_Main
//	if (m_pLobby->Get_Enable())//로비
//	{
//		_uint iSelectedMenu = static_cast<CUIBackground*>(m_pLobby)->Selected_Menu();
//
//		switch (iSelectedMenu)
//		{
//		case 1:
//			SetWindowText(g_hWnd, TEXT("Level_Lobby_WaitingRoom. "));
//			m_pLobby->Set_Enable(false);
//			m_pWaitingRoom->Set_Enable(true);
//			break;
//
//		case 2:
//			SetWindowText(g_hWnd, TEXT("Level_Lobby_Store. "));
//			m_pLobby->Set_Enable(false);
//			m_pStore->Set_Enable(true);
//			break;
//
//		}
//	}
//#pragma endregion Lobby_Main
//
//#pragma region  WaitingRoom
//	else if (m_pWaitingRoom->Get_Enable())//대기실
//	{
//		_uint iSelectedMenu = static_cast<CUIBackground*>(m_pWaitingRoom)->Selected_Menu();
//
//		switch (iSelectedMenu)
//		{
//		case 2://추가
//			SetWindowText(g_hWnd, TEXT("Level_Lobby_AddItems. "));
//			m_pWaitingRoom->Set_Enable(false);
//			m_pAddItems->Set_Enable(true);
//			break;
//
//		case 3://구입
//			SetWindowText(g_hWnd, TEXT("Level_Lobby_Store. "));
//			m_pWaitingRoom->Set_Enable(false);
//			m_pStore->Set_Enable(true);
//			break;
//
//		case 4://떠나기
//			SetWindowText(g_hWnd, TEXT("Level_Lobby_Main. "));
//			m_pLobby->Set_Enable(true);
//			m_pWaitingRoom->Set_Enable(false);
//			break;
//
//		case 6://시작
//			m_pWaitingRoom->Set_Enable(false);
//			if (FAILED(CGameInstance::Get_Instance()->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY), LEVEL_GAMEPLAY)))
//				return;
//			break;
//		}
//	}
//#pragma endregion  WaitingRoom


}

HRESULT CLevel_Lobby::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	

	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Layer_SkyBox(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.Lobby*/
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Layer_Terrain(const _tchar* pLayerTag)
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;



	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Layer_Player(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_float4 vPosition = _float4(10.f, 0.f, 50.f, 1.f);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_Player"), nullptr, &vPosition)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Lights()
{
	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
	LightDesc.vSpecular = _float4(0.2f, 0.2f, 0.2f, 1.f);

	m_pBaseLight = CLight::Create(m_pDevice, m_pContext, LightDesc);
	if (nullptr == m_pBaseLight)
		return E_FAIL;


	return S_OK;

}

HRESULT CLevel_Lobby::Load_Stage()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	char Filepath[255] = "../Bin/Resources/Map/Lobby/Lobby";
	HANDLE hFile = CreateFileA(Filepath,
		GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed to load file");
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}
	DWORD dwByteHouse = 0;
	MAP_DATA tLobbyPos;
	ZeroMemory(&tLobbyPos, sizeof(MAP_DATA));
	_float4x4	WorldMat;
	while (true)
	{
		if (TRUE == ReadFile(hFile, &tLobbyPos, sizeof(MAP_DATA), &dwByteHouse, nullptr))
		{
			if (0 == dwByteHouse)
			{
				break;
			}
			
			XMStoreFloat4x4(&WorldMat, tLobbyPos.matWorld);

			if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, TEXT("Layer_Lobby"), TEXT("Prototype_GameObject_Lobby"),
				nullptr, &WorldMat)))
				return E_FAIL;
		}
	}

	CloseHandle(hFile);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.Lobby*/
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_Lobby"),&m_pLobby)))
		return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Layer_WaitingRoom(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.WaitingRoom*/
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_WaitingRoom"), &m_pWaitingRoom)))
		return E_FAIL;

		m_pWaitingRoom->Set_Enable(false);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Layer_Store(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.WaitingRoom*/
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_Store"), &m_pStore)))
		return E_FAIL;

	m_pStore->Set_Enable(false);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Layer_AddItems(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.WaitingRoom*/
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_AddItems"), &m_pAddItems)))
		return E_FAIL;

	m_pAddItems->Set_Enable(false);

	Safe_Release(pGameInstance);
	return S_OK;
}


CLevel_Lobby * CLevel_Lobby::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Lobby*		pInstance = new CLevel_Lobby(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Lobby");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Lobby::Free()
{
	__super::Free();

	Safe_Release(m_pBaseLight);

}

