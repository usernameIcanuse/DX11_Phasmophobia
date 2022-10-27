#include "stdafx.h"
#include "..\Public\Level_Loading.h"
#include "Loader.h"
#include "Level_Logo.h"
#include "Level_Lobby.h"
#include "Level_Tutorial.h"
#include "Level_StreetHouse.h"
#include "GameInstance.h"

CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevel, _bool bFirst, _bool bLobby,STAGE eStage)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;


	m_eNextLevel = eNextLevel;
	m_bLobby = bLobby;
	m_eStage = eStage;

	m_pLoader = CLoader::Create(m_pDevice, m_pContext, eNextLevel, bFirst,eStage);
	if (nullptr == m_pLoader)
		return E_FAIL;

	if (false == bFirst)
	{
		if (FAILED(Ready_Layer_Loading(TEXT("Layer_Loading"))))
			return E_FAIL;
	}
	else
	{
		if (FAILED(Ready_Layer_FirstLoading(TEXT("Layer_Loading"))))
			return E_FAIL;
	}

	return S_OK;
}

void CLevel_Loading::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	


	if (true == m_pLoader->is_Finished())
	{
		GAMEINSTANCE->Clear_Cameras();
		GAMEINSTANCE->Clear_List();
		CLevel*			pLevel = nullptr;

		switch (m_eNextLevel)
		{
		case LEVEL_LOGO:
			//GAMEINSTANCE->Set_Current_Level(LEVEL_LOGO);
			pLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_LOBBY:
			//GAMEINSTANCE->Set_Current_Level(LEVEL_LOBBY);
			pLevel = CLevel_Lobby::Create(m_pDevice, m_pContext, m_bLobby);
			break;
		case LEVEL_GAMEPLAY:
			if(TUTORIAL == m_eStage)
				pLevel = CLevel_Tutorial::Create(m_pDevice, m_pContext);
			if (STREETHOUSE == m_eStage)
				pLevel = CLevel_StreetHouse::Create(m_pDevice, m_pContext);
			break;
		}

		if (nullptr == pLevel)
			return;

		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->Add_ReserveLevel(m_eNextLevel, pLevel, m_eNextLevel)))
			return;

		Safe_Release(pGameInstance);
		
	}
}

HRESULT CLevel_Loading::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, m_pLoader->Get_LoadingText());	

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_Loading(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	/* For.Prototype_Component_Texture_Default */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_Component_Texture_Loading"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Phasmophobia_Loading.dds")))))
		return E_FAIL;

	/* For.Loading */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOADING, pLayerTag, TEXT("Prototype_GameObject_Loading"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_FirstLoading(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	/* For.Prototype_Component_Texture_FirstLoading*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_Component_Texture_FirstLoading"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Phasmophobia_Loading%d.png"),4))))
		return E_FAIL;

	/* For.FirstLoading */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOADING, pLayerTag, TEXT("Prototype_GameObject_FirstLoading"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevel, _bool bFirst, _bool bLobby,STAGE eStage)
{
	CLevel_Loading*		pInstance = new CLevel_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel, bFirst, bLobby,eStage)))
	{
		MSG_BOX("Failed to Created : CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}

