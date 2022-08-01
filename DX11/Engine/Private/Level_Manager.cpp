#include "..\Public\Level_Manager.h"
#include "Level.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CLevel_Manager)

CLevel_Manager::CLevel_Manager()
{
}

HRESULT CLevel_Manager::Open_Level(_uint iLevelID, CLevel * pLevel)
{
	/* 이전레벨에서 사용한 자원들을 모두 소거한다. */
	if(nullptr != m_pCurrentLevel)
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->Clear(m_iCurrentLevelID)))
			return E_FAIL;

		Safe_Release(pGameInstance);
	}


	Safe_Release(m_pCurrentLevel);

	m_pCurrentLevel = pLevel;

	m_iCurrentLevelID = iLevelID;

	return S_OK;
}

void CLevel_Manager::Tick(_float fTimeDelta)
{
	if (nullptr == m_pCurrentLevel)
		return;

	m_pCurrentLevel->Tick(fTimeDelta);
}

HRESULT CLevel_Manager::Render()
{
	if (nullptr == m_pCurrentLevel)
		return E_FAIL;

	return m_pCurrentLevel->Render();
}

void CLevel_Manager::Free()
{
	Safe_Release(m_pCurrentLevel);
}
