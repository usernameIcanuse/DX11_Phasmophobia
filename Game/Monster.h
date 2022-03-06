#pragma once
#include "CObj.h"

class CMonster : public CObj
{
private:
	//CObj*		m_tCObj;
	
public:
	CMonster() {  }
	virtual  ~CMonster() { }

public:
	virtual		void	Initialize(string _szName, int _iAttack, int _iHPMax, int _iLevel);
	virtual     void	Render(void)	const;

};
