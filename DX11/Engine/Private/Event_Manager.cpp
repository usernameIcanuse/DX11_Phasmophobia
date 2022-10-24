#include "..\Public\Game_Manager.h"
#include "GameObject.h"
#include "Event_Manager.h"
#include "GameInstance.h"
#include "Level.h"

IMPLEMENT_SINGLETON(CEvent_Manager)

CEvent_Manager::CEvent_Manager()
{
}

void CEvent_Manager::Add_Desc(_uint eType, _float fValue)
{
	switch (eType)
	{
	case PLAYTIME:
		m_StageDesc.fTotalPlayTime += fValue;
		break;

	case HOUSETIME:
		m_StageDesc.fInHouseTime += fValue;
		break;

	case AROUNDGHOST:
		m_StageDesc.fAroundGhostTime += fValue;
		break;

	case EVENT:
		m_StageDesc.iEventCnt += (_int)fValue;
		break;

	case INTERACTION:
		m_StageDesc.iInteractionCnt += (_int)fValue;
		break;
	}
}

void CEvent_Manager::Set_GhostName(const _tchar* _szGhostName)
{
	lstrcpy(m_szGhostName, _szGhostName);
}

void CEvent_Manager::Clear_Desc()
{
	ZeroMemory(&m_StageDesc, sizeof(STAGEDESC));
}


void CEvent_Manager::Free()
{

}
