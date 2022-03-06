#pragma once
#include<iostream>
#include<string>
using namespace std;

#define SAFE_RELEASE(p) { delete p; p = nullptr;}


typedef struct  Info
{
	string		m_szName;
	int			m_iAttack;
	int			m_iHP;
	int			m_iHPMax;
	int			m_iLevel;
	Info()
	{
		m_szName = "";
		m_iAttack = 0;
		m_iHP = 0;
		m_iHPMax = 0;
		m_iLevel = 0;
	}
}Info;


class CObj
{
protected:
//protected�� �����ϸ� �ڽ� Ŭ������ ���ٸ� �����ϰ� �������� �Ұ���->���� �Լ� �����ؼ� ������ �׷��� ��� ���� �̿�
	Info*	m_tInfo;

public:
	CObj():m_tInfo(new Info)
	{}
	virtual ~CObj(){ }

public:

	int			Get_Attack(void) { return m_tInfo->m_iAttack; }
	int		    Get_HP(void) { return m_tInfo->m_iHP; }
	void		Set_Damaged(int _iAttack) { m_tInfo->m_iHP -= _iAttack; }
	void		Level_Up(void);
	void		Increase_Attack(int	_iAttack) { m_tInfo->m_iAttack += _iAttack; }
	void		Reset_HP(void){ m_tInfo->m_iHP = m_tInfo->m_iHPMax;}

	virtual		void	Initialize(string _szName, int _iAttack, int _iHPMax, int _iLevel) = 0;
	virtual		void	Render(void) const = 0;
	

};



class Item
{
private:
	string		m_szName;
	int			m_iPower;
	int			m_iShield;
	int			m_iPrice;
	int			m_iNum;

public:
	Item()
	{
		m_szName = "";
		m_iPower = 0;
		m_iShield = 0;
		m_iPrice = 0;
		m_iNum = 0;
	}

	Item(string _szName, int _iPower, int _iShield, int _iPrice,int _iNum)
		:m_szName(_szName), m_iPower(_iPower), m_iPrice(_iPrice),
		m_iShield(_iShield), m_iNum(_iNum){}

	Item(Item&	item)
	{
		m_szName = item.m_szName;
		m_iPower = item.m_iPower;
		m_iShield = item.m_iShield;
		m_iPrice = item.m_iPrice;
		m_iNum = item.m_iNum;
	}

public:
	void Initialize(string _szName, int _iPower, int _iPrice, int _iShield,int _iNum)
	{
		m_szName = _szName;
		m_iPower = _iPower;
		m_iPrice = _iPrice;
		m_iShield = _iShield;
		m_iNum = _iNum;
	}

	void Render()
	{
		cout << "�̸� : " << m_szName << endl;
		cout << "���ݷ� ������ : " << m_iPower << endl;
		cout << "ü�� ������ : " << m_iShield << endl;
		cout << "���� : " << m_iPrice << endl;
	}

	int	Get_Money() { return m_iPrice; }
	int Get_Attack() { return m_iPower; }
	int Get_Shield() { return m_iShield; }
	int Get_Num() { return m_iNum; }
	string	Get_Name() { return m_szName; }
};
