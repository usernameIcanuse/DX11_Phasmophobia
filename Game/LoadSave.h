#pragma once
#include"Player.h"
#include"CObj.h"
//�÷��̾��� ���, �κ��丮 ������ �����ϱ� ���� ����ü�� ������ �ʿ� ����
class  CMemory
{
private:
	CObj*	m_pPlayer;//�ۿ��� �޾ƿ��� �Ŷ� Release�ϸ� �ȵ�

public:
	CMemory() :m_pPlayer(nullptr){}
	~CMemory() {}

public:
	void	Set_Player(CObj*	_pPlayer){m_pPlayer = _pPlayer;}
	bool	Loading();
	void	Saving();
};