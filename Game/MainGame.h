#pragma once
#include"Field.h"
#include"Player.h"
#include"Store.h"
#include"LoadSave.h"

class CMainGame
{
private:
	CObj*	m_pPlayer;
	CField*		m_pField;
	CStore*		m_pStore;
	CMemory*	 m_pLoadSave;	
public:
	CMainGame() :  m_pField(nullptr), m_pPlayer(nullptr), m_pStore(nullptr),m_pLoadSave(nullptr)
				{}
	~CMainGame() { Release(); }
public:
	
	void		Initialize(void);
	bool		MainMenu(void);
	void		Village(void);
	void		My_Profile(void);
	void		Release(void);

public:
	bool	Loading(void);
	void	New_character(void);
};