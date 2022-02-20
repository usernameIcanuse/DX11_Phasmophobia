#pragma once

#include"Enemy.h"
#include<iostream>

using namespace std;
void Enemy::Enemy_Easy() {
	strcpy_s(szRole, "초급");
	iHP = iHPFull = 30;
	iPower = 5;
}

void Enemy::Enemy_Normal() {
	strcpy_s(szRole, "중급");
	iHP = iHPFull = 50;
	iPower = 25;
}

void Enemy::Enemy_Hard() {
	strcpy_s(szRole, "고급");
	iHP = iHPFull = 150;
	iPower = 50;
}

void Enemy::Print_Enemy() const {
	cout << "직업 : " << szRole << '\n';
	cout << "공격력: " << iPower << '\n';
	cout << "체력: " << iHP << '\t' << iHPFull << '\n';
}

