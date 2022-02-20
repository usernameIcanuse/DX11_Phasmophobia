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
		cout << "1.전사 2.마법사 3.도둑 4.불러오기 5. 종료 :";
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
			cout << "잘못 입력하셨습니다.\n";
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
		cout << "1. 사냥터 2. 저장하기 3. 종료 : ";
		cin >> menu;

		switch (menu) {
		case 1:
			Enemy_Select(p, enemy);
			break;
		case 2:
			bFlag = p->Save();
			if (bFlag) {
				cout << "저장하였습니다.\n";
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
		cout << "1. 전투 2. 도망 \n";
		cout << "=================================\n";
		cout << "입력 : ";
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
			cout << "플레이어 사망\n"; system("pause");
			p->Set_Zero();
			break;
		}
		else if (iFlag == -1) {
			cout << "사냥 성공\n";
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
		cout << "1. 초급 2. 중급 3. 고급 4. 전단계: ";
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