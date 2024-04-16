#include "stdafx.h"
#include "..\Public\Level_Lobby.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "MenuScreen.h"
#include "SoundMgr.h"



CLevel_Lobby::CLevel_Lobby(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Lobby::Initialize( _bool bGamePlay)
{
	m_bGamePlay = bGamePlay;

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_SkyBox(TEXT("Layer_SkyBox"))))
		return E_FAIL;
	//임구이 안 쓸 때
	//if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;
	/////////////////////////

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Load_Stage()))
		return E_FAIL;

	if (false == bGamePlay)
	{
		if (FAILED(GAMEINSTANCE->Change_Camera(TEXT("Camera_Player"))))
			return E_FAIL;
	}
	else
	{
		if (FAILED(GAMEINSTANCE->Change_Camera(TEXT("Camera_Fixed"))))
			return E_FAIL;
	}

	SetWindowText(g_hWnd, TEXT("Level_Lobby_Main. "));

	
	return S_OK;
}

void CLevel_Lobby::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		
	if (true == m_bFirst)
	{
		m_bFirst = false;
		CSoundMgr::Get_Instance()->PlaySound(TEXT("welcome back 1.wav"), CSoundMgr::BGM_RADIO, 1.f);
	}
	CSoundMgr::Get_Instance()->PlaySound(TEXT("nights ambience neighborhood.wav"), CSoundMgr::BGM, 1.f,true);

	GAMEINSTANCE->Add_Light(m_pBaseLight);

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
	LightDesc.vAmbient = _float4(0.5f, 0.5f, 0.5f, 1.f);
	LightDesc.vSpecular = _float4(0.5f, 0.5f, 0.5f, 1.f);

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

	strcpy_s(Filepath, "../Bin/Resources/Map/Lobby/MenuScreen");
	hFile = CreateFileA(Filepath,
		GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed to load file");
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}
	dwByteHouse = 0;
	
	ZeroMemory(&tLobbyPos, sizeof(MAP_DATA));
	
	while (true)
	{
		if (TRUE == ReadFile(hFile, &tLobbyPos, sizeof(MAP_DATA), &dwByteHouse, nullptr))
		{
			if (0 == dwByteHouse)
			{
				break;
			}

			XMStoreFloat4x4(&WorldMat, tLobbyPos.matWorld);
			CMenuScreen::SCREENDESC tDesc;
			tDesc.WorldMat = WorldMat;
			tDesc.bGamePlay = m_bGamePlay;

			if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, TEXT("Layer_Lobby"), TEXT("Prototype_GameObject_MenuScreen"),
				nullptr, &tDesc)))
				return E_FAIL;
		}
	}

	CloseHandle(hFile);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



CLevel_Lobby * CLevel_Lobby::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _bool bGamePlay)
{
	CLevel_Lobby*		pInstance = new CLevel_Lobby(pDevice, pContext);

	if (FAILED(pInstance->Initialize(bGamePlay)))
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

