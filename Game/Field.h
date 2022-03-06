#pragma once
#include"Monster.h"
#include"Player.h"

class CField
{
private:
	CObj*	m_pMonster;
	CObj*	m_pPlayer;

public:
	CField() :m_pMonster(nullptr){}
	~CField() { Release_Monster(); }

public:
	void	Set_Player(CObj* pPlayer) { m_pPlayer = pPlayer; }

public:
	void		Stage(void);
	int		Fighting(CObj* _pMonster);
	void		Release_Monster(void);


};