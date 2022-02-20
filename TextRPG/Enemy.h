#pragma once

class Enemy {

public:
	char szRole[10];
	int iPower;
	int iHP;
	int iHPFull;

	
	void Enemy_Easy();
	void Enemy_Normal();
	void Enemy_Hard();
	void Print_Enemy () const;
};
