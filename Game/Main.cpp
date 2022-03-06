#include"MainGame.h"

int main()
{
	CMainGame* _pMainGame = new CMainGame;

	if(_pMainGame->MainMenu())
		_pMainGame->Village();

	SAFE_RELEASE(_pMainGame);

	return 0;
}