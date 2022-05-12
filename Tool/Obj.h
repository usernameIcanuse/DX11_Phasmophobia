#pragma once

#include "Include.h"

class CToolView;
class CObj
{
public:
	CObj();
	~CObj();

public:
	void		Object_Pick(const D3DXVECTOR3& vPos, const BYTE& byDrawID);
	int			GetObjectPos(const D3DXVECTOR3& vMouse);
	bool		PickingDot(const D3DXVECTOR3& vPos, const int& iIndex);
	void		SetMainView(CToolView* pView) { m_pMainView = pView; }
	const vector<OBJECT*>& Get_ObjVector() const { return m_vecObj; }
	void		Relese_Object() {
		for_each(m_vecObj.begin(), m_vecObj.end(), CDeleteObj());
		m_vecObj.clear();
	}

	void	Set_Vector(vector<OBJECT*> vecTile) { m_vecObj = vecTile; }

	OBJECT*	PickObject(const D3DXVECTOR3& vMouse, int&	 Index);
	int		GetObjectPick(const D3DXVECTOR3& vMouse);
	bool	PickingObjDot(const D3DXVECTOR3& vPos, const int& iIndex);

	void	Delete_Object(int iIndex);

public:
	void		Initialize(void);
	void		Update(void);
	void		Render(void);
	void		Mini_Render(void);
	void		Release(void);


private:
	void		Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY);
	void        Set_Size(D3DXMATRIX* pOut, int _iSize);

public:
	vector<OBJECT*>		m_vecObj;

	vector<D3DXVECTOR3*>	m_vecPos;
	CToolView*			m_pMainView = nullptr;
	D3DXMATRIX			matSize;
};
