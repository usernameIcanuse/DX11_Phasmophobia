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
	cout << "������ �����ϼ��� (1. ����	2. ������	3.����) :";
	cin >> iInput;

	switch (iInput)
	{
	case 1:
		Initialize("����", 20, 200, 1);
		
		break;

	case 2:
		Initialize("������", 50, 100, 1);
		break;

	case 3:
		Initialize("����", 10, 50, 1);
		break;

	default:
		break;
	}
}

void	CPlayer::Render(void)	const
{
	cout << "����:\t" << m_tInfo->m_szName << endl;
	cout << "���ݷ�:\t" << m_tInfo->m_iAttack << endl;
	cout << "ü��:\t" << m_tInfo->m_iHP << '\t' << m_tInfo->m_iHPMax << endl;
	cout << "����:\t" << m_tInfo->m_iLevel;
	cout<< " ( " << m_iExp <<" / "<<m_iExpMax<<" )"<< endl;
}

void		CPlayer::Render_Inventory(void)
{

	if (m_vInventory.empty())
	{
		cout << "�κ��丮�� ��� �ֽ��ϴ�\n";
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
	cout << "��� : " << m_iMoney << "G\n";



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
	cout << "���� ����!\n";
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

	cout << "�ҷ����� �Ϸ�!" << endl;
	system("pause");

}

void		CPlayer::Saving(FILE*	fp)
{	

	fwrite(m_tInfo, sizeof(Info), 1, fp);
	fwrite(&m_iExp, sizeof(int), 1, fp);
	fwrite(&m_iExpMax, sizeof(int), 1, fp);
	fwrite(&m_iMoney, sizeof(int), 1, fp);
	cout << "�����Ͽ����ϴ�." << endl;
	system("pause");

}
//string �ѱ� ���ڿ� �񱳰� �ȵǳ�?
void		CPlayer::Delete_Item()
{
	
	while (1)
	{
		system("cls");
		bool bFlag = true;
		int iIndex;

		system("cls");
		Render_Inventory();
		cout << "������ ������ (�ڷΰ����� 0�Է�) : ";
		cin >> iIndex;

		if (0 == iIndex) return;
		if (1 > iIndex || m_vInventory.size() < iIndex) continue;

		int iNum;
		vector<Item*>::iterator		iter = m_vInventory.begin() +iIndex-1;
		(*iter)->Render();
		cout << "�������� �����Ͻðڽ��ϱ�?( 1.�� / 2. �ƴϿ� ) : ";
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
		cout << "������ �������� �����ϴ�.\n";
	}
	else
	{
		cout << "������ ������ :\n";
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
		cout << "==========������ ������==========\n";
		Render_Cap();
		cout << "======================================\n";
		Render_Inventory();
		cout << "======================================\n";
		cout << "������ ���(�ڷΰ����� 0�Է�) : ";
		cin >> Find;

		if (0 == Find) return;
		if (1 > Find || m_vInventory.size() < Find) continue;

		vector<Item*>::iterator		iterI = m_vInventory.begin() + Find -1;
		vector<Item*>::iterator		iterC = m_vCap.begin();

		for (; iterC != m_vCap.end(); ++iterC)
		{
			if ((*iterI)->Get_Num() == (*iterC)->Get_Num())
			{
				cout << "�������� �ߺ� ������ �� �����ϴ�\n";
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
		cout << "������ ���(�ڷΰ����� 0 �Է�) : ";
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
			cout << "�κ��丮�� ���� á���ϴ�.\n";
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
		cout << "1. ����\t2. ����\t3. �ڷΰ��� : ";
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