#pragma once
#include"Player.h"
#include<iostream>

using namespace std;

enum MENU {
	Worrior = 1,
	Magician,
	Thief,
	Load,
	Esc
};

int Player::Load_Player() {
	int menu;

	FILE* fp1 = nullptr;
	FILE* fp2 = nullptr;
	FILE* fp3 = nullptr;
	system("cls");
	errno_t err1 = fopen_s(&fp1, "../TextRPG/Worrior.txt", "rb");
	if (0 == err1) {
		cout << "Save 1: ����\t";
	}
	else {
		cout << "Save 1: empty\t";
	}
	errno_t err2 = fopen_s(&fp2, "../TextRPG/Magician.txt", "rb");
	if (0 == err2) {
		cout << "Save 2: ������\t";
	}
	else {
		cout << "Save 2: empty\t";
	}
	errno_t err3 = fopen_s(&fp3, "../TextRPG/Thief.txt", "rb");
	if (0 == err3) {
		cout << "Save 3: ����\t";
	}
	else {
		cout << "Save 3: empty (0��: ���ư���):";
	}
	cin >> menu;

	if (menu == 0) return 0;

	else if (menu == 1) {
		fread(p, sizeof(player), 1, fp1);
	}
	else if (menu == 2) {
		fread(p, sizeof(player), 1, fp2);
	}
	else if (menu == 3) {
		fread(p, sizeof(player), 1, fp3);
	}
	return 1;


}

void Player::Print_Player() const {
	cout << "���� : " << p->szRole << '\n';
	cout << "���ݷ� : " << p->iPower << '\n';
	cout << "ü�� : " << p->iHP << "\t" << p->iHPFull << '\n';
	cout << "���� : " << p->iLevel << " ( " << p->iExp << " / " << p->iExpFull << " )\n";
}

void Player::Player_Worrior() {
	strcpy_s(p->szRole, "����");
	p->iSerialNum = Worrior;
	p->iPower = 10;
	p->iHP = 200;
	p->iHPFull = 200;
	p->iLevel = 1;
	p->iExp = 0;
	p->iExpFull = 100;


}
void Player::Player_Magician() {
	strcpy_s(p->szRole, "������");
	p->iSerialNum = Magician;
	p->iPower = 100;
	p->iHP = 100;
	p->iHPFull = 100;
	p->iLevel = 1;
	p->iExp = 0;
	p->iExpFull = 100;
}
void Player::Player_Thief() {
	strcpy_s(p->szRole, "����");
	p->iSerialNum = Thief;
	p->iPower = 20;
	p->iHP = 50;
	p->iHPFull = 50;
	p->iLevel = 1;
	p->iExp = 0;
	p->iExpFull = 100;
}



bool Player::Save() {
	FILE* fp = nullptr;
	if (p->iSerialNum == Worrior) {
		errno_t err = fopen_s(&fp, "../TextRPG/Worrior.txt", "wb");
		if (0 == err) {
			fwrite(p, sizeof(player), 1, fp);
			fclose(fp);
		}
		else {
			cout << "�������� ���߽��ϴ�.\n";
			system("pause");
			return false;
		}
	}
	else if (p->iSerialNum == Magician) {
		errno_t err = fopen_s(&fp, "../TextRPG/Magician.txt", "wb");
		//fopen_s�� ó�� ������ �� ���� ��� ������ 2ȸ�̻����δ� Permission denied�߻�
		if (0 == err) {
			fwrite(p, sizeof(player), 1, fp);
			fclose(fp);

		}
		else {
			cout << "�������� ���߽��ϴ�.\n";
			system("pause");
			return false;
		}
	}
	else if (p->iSerialNum == Thief) {
		errno_t err = fopen_s(&fp, "../TextRPG/Thief.txt", "wb");
		if (0 == err) {
			fwrite(p, sizeof(player), 1, fp);

			fclose(fp);
		}
		else {
			cout << "�������� ���߽��ϴ�.\n";
			system("pause");
			return false;
		}
	}
	return true;
}

void Player::Set_Zero() {
	p->iHP = p->iHPFull;
	p->iExp = 0;
}

void Player::Level_Up(Enemy* e) {
	p->iExp += rand() % e->iHPFull + 10;
	if (p->iExp >= p->iExpFull) {
		p->iExp -= p->iExpFull;
		p->iExpFull += 100;
		p->iHP = p->iHPFull += 100;
		++p->iLevel;
		p->iPower += 50;
	}
}

int Player::Fighting(Enemy* e) {
	p->iHP -= e->iPower;
	if (p->iHP <= 0) return 1;
	e->iHP -= p->iPower;
	if (e->iHP <= 0) return -1;
	return 0;
}