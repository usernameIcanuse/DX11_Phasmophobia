#pragma once

#include"CGameMenu.h"
#include<iostream>
#include<time.h>
using namespace std;

enum MENU {
	Worrior = 1,
	Magician,
	Thief,
	Load,
	Esc
};

void CGameMenu::Menu(Player* player1) {

	bool flag = true;
	int Continue;
	int menu;
	while (flag) {
		system("cls");
		cout << "1.���� 2.������ 3.���� 4.�ҷ����� 5. ���� :";
		cin >> menu;
		switch (menu) {
		case Worrior:
			player1->Player_Worrior();
			break;
		case Magician:
			player1->Player_Magician();
			break;
		case Thief:
			player1->Player_Thief();
			break;
		case Load:
			Continue = player1->Load_Player();
			if (Continue == 0) continue;
			break;
		case Esc:
			return;
			break;
		default:
			cout << "�߸� �Է��ϼ̽��ϴ�.\n";
			system("pause");
			break;
		}
		flag = Select(player1);
	}
}


bool CGameMenu::Select(Player* p) {

	int menu;
	Enemy* enemy = new Enemy;
	bool bFlag = true;

	while (1) {
		system("cls");
		p->Print_Player();
		cout << "=================================\n";
		cout << "1. ����� 2. �����ϱ� 3. ���� : ";
		cin >> menu;

		switch (menu) {
		case 1:
			Enemy_Select(p, enemy);
			break;
		case 2:
			bFlag = p->Save();
			if (bFlag) {
				cout << "�����Ͽ����ϴ�.\n";
				system("pause");
			}
			break;
		case 3:
			delete enemy;
			enemy = nullptr;
			return false;
			break;
		}
	}


}





void CGameMenu::Field(Player* p, Enemy* e) {
	int menu;
	int iFlag = 0;
	srand(unsigned(time(NULL)));
	while (1) {
		system("cls");
		p->Print_Player();
		cout << "=================================\n";
		e->Print_Enemy();
		cout << "=================================\n";
		cout << "1. ���� 2. ���� \n";
		cout << "=================================\n";
		cout << "�Է� : ";
		cin >> menu;

		switch (menu) {
		case 1:
			iFlag = p->Fighting(e);
			break;
		case 2:
			system("pause");
			return;
			break;
		}

		if (iFlag == 1) {
			cout << "�÷��̾� ���\n"; system("pause");
			p->Set_Zero();
			break;
		}
		else if (iFlag == -1) {
			cout << "��� ����\n";
			p->Level_Up(e);
			system("pause");
			break;
		}
	}
}

void CGameMenu::Enemy_Select(Player* p, Enemy* e) {
	int menu;
	while (1) {
		system("cls");
		p->Print_Player();
		cout << "=================================\n";
		cout << "1. �ʱ� 2. �߱� 3. ��� 4. ���ܰ�: ";
		cin >> menu;
		switch (menu) {
		case 1:
			e->Enemy_Easy();
			break;
		case 2:
			e->Enemy_Normal();
			break;
		case 3:
			e->Enemy_Hard();
			break;
		case 4:
			return;
			break;
		}
		Field(p, e);

	}
}