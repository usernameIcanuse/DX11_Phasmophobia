#include"CObj.h"

void	CObj::Level_Up(void)
{
	 ++m_tInfo->m_iLevel;
	 m_tInfo->m_iAttack += 20;
	 m_tInfo->m_iHP = m_tInfo->m_iHPMax += 50;
}