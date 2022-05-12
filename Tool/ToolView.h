
// ToolView.h : CToolView Ŭ������ �������̽�
//

#pragma once

#include "MyTerrain.h"
#include "Obj.h"

class CToolDoc;
class CToolView : public CScrollView
{
protected: // serialization������ ��������ϴ�.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// Ư���Դϴ�.
public:
	CToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:
	int	iDrawID;
	int	iObjID;

	int iPickObjIndex;

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void	Release();
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);


// �����Դϴ�.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

public:
	int						m_iID;
	bool					m_bMouseRender;

	CMyTerrain*				m_pTerrain;
	CObj*					m_pObjPick;
	OBJECT					m_pObjRender;

	bool					m_bPickObj;
	bool					m_bObjDragged;
	bool					m_bLButton;
	OBJECT*					m_pPicked;

	vector<OBJECT*>			m_vecPicked;
	bool					m_bCtrl;
	bool					m_bDrag;

	bool					m_bDragRange;

	CPoint					m_DragPoint1;
	CPoint					m_DragPoint2;
	
	D3DXVECTOR3				m_vMouse;

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // ToolView.cpp�� ����� ����
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

// 1. ��Ƽ �ؽ�ó�� �̿��Ͽ� Ÿ�� ������ ���ͺ� �������� ��ƿ���
// 2. ���콺 ��ŷ�� �̿��Ͽ� ���콺 Ŭ������ ���ϴ� Ÿ�� �̹����� �ٲ����
