#pragma once
#include"Player.h"
#include"Enemy.h"

class CGameMenu {
public:
	void Menu(Player* p);
	bool Select(Player* p);
	void Field(Player* p, Enemy* e);
	void Enemy_Select(Player* p, Enemy* e);
};
