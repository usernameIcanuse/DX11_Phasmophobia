#pragma once

#include"Enemy.h"
#include<iostream>

using namespace std;
void Enemy::Enemy_Easy() {
	strcpy_s(szRole, "�ʱ�");
	iHP = iHPFull = 30;
	iPower = 5;
}

void Enemy::Enemy_Normal() {
	strcpy_s(szRole, "�߱�");
	iHP = iHPFull = 50;
	iPower = 25;
}

void Enemy::Enemy_Hard() {
	strcpy_s(szRole, "���");
	iHP = iHPFull = 150;
	iPower = 50;
}

void Enemy::Print_Enemy() const {
	cout << "���� : " << szRole << '\n';
	cout << "���ݷ�: " << iPower << '\n';
	cout << "ü��: " << iHP << '\t' << iHPFull << '\n';
}

