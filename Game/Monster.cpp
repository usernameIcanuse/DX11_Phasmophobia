#pragma once
#include"Monster.h"


void	CMonster::Initialize(string _szName, int _iAttack, int _iHPMax, int _iLevel)
{
	m_tInfo->m_szName = _szName;
	m_tInfo->m_iAttack = _iAttack;
	m_tInfo->m_iHP = m_tInfo->m_iHPMax = _iHPMax;
	m_tInfo->m_iLevel = _iLevel;
}


void	CMonster::Render(void)		const
{
	cout << "직업:	" << m_tInfo->m_szName << endl;;
	cout << "공격력:	" << m_tInfo->m_iAttack << endl;
	cout << "레벨:	" << m_tInfo->m_iLevel << endl;
	cout << "체력:	" << m_tInfo->m_iHP << "	/ " << m_tInfo->m_iHPMax << endl;
}