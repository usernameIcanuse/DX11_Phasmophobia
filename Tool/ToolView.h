
// ToolView.h : CToolView 클래스의 인터페이스
//

#pragma once

#include "MyTerrain.h"
#include "Obj.h"

class CToolDoc;
class CToolView : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// 특성입니다.
public:
	CToolDoc* GetDocument() const;

// 작업입니다.
public:
	int	iDrawID;
	int	iObjID;

	int iPickObjIndex;

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void	Release();
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);


// 구현입니다.
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

// 생성된 메시지 맵 함수
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

#ifndef _DEBUG  // ToolView.cpp의 디버그 버전
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

// 1. 멀티 텍스처를 이용하여 타일 지형을 쿼터뷰 형식으로 깔아오기
// 2. 마우스 피킹을 이용하여 마우스 클릭으로 원하는 타일 이미지로 바꿔오기
