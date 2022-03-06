#include"Field.h"

void	CField::Stage(void)
{
	m_pMonster = new CMonster[3];
	m_pMonster[0].Initialize("�ʱ�", 5, 30, 2);
	m_pMonster[1].Initialize("�߱�", 15, 60, 5);
	m_pMonster[2].Initialize("���", 30, 90, 10);

	while (true)
	{
		system("cls");
		int		iInput;
		m_pPlayer->Render();
		cout << "======================================\n";
		cout << "1. �ʱ�\t2. �߱�\t3. ���\t4. ���ܰ�: ";
		cin >> iInput;

		switch (iInput)
		{
		case 1:
			if(0>=m_pMonster[0].Get_HP())
			{
				m_pMonster[0].Reset_HP();
			}
			break;

		case 2:
			if (0>=m_pMonster[1].Get_HP())
			{
				m_pMonster[1].Reset_HP();
			}
			break;

		case 3:
			if (0>=m_pMonster[2].Get_HP())
			{
				m_pMonster[2].Reset_HP();
			}
			break;

		case 4:
			return;
			break;

		default:
			break;
		}
		
		Fighting(&m_pMonster[iInput-1]);		
	}
	
}

int	CField::Fighting(CObj* _pMonster)
{
	int		iInput;
	while (1) {
		system("cls");
		m_pPlayer->Render();
		cout << "======================================\n";
		_pMonster->Render();
		cout << "======================================\n";
		cout << "1. ����		2. ���� : ";
		cin >> iInput;

		switch (iInput)
		{
		case 1:
			m_pPlayer->Set_Damaged(_pMonster->Get_Attack());
			_pMonster->Set_Damaged(m_pPlayer->Get_Attack());
			if(0 >= m_pPlayer->Get_HP())
			{
				dynamic_cast<CPlayer*>(m_pPlayer)->Set_Zero();
				cout << "�÷��̾� ���!\n";system("pause");
				return -1;
			}
			else if (0 >= _pMonster->Get_HP())
			{
				cout << "��� ����!\n"; 
				cout << "����ġ +" << _pMonster->Get_Attack() + 20 <<endl;
				cout << "��� +" << _pMonster->Get_Attack() + 5 << "G\n";
				dynamic_cast<CPlayer*>(m_pPlayer)->Get_Exp(_pMonster->Get_Attack() + 20);
				dynamic_cast<CPlayer*>(m_pPlayer)->Money_Plus(_pMonster->Get_Attack() + 5);
				system("pause");
				
				return 1;
			}
			break;

		case 2:
			return 0; break;

		default:
			break;
			
		}
	}
}

void	CField::Release_Monster(void)
{
	if (m_pMonster != nullptr) {
		delete[]m_pMonster;
		m_pMonster = nullptr;
	}
}