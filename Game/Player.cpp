#pragma once

#include "Player.h"
#include "string.h"

void	CPlayer::Initialize(string _szName, int _iAttack, int _iHPMax, int _iLevel)
{
	m_tInfo->m_szName = _szName;
	m_tInfo->m_iAttack = _iAttack;
	m_tInfo->m_iHP = m_tInfo->m_iHPMax = _iHPMax;
	m_tInfo->m_iLevel = _iLevel;
	m_iExp = 0;
	m_iExpMax = 100;
}


void	CPlayer::Set_Zero(void)
{
	CObj::Reset_HP();
	m_iExp = 0;
}

void	CPlayer::Get_Exp(int _iExp)
{
	m_iExp += _iExp;
	if (m_iExp >= m_iExpMax)
	{
		CObj::Level_Up();
		m_iExp -= m_iExpMax;
		m_iExpMax += 100;
	}
}


void	CPlayer::Select_Job(void)
{
	system("cls");
	int iInput;
	cout << "직업을 선택하세요 (1. 전사	2. 마법사	3.도둑) :";
	cin >> iInput;

	switch (iInput)
	{
	case 1:
		Initialize("전사", 20, 200, 1);
		
		break;

	case 2:
		Initialize("마법사", 50, 100, 1);
		break;

	case 3:
		Initialize("도둑", 10, 50, 1);
		break;

	default:
		break;
	}
}

void	CPlayer::Render(void)	const
{
	cout << "직업:\t" << m_tInfo->m_szName << endl;
	cout << "공격력:\t" << m_tInfo->m_iAttack << endl;
	cout << "체력:\t" << m_tInfo->m_iHP << '\t' << m_tInfo->m_iHPMax << endl;
	cout << "레벨:\t" << m_tInfo->m_iLevel;
	cout<< " ( " << m_iExp <<" / "<<m_iExpMax<<" )"<< endl;
}

void		CPlayer::Render_Inventory(void)
{

	if (m_vInventory.empty())
	{
		cout << "인벤토리가 비어 있습니다\n";
	}

	else
	{
		vector<Item*>::iterator		iter = m_vInventory.begin();
		for (int i=1; iter != m_vInventory.end(); ++i, ++iter)
		{
			cout << i << endl;
			(*iter)->Render();
			cout << "======================================\n";
		}
	}
	cout << "골드 : " << m_iMoney << "G\n";



}

bool		CPlayer::Is_Full_Inven(void)
{
	if (m_vInventory.size() == 5) return true;
	else return false;
}
void		CPlayer::Get_Item(Item&	_item)
{
	Item*		Temp = new Item(_item);

	m_vInventory.push_back(Temp);
	cout << "구매 성공!\n";
}

void	CPlayer::Release_Inventory()
{
	if (!m_vInventory.empty())
	{
		vector<Item*>::iterator		iter = m_vInventory.begin();
		for (; iter != m_vInventory.end();)
		{
			SAFE_RELEASE(*iter);
			iter = m_vInventory.erase(iter);
		}
	}

	if (!m_vCap.empty())
	{
		vector<Item*>::iterator		iter = m_vCap.begin();
		for (; iter != m_vCap.end();)
		{
			SAFE_RELEASE(*iter);
			iter = m_vCap.erase(iter);
		}
	}
}

void		CPlayer::Loading(FILE*	fp)
{

	fread(m_tInfo, sizeof(Info), 1, fp);
	fread(&m_iExp, sizeof(int), 1, fp);
	fread(&m_iExpMax, sizeof(int), 1, fp);
	fread(&m_iMoney, sizeof(int), 1, fp);

	cout << "불러오기 완료!" << endl;
	system("pause");

}

void		CPlayer::Saving(FILE*	fp)
{	

	fwrite(m_tInfo, sizeof(Info), 1, fp);
	fwrite(&m_iExp, sizeof(int), 1, fp);
	fwrite(&m_iExpMax, sizeof(int), 1, fp);
	fwrite(&m_iMoney, sizeof(int), 1, fp);
	cout << "저장하였습니다." << endl;
	system("pause");

}
//string 한글 문자열 비교가 안되나?
void		CPlayer::Delete_Item()
{
	
	while (1)
	{
		system("cls");
		bool bFlag = true;
		int iIndex;

		system("cls");
		Render_Inventory();
		cout << "삭제할 아이템 (뒤로가려면 0입력) : ";
		cin >> iIndex;

		if (0 == iIndex) return;
		if (1 > iIndex || m_vInventory.size() < iIndex) continue;

		int iNum;
		vector<Item*>::iterator		iter = m_vInventory.begin() +iIndex-1;
		(*iter)->Render();
		cout << "아이템을 삭제하시겠습니까?( 1.예 / 2. 아니오 ) : ";
		cin >> iNum;
		if (iNum == 1)
		{
			SAFE_RELEASE(*iter);
			iter = m_vInventory.erase(iter);
		}
		
	}
}

void		CPlayer::Sell_Item(int	_iIndex)
{
	vector<Item*>::iterator		iter = m_vInventory.begin() + _iIndex - 1;
	
	m_iMoney += (*iter)->Get_Money();
	SAFE_RELEASE(*iter);
	iter = m_vInventory.erase(iter);

}

void		CPlayer::Render_Cap()
{
	if (m_vCap.empty())
	{
		cout << "장착한 아이템이 없습니다.\n";
	}
	else
	{
		cout << "장착한 아이템 :\n";
		vector<Item*>::iterator		iter = m_vCap.begin();
		for (int i=1; iter != m_vCap.end();++i, ++iter)
		{
			cout << i << endl;
			(*iter)->Render();
			cout << "======================================\n";
		}
	}
}

void		CPlayer::Puton_Item()
{
	while (1)
	{
		bool	bFlag = false;
		int	Find;
		system("cls");
		Render();
		cout << "==========장착한 아이템==========\n";
		Render_Cap();
		cout << "======================================\n";
		Render_Inventory();
		cout << "======================================\n";
		cout << "장착할 장비(뒤로가려면 0입력) : ";
		cin >> Find;

		if (0 == Find) return;
		if (1 > Find || m_vInventory.size() < Find) continue;

		vector<Item*>::iterator		iterI = m_vInventory.begin() + Find -1;
		vector<Item*>::iterator		iterC = m_vCap.begin();

		for (; iterC != m_vCap.end(); ++iterC)
		{
			if ((*iterI)->Get_Num() == (*iterC)->Get_Num())
			{
				cout << "아이템을 중복 착용할 수 없습니다\n";
				system("pause");
				bFlag = true;
				break;
			}
		}

		if (bFlag || iterC == m_vCap.end()) continue;
		
		m_vCap.push_back(*iterI);
		iterI = m_vInventory.erase(iterI);
		m_tInfo->m_iAttack += (*iterI)->Get_Attack();
		m_tInfo->m_iHPMax += (*iterI)->Get_Shield();
	}
}


void		CPlayer::Putoff_Item()
{
	while (1)
	{
		bool	bFlag = false;
		int	Find;
		system("cls");
		Render();
		cout << "======================================\n";
		Render_Cap();
		cout << "======================================\n";
		cout << "해제할 장비(뒤로가려면 0 입력) : ";
		cin >> Find;

		if (0 == Find) return;

		vector<Item*>::iterator		iter = m_vCap.begin()+Find-1;

		
		if (5 > m_vInventory.size())
		{
			m_vInventory.push_back(*iter);
			iter = m_vCap.erase(iter);
			m_tInfo->m_iAttack -= (*iter)->Get_Attack();
			m_tInfo->m_iHPMax -= (*iter)->Get_Shield();
			if (m_tInfo->m_iHP > m_tInfo->m_iHPMax)
			{
				m_tInfo->m_iHP = m_tInfo->m_iHPMax;
			}
		}
		else
		{
			cout << "인벤토리가 가득 찼습니다.\n";
			system("pause");
		}
	}
	

}

void		CPlayer::Set_Item()
{
	int		_iInput;
	while (1)
	{
		system("cls");
		Render();
		cout << "======================================\n";
		Render_Cap();
		cout << "======================================\n";
		Render_Inventory();
		cout << "======================================\n";
		cout << "1. 장착\t2. 해제\t3. 뒤로가기 : ";
		cin >> _iInput;

		switch (_iInput)
		{
		case 1:
			Puton_Item();
			break;

		case 2:
			Putoff_Item();
			break;

		case 3:
			return;

		default:
			break;
		}
	}
}