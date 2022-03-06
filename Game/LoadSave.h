#pragma once
#include"Player.h"
#include"CObj.h"
//플레이어의 골드, 인벤토리 정보도 저장하기 위해 구조체로 선언할 필요 있음
class  CMemory
{
private:
	CObj*	m_pPlayer;//밖에서 받아오는 거라 Release하면 안됨

public:
	CMemory() :m_pPlayer(nullptr){}
	~CMemory() {}

public:
	void	Set_Player(CObj*	_pPlayer){m_pPlayer = _pPlayer;}
	bool	Loading();
	void	Saving();
};