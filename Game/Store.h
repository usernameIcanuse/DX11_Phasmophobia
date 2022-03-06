#pragma once
#include"Player.h"

enum
{
	Weapon = 1,
	Shield
};

enum
{
	Easy = 1,
	Normal,
	Hard
};

class CStore
{
private:
	CObj*		m_pPlayer;//여기서 해제하면 안됨
	Item*		m_pItem[3][2];

public:
	CStore()
	{
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 2; ++j)
				m_pItem[i][j] = nullptr;
	}
	~CStore() { Release(); }

public:
	void		Set_Player(CObj*	_pPlayer) { m_pPlayer = _pPlayer; }
	void		Initialize();

public:
	void		Update();
	void		Store(void);
	
	void		Strength(void);
	void		Buy_Item(Item&	Temp);
	void		Sell_Item();
	void		Render(int _eLevel);

	void		Release();
};