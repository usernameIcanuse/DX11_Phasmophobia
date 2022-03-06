#pragma once
#include"MainGame.h"

/*
void	CMainGame::Initialize(void)
{
	m_pField = new CField;
	m_pStore = new CStore;
	
	if (!m_pPlayer)
	{
		m_pPlayer = new CPlayer;
		
		dynamic_cast<CPlayer*>(m_pPlayer)->Select_Job();
	}
}
*/

void	CMainGame::Initialize(void)
{
	m_pField = new CField;
	m_pStore = new CStore;
	m_pPlayer = new CPlayer;
}

void	CMainGame::New_character(void)
{
	dynamic_cast<CPlayer*>(m_pPlayer)->Select_Job();
}

bool	CMainGame::Loading(void)
{
	m_pLoadSave->Set_Player(m_pPlayer);
	return m_pLoadSave -> Loading();
}


bool	CMainGame::MainMenu(void)
{	
	int iMenu;
	while (1)
	{
		system("cls");
		cout << "1. 새로 만들기\t2. 불러오기\t3.종료: ";
		cin >> iMenu;
		if (iMenu == 3) return false;
		Initialize();

		switch (iMenu)
		{
		case 1:
			New_character();
			return true;
			break;

		case 2:
			if (m_pLoadSave == nullptr)
			{
				m_pLoadSave = new CMemory;
				m_pLoadSave->Set_Player(m_pPlayer);
			}
			if(Loading()) continue;
			else return true;
			break;

		default:
			break;
		}
	}
	
}

void	CMainGame::Village(void)
{
	while (1) {
		int iInput;
		system("cls");
		m_pPlayer->Render();
		cout << "======================================\n";
		
		cout << "1. 사냥터\t2. 상점\t3. 인벤토리\t4. 저장하기\t5. 나가기 : ";
		cin >> iInput;

		switch (iInput)
		{
		case 1:
			m_pField->Set_Player(m_pPlayer);
			m_pField->Stage();
			break;

		case 2:
			m_pStore->Set_Player(m_pPlayer);
			m_pStore->Update();
			break;
		case 3:
			My_Profile();
			break;
		case 4:
			if(m_pLoadSave == nullptr){ m_pLoadSave = new CMemory; m_pLoadSave->Set_Player(m_pPlayer);}
			m_pLoadSave->Saving();
			break;
		case 5:
			return;
		default:
			break;
		}
		

	}
	
}

void		CMainGame::My_Profile(void)
{
	int _iMenu;

	while (1)
	{
		system("cls");
		m_pPlayer->Render();
		cout << "======================================\n";
		dynamic_cast<CPlayer*>(m_pPlayer)->Render_Inventory();
		cout << "======================================\n";
		cout << "1. 아이템 삭제\t2. 아이템 장착/해제\t3. 뒤로가기 :  ";
		cin >> _iMenu;

		switch (_iMenu)
		{
		case 1:
			dynamic_cast<CPlayer*>(m_pPlayer)->Delete_Item();
			break;

		case 2:
			dynamic_cast<CPlayer*>(m_pPlayer)->Set_Item();
			break;

		case 3:
			return;

		default:
			break;

		}

	}
	
}

void CMainGame::Release(void)
{
	SAFE_RELEASE(m_pPlayer);
	SAFE_RELEASE(m_pField);
	SAFE_RELEASE(m_pStore);
	SAFE_RELEASE(m_pLoadSave);
}