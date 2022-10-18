#include "..\Public\Game_Manager.h"
#include "GameObject.h"
#include "Game_Manager.h"
#include "GameInstance.h"
#include "Level.h"

IMPLEMENT_SINGLETON(CGame_Manager)

CGame_Manager::CGame_Manager()
{
}


void CGame_Manager::Tick(_float fTimeDelta)
{
	for(int i = 0 ; i<EVENT_END ; ++i)
	{
		for (auto& elem : m_ObjectList[i])
		{
			elem->Call_EventFunc(fTimeDelta);
		}
	}
}

void CGame_Manager::Open_Level()
{
	for (auto& elem : m_LevelList)
	{
		GAMEINSTANCE->Open_Level(elem.iLevelID, elem.pLevel, elem.iNextLevelID);
	}
	m_LevelList.clear();
}

HRESULT CGame_Manager::Add_EventObject(_int iIndex,  CGameObject* pObject)
{
	if (EVENT_END <= iIndex)
		return E_FAIL;

	m_ObjectList[iIndex].push_back(pObject);
	Safe_AddRef(pObject);

	return S_OK;
}

HRESULT CGame_Manager::Add_ReserveLevel(_uint iLevelID, CLevel* pLevel, _uint iNextLevelID)
{
	OPENLEVELDESC	OpenLevelDesc;
	OpenLevelDesc.iLevelID = iLevelID;
	OpenLevelDesc.pLevel = pLevel;
	OpenLevelDesc.iNextLevelID = iNextLevelID;

	m_LevelList.push_back(OpenLevelDesc);

	return S_OK;
}

void CGame_Manager::Broadcast_Message(_int iIndex, const _tchar* pMessage)
{
	if (EVENT_END <= iIndex)
		return;

	for (auto& elem : m_ObjectList[iIndex])
	{
		elem->OnEventMessage(pMessage);
	}
}



void CGame_Manager::Clear_Layer(_int iIndex)
{
	if (EVENT_END <= iIndex)
		return;

	for (auto& elem : m_ObjectList[iIndex])
	{
		Safe_Release(elem);
	}

	m_ObjectList[iIndex].clear();

}

void CGame_Manager::Clear_List()
{
	for (int i = 0; i < EVENT_END; ++i)
	{
		for (auto& elem : m_ObjectList[i])
		{
			Safe_Release(elem);
		}
		m_ObjectList[i].clear();

	}

}



void CGame_Manager::Free()
{
	for (_uint i = 0; i < EVENT_END; ++i)
	{
		for (auto& elem : m_ObjectList[i])
			Safe_Release(elem);

		m_ObjectList[i].clear();
	}

}
