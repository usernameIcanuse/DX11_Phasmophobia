#pragma once
#include<iostream>
using namespace std;

#define		SAFE_RELEASE(p) {delete p; p = nullptr;}

typedef struct Info
{
	string		m_strName;
	int			m_Kor;
	int			m_Math;
	int			m_Eng;
	float		m_Avg;

	Info()
	{
		m_strName = "";
		m_Kor = 0;
		m_Math = 0;
		m_Eng = 0;
		m_Avg = 0;
	}
}Student;