#include "stdafx.h"
#include "..\Public\Level_Lobby.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Lobby.h"
#include "Lobby_WaitingRoom.h"



CLevel_Lobby::CLevel_Lobby(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Lobby::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;


	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_Lobby"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_WaitingRoom(TEXT("Layer_WaitingRoom"))))
		return E_FAIL;


	return S_OK;
}

void CLevel_Lobby::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		
	

	//이게 맞냐
#pragma region Lobby_Main
	if (m_pLobby->Get_Enable())//로비
	{
		_uint iSelectedMenu = static_cast<CLobby*>(m_pLobby)->Selected_Menu();

		switch (iSelectedMenu)
		{
		case 1:
			m_pLobby->Set_Enable(false);
			m_pWaitingRoom->Set_Enable(true);
			break;
		}
	}
#pragma endregion Lobby_Main

#pragma region  WaitingRoom
	else if (m_pWaitingRoom->Get_Enable())//대기실
	{
		_uint iSelectedMenu = static_cast<CLobby_WaitingRoom*>(m_pWaitingRoom)->Selected_Menu();

		switch (iSelectedMenu)
		{
		case 4:
			m_pLobby->Set_Enable(true);
			m_pWaitingRoom->Set_Enable(false);
			break;
		}
	}
#pragma endregion  WaitingRoom


}

HRESULT CLevel_Lobby::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("Level_Lobby. "));

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

}

