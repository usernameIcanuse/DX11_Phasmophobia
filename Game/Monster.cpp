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
	cout << "����:	" << m_tInfo->m_szName << endl;;
	cout << "���ݷ�:	" << m_tInfo->m_iAttack << endl;
	cout << "����:	" << m_tInfo->m_iLevel << endl;
	cout << "ü��:	" << m_tInfo->m_iHP << "	/ " << m_tInfo->m_iHPMax << endl;
}