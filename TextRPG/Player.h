#pragma once
#include"Enemy.h"

struct player {
	char szRole[10];
	int iSerialNum;
	int iPower;
	int iHP;
	int iHPFull;
	int iLevel;
	int iExp;
	int iExpFull;

	player()
	{
		iSerialNum = 0;
		iPower= 0;
		iHP= 0;
		iHPFull= 0; 
		iLevel= 0;
		iExp = 0;
		iExpFull = 0;
	}
};


class Player {
private:
	player *p;

public:
	Player() 
		:p (new player)
	{
	}
	~Player() {
		delete p;
		p = nullptr;
	}
	void Player_Worrior();
	void Player_Magician();
	void Player_Thief();
	void Print_Player() const;
	int Load_Player();
	void Set_Zero();
	bool Save();
	void Level_Up(Enemy* e);
	int Fighting(Enemy* e);
};