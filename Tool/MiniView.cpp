// MiniView.cpp : 구현 파일입니다.
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


// CMiniView 그리기입니다.

void CMiniView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.

	// AfxGetMainWnd : 현재 쓰레드로부터 wnd를 반환 -> 현재 쓰레드가 메인 쓰레드일 경우 MainFrameWnd를 반환
	//CMainFrame*		pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

	// GetParentFrame : 현재 view를 둘러싸고 있는 상위 FrameWnd를 반환
	//CMainFrame*		pMain = dynamic_cast<CMainFrame*>(GetParentFrame());
		
	// AfxGetApp : 메인 스레드를 갖고 있는 현재 메인 app을 반환
	CMainFrame*		pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	
	//GetPane(열, 행) : 해당 셀의 정보를 얻어오는 함수
	CToolView*		pToolView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	CMyTerrain*		pMyTerrain = pToolView->m_pTerrain;
	CObj*			pObj = pToolView->m_pObjPick;

	CDevice::Get_Instance()->Render_Begin();

	pMyTerrain->Mini_Render();
	pObj->Mini_Render();

	CDevice::Get_Instance()->Render_End(m_hWnd);
}


// CMiniView 진단입니다.

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


// CMiniView 메시지 처리기입니다.
