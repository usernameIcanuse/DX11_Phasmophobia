#include"Store.h"
#include<time.h>

void		CStore::Initialize(void)
{
	m_pItem[0][0] = new Item("초급 무기", 5,0,100,1);
	m_pItem[1][0] = new Item("중급 무기", 10, 0, 200,1);
	m_pItem[2][0] = new Item("고급 무기", 15, 0, 300,1);

	m_pItem[0][1] = new Item("초급 방어구", 0, 30, 100,2);
	m_pItem[1][1] = new Item("중급 방어구", 0, 40, 200,2);
	m_pItem[2][1] = new Item("고급 방어구", 0, 50, 300,2);
}

void		CStore::Update(void)
{
	int iInput;
	while (true)
	{
		system("cls");
		cout << "======================================\n";
		cout << "1. 상점\t2.아이템 팔기\t3. 무기강화\t4. 나가기 : ";
		cin >> iInput;

		switch (iInput)
		{
		case 1:
			Store();
			break;

		case 2:
			Sell_Item();
			break;

		case 3:
			Strength();
			break;

		case 4:
			return; break;
		default:
			break;
		}
	}
}

void		CStore::Strength(void)
{
	int		_iInput;
	while (1)
	{	
		system("cls");
		m_pPlayer->Render();
		cout << "======================================\n";
		cout << "강화 전: " << m_pPlayer->Get_Attack() << endl;
		cout << "강화 후 예상 공격력: " << m_pPlayer->Get_Attack() + 5 << " ~ " << m_pPlayer->Get_Attack() + 20 << endl;
		cout << "======================================\n";
		cout << "강화 비용 : " << 100 << "G\n";
		cout << "소지금 : " << dynamic_cast<CPlayer*>(m_pPlayer)->Get_Money();
		cout << "\n======================================\n";
		cout << "1. 강화하기		2. 뒤로가기 : ";
		cin >> _iInput;
		cout << "======================================\n";

		switch (_iInput)
		{
		case 1:
			srand(unsigned(time(NULL)));
			m_pPlayer->Increase_Attack(rand() % 16 + 5);
			cout << "강화 성공!!" << endl;
			cout << "변화한 공격력 : " << m_pPlayer->Get_Attack();
			system("pause");
			break;

		case 2:
			return;
		}

	}
}


void		CStore::Store()
{
	int _eLevel;
	while (1)
	{
		system("cls");

		cout << "======================================\n";
		cout << "1. 초급 2. 중급 3. 고급 4. 뒤로가기\n";
		cout << "======================================\n";
		cout << ">> ";
		cin >> _eLevel;

		switch (_eLevel)
		{
		case 1:
		case 2:
		case 3:
			Initialize();
			Render(_eLevel-1);
			Release();
			break;

		case 4:
			return;
		}
		
	}
}

void		CStore::Render(int	_eLevel)
{	
	int _iMenu =0;
	while (1)
	{
		system("cls");
		cout << "======================================\n";
		m_pItem[_eLevel][0]->Render();
		cout << "======================================\n";
		m_pItem[_eLevel][1]->Render();
		cout << "======================================\n";
		cout << "골드 : " << dynamic_cast<CPlayer*>(m_pPlayer)->Get_Money()<<endl;
		cout << "구입 (1. 무기 / 2. 방어구 /3. 뒤로가기)>> ";
		cin >> _iMenu;
		

		switch (_iMenu)
		{
		case 1:
		case 2:
			if (dynamic_cast<CPlayer*>(m_pPlayer)->Is_Full_Inven())
			{
				cout << "인벤토리가 가득 찼습니다.\n";
				system("pause");
				continue;
			}

			if (dynamic_cast<CPlayer*>(m_pPlayer)->Get_Money() < m_pItem[_eLevel][_iMenu - 1]->Get_Money())
			{
				cout << "잔액이 부족합니다.\n";
				system("pause");
				continue;
			}
			Buy_Item(*m_pItem[_eLevel][_iMenu - 1]);
			break;

		case 3:
			return;

		default:
			continue;
		}
 	}
}

void		CStore::Buy_Item(Item&	Temp)
{
	dynamic_cast<CPlayer*>(m_pPlayer)->Get_Item(Temp);
	dynamic_cast<CPlayer*>(m_pPlayer)->Money_Minus(Temp.Get_Money());
	system("pause");
}

void		CStore::Sell_Item()
{
	while (1)
	{
		int iMenu;
		system("cls");
		dynamic_cast<CPlayer*>(m_pPlayer)->Render_Inventory();
		cout << "======================================\n";
		cout << "골드 : " << dynamic_cast<CPlayer*>(m_pPlayer)->Get_Money()<<endl;
		cout << "======================================\n";
		cout << "판매할 아이템(뒤로가려면 0입력) : ";
		cin >> iMenu;

		if (0 == iMenu) return;

		dynamic_cast<CPlayer*>(m_pPlayer)->Sell_Item(iMenu);

	}
}

void		CStore::Release()
{
	for(int i=0;i<3;++i)
		for (int j = 0; j < 2; ++j)
		{
			if (m_pItem[i][j])
			{
				SAFE_RELEASE(m_pItem[i][j]);
				m_pItem[i][j] = nullptr;
			}
		}
}