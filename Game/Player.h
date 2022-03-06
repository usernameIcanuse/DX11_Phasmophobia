#pragma once
#include "CObj.h"
#include<vector>

class CPlayer : public CObj
{
private:
//	CObj*		m_tCObj;
	int			m_iExp;
	int			m_iExpMax;
	int			m_iMoney;
	vector<Item*>		m_vInventory;
	vector<Item*>		m_vCap;

public:
	CPlayer() : m_iExp(0), m_iExpMax(0), m_iMoney(1000)
	{
		m_vInventory.reserve(5);//5���� max �̸� �Ҵ��� ���´�.
		m_vCap.reserve(2);
	}
	virtual	~CPlayer() 
	{
		Release_Inventory();
	};
	
public:
	//����
	void		Select_Job(void);
	void		Set_Zero(void);
	void		Get_Exp(int _iExp);

public:
	virtual		void		Render(void)	const;
	virtual		void		Initialize(string _szName, int _iAttack, int _iHPMax, int _iLevel);

public:
	//�κ��丮
	void		Render_Inventory(void);
	int			Get_Money(void) { return m_iMoney; }
	void		Money_Minus(int _iMoney) { m_iMoney -= _iMoney; }
	void		Money_Plus(int _iMoney) { m_iMoney += _iMoney; }
	bool		Is_Full_Inven(void);
	void		Get_Item(Item&	_item);
	void		Delete_Item();//�˻� ���� �̿�
	
	void		Release_Inventory();
	void		Sell_Item(int _iIndex);

public:
	void		Render_Cap();//���� ������
	void		Set_Item();//������ ����,����

	void		Puton_Item();
	void		Putoff_Item();


public:
	//����
	void		Loading(FILE*	fp);
	void		Saving(FILE*	fp);
};