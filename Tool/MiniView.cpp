// MiniView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MiniView.h"
#include "Device.h"
#include "MainFrm.h"


// CMiniView

IMPLEMENT_DYNCREATE(CMiniView, CView)

CMiniView::CMiniView()
{

}

CMiniView::~CMiniView()
{
}

BEGIN_MESSAGE_MAP(CMiniView, CView)
END_MESSAGE_MAP()


// CMiniView �׸����Դϴ�.

void CMiniView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.

	// AfxGetMainWnd : ���� ������κ��� wnd�� ��ȯ -> ���� �����尡 ���� �������� ��� MainFrameWnd�� ��ȯ
	//CMainFrame*		pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

	// GetParentFrame : ���� view�� �ѷ��ΰ� �ִ� ���� FrameWnd�� ��ȯ
	//CMainFrame*		pMain = dynamic_cast<CMainFrame*>(GetParentFrame());
		
	// AfxGetApp : ���� �����带 ���� �ִ� ���� ���� app�� ��ȯ
	CMainFrame*		pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	
	//GetPane(��, ��) : �ش� ���� ������ ������ �Լ�
	CToolView*		pToolView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	CMyTerrain*		pMyTerrain = pToolView->m_pTerrain;
	CObj*			pObj = pToolView->m_pObjPick;

	CDevice::Get_Instance()->Render_Begin();

	pMyTerrain->Mini_Render();
	pObj->Mini_Render();

	CDevice::Get_Instance()->Render_End(m_hWnd);
}


// CMiniView �����Դϴ�.

#ifdef _DEBUG
void CMiniView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiniView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMiniView �޽��� ó�����Դϴ�.
