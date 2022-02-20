#include"Player.h"
#include"CGameMenu.h"
#include<iostream>
using namespace std;


int main() {
	Player* player1 = new Player;
	CGameMenu* Menu = new CGameMenu;
	Menu->Menu(player1);

	delete player1;
	player1 = nullptr;

	delete Menu;
	Menu = nullptr;

	return 0;
	
}



