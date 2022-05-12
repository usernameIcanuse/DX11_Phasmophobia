
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "Device.h"
#include "TextureMgr.h"
#include "MainFrm.h"
#include "MyForm.h"
#include "Tab1.h"
#include "Tab3.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


HWND	g_hWnd;

// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)
//왜안됨
BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView()
	:iDrawID(0),
	iObjID(0),m_bPickObj(false),m_pPicked(nullptr),m_bObjDragged(false), m_bLButton(false),
	iPickObjIndex(-1),m_bCtrl(false),m_bDrag(false), m_bDragRange(false), m_iID(-1), m_bMouseRender(false)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolView::~CToolView()
{
	Release();
}

void CToolView::OnInitialUpdate()
{


	// SetScrollSizes : 스크롤 바의 사이즈를 지정하는 함수
	// MM_TEXT : 픽셀 단위로 조정하겠다는 옵션
	// 2인자 : 스크롤 가로, 세로 크기

	SetScrollSizes(MM_TEXT, CSize(TILECX * TILEX, TILEY * (TILECY / 2)));


	// AfxGetMainWnd : 현재의 메인 윈도우를 반환하는 전역 함수
	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();

	RECT	rcWnd{};

	// GetWindowRect : 현재 윈도우의 렉트 정보를 얻어와 기록해주는 함수
	pMainFrm->GetWindowRect(&rcWnd);

	// SetRect : 지정한 인자값으로 렉트의 정보를 기록해주는 함수
	// 현재 0,0 좌표 기준으로 다시 렉트를 조정하는 중
	SetRect(&rcWnd, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);

	RECT	rcMainView{};

	// GetClientRect : 현재 뷰창의 렉트 정보를 얻어오는 함수
	GetClientRect(&rcMainView);

	// 프레임의 가로, 세로 사이즈를 구한다
	float	fRowFrm = float(rcWnd.right - rcMainView.right);
	float	fColFrm = float(rcWnd.bottom - rcMainView.bottom);

	/*SetWindowPos : 인자값대로 새롭게 윈도우 위치와 크기를 조정하는 함수

	1. 배치할 윈도우의 Z순서에 대한 포인터(NULL : 순서 변경하지 않음)
	2,3. X,Y좌표
	4,5. 가로, 세로 크기
	6. 배치할 윈도우의 크기 및 위치 지정 옵션(SWP_NOZORDER : 현재 순서를 유지하겠음)*/

	pMainFrm->SetWindowPos(nullptr, 0, 0, int(WINCX+ 400 + fRowFrm), int(WINCY+400 + fColFrm), SWP_NOZORDER);
	
	CScrollView::OnInitialUpdate();

	g_hWnd = m_hWnd;

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (FAILED(CDevice::Get_Instance()->Initialize()))
		AfxMessageBox(L"Device Create Failed");

	/*if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Cube.png", TEX_SINGLE, L"Cube")))
	{
		AfxMessageBox(L"Cube Image Insert Failed");
		return;
	}*/

	m_pTerrain = new CMyTerrain;
	m_pTerrain->Initialize();
	m_pTerrain->SetMainView(this);

	m_pObjPick= new CObj;
	m_pObjPick->Initialize();
	m_pObjPick->SetMainView(this);



	
}

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	

	CDevice::Get_Instance()->Render_Begin();
	
	m_pTerrain->Render();

	m_pObjPick->Render();


	CMainFrame*		pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	if (!pMain)
		return;
	CMiniView*		pMiniView = dynamic_cast<CMiniView*>(pMain->m_SecondSplitter.GetPane(0, 0));
	CMyForm*	pForm = dynamic_cast<CMyForm*>(pMain->m_SecondSplitter.GetPane(1, 0));
	m_iID = pForm->pDlg3->m_iDrawID;
	

	if (m_iID != -1 &&m_bMouseRender)
	{
		D3DXMATRIX		matWorld , matTrans;
		D3DXMatrixIdentity(&matWorld);

	
		D3DXMatrixTranslation(&matTrans, m_vMouse.x,  // x는 0
			m_vMouse.y,  // y는 1
			0.f);
		

		matWorld = matTrans;


		RECT	rc{};

		// GetClientRect : 현재 클라이언트의 렉트를 얻어옴
		// ::함수 : 전역 함수 호출 구문, 일반적인 클래스 멤버함수와 구분하기 위해 표기하기도 한다.
		::GetClientRect(m_hWnd, &rc);

		float fCX = WINCX / float(rc.right - rc.left);
		float fCY = WINCY / float(rc.bottom - rc.top);

		matWorld._11 *= fCX;
		matWorld._21 *= fCX;
		matWorld._31 *= fCX;
		matWorld._41 *= fCX;

		matWorld._12 *= fCY;
		matWorld._22 *= fCY;
		matWorld._32 *= fCY;
		matWorld._42 *= fCY;

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		const TEXINFO*		pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Object", L"Object", m_iID);

		float		fX = pTexInfo->tImgInfo.Width / 2.f;
		float		fY = pTexInfo->tImgInfo.Height / 2.f;

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,
			&D3DXVECTOR3(fX, fY, 0.f),
			nullptr,
			D3DCOLOR_ARGB(150, 255, 255, 255));


	
	}



	CDevice::Get_Instance()->Render_End();
	if (m_bCtrl)
	{
		if (!m_vecPicked.empty())
		{
			CClientDC dc(this);
			CPen pen;
			pen.CreatePen(PS_SOLID, 5, RGB(255, 174, 201));    // 빨간색 펜 생성

			CPen* oldPen = dc.SelectObject(&pen);
		
			for (auto& iter : m_vecPicked)
			{
				float fX = iter->vPos.x - GetScrollPos(0);
				float fY = iter->vPos.y - GetScrollPos(1);
				

				dc.MoveTo(fX + TILECX*0.5f, fY);
				dc.LineTo(fX, fY + TILECY*0.5f);
				dc.LineTo(fX - TILECX*0.5f, fY);
				dc.LineTo(fX, fY - TILECY*0.5f);
				dc.LineTo(fX + TILECX*0.5f, fY);

			}
			dc.SelectObject(oldPen);
		}
		else if (m_bDragRange)
		{
			CClientDC dc(this);
			CPen pen;
			pen.CreatePen(PS_SOLID, 5, RGB(200, 174, 200));    // 빨간색 펜 생성

			CPen* oldPen = dc.SelectObject(&pen);


			dc.MoveTo(m_DragPoint1.x, m_DragPoint1.y);
			dc.LineTo(m_DragPoint1.x, m_DragPoint2.y);
			dc.LineTo(m_DragPoint2.x, m_DragPoint2.y);
			dc.LineTo(m_DragPoint2.x, m_DragPoint1.y);
			dc.LineTo(m_DragPoint1.x, m_DragPoint1.y);

			dc.SelectObject(oldPen);
		}
	}
	
	else if (m_pPicked)
	{
		CClientDC dc(this);

		float fX = m_pPicked->vPos.x - GetScrollPos(0);
		float fY = m_pPicked->vPos.y - GetScrollPos(1);
		CPen pen;
		pen.CreatePen(PS_SOLID, 5, RGB(255, 174, 201));    // 빨간색 펜 생성

		CPen* oldPen = dc.SelectObject(&pen);

		dc.MoveTo(fX + TILECX*0.5f, fY);
		dc.LineTo(fX, fY + TILECY*0.5f);
		dc.LineTo(fX - TILECX*0.5f, fY);
		dc.LineTo(fX, fY - TILECY*0.5f);
		dc.LineTo(fX + TILECX*0.5f, fY);

		dc.SelectObject(oldPen);
	}





}

void CToolView::OnDestroy()
{
	CScrollView::OnDestroy();

	Safe_Delete(m_pTerrain);
	Safe_Delete(m_pObjPick);

	CTextureMgr::Get_Instance()->Destroy_Instance();
	CDevice::Get_Instance()->Destroy_Instance();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}




#pragma region 불필요

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}
void CToolView::Release()
{
	Safe_Delete(m_pTerrain);
	Safe_Delete(m_pObjPick);
}
// CToolView 그리기



// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기


#pragma endregion 불필요






void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.


	CMainFrame*		pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	if (!pMain)
		return;
	CMiniView*		pMiniView = dynamic_cast<CMiniView*>(pMain->m_SecondSplitter.GetPane(0, 0));
	//1,0 창의 정보가져옴 
	CMyForm*	pForm						 = dynamic_cast<CMyForm*>(pMain->m_SecondSplitter.GetPane(1, 0));

	OBJECT*		pTemp = m_pObjPick->PickObject(D3DXVECTOR3(point.x + GetScrollPos(0), point.y + GetScrollPos(1), 0.f),iPickObjIndex);

	m_bLButton = true;

	if (!m_pPicked&& !pTemp && !m_bCtrl)
	{
		bool		bFlag = pForm->m_bTile;

		if (bFlag)
		{
			iDrawID = pForm->pDlg1->m_iDrawID;
			m_pTerrain->Tile_Change(D3DXVECTOR3(float(point.x + GetScrollPos(0)), float(point.y + GetScrollPos(1)), 0.f), iDrawID);

		}
		else if(m_bMouseRender)
		{
			iObjID = pForm->pDlg3->m_iDrawID;
			m_pObjPick->Object_Pick(D3DXVECTOR3(float(point.x + GetScrollPos(0)), float(point.y + GetScrollPos(1)), 0.f), iObjID);

		}
	}

	if(m_bCtrl)
		m_DragPoint1 = point;

	Invalidate(FALSE);

	pMiniView->Invalidate(FALSE);
	CScrollView::OnLButtonDown(nFlags, point);

}

void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnMouseMove(nFlags, point);
	
	CMainFrame*		pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	if (!pMain)
		return;
	CMiniView*		pMiniView = dynamic_cast<CMiniView*>(pMain->m_SecondSplitter.GetPane(0, 0));
	//1,0 창의 정보가져옴 
	CMyForm*	pForm = dynamic_cast<CMyForm*>(pMain->m_SecondSplitter.GetPane(1, 0));
	if (GetAsyncKeyState(VK_LBUTTON))
	{
		if (m_bLButton)
		{
			

			if (m_bCtrl)
			{
				if (!m_vecPicked.empty())
				{
					CPoint	movePos = point - m_DragPoint1;

					for (auto& iter : m_vecPicked)
					{
						
						int iIndex = m_pObjPick->GetObjectPos(D3DXVECTOR3(iter->vPos.x + movePos.x , iter->vPos.y + movePos.y , 0.f));

						if (iIndex == -1)
							return;

						D3DXVECTOR3 Temp = *m_pObjPick->m_vecPos[iIndex];
						if (Temp.x != iter->vPos.x || Temp.y != iter->vPos.y)
						{
							iter->vPos = Temp;
							m_DragPoint1 = point;
						}
					}
					
					m_bDrag = true;
				}
				else
				{
					m_bDragRange = true;
					m_DragPoint2 = point;
					m_bDrag = true;

				}
			}

			else if (m_pPicked)
			{

				int iIndex = m_pObjPick->GetObjectPos(D3DXVECTOR3(point.x + GetScrollPos(0), point.y + GetScrollPos(1), 0.f));
				m_pPicked->vPos = *m_pObjPick->m_vecPos[iIndex];

				m_bObjDragged = true;

			}
			
			else
			{
				bool		bFlag = pForm->m_bTile;
				if (bFlag)
				{
					m_pTerrain->Tile_Change(D3DXVECTOR3(float(point.x + GetScrollPos(0)), float(point.y + GetScrollPos(1)), 0.f), iDrawID);
				}
			}
			
		}
	}

	else
	{
		m_vMouse.x = point.x ;
		m_vMouse.y = point.y;
	}

	Invalidate(FALSE);

	pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	pMiniView = dynamic_cast<CMiniView*>(pMain->m_SecondSplitter.GetPane(0, 0));
	pMiniView->Invalidate(FALSE);
}


void CToolView::OnLButtonUp(UINT nFlags, CPoint point)
{
	
	m_bLButton = false;
	if (!m_bCtrl)
	{
		if (m_bObjDragged)
		{
			m_bObjDragged = false;
			m_bPickObj = true;
		}
		else
		{
			OBJECT*		pTemp = m_pObjPick->PickObject(D3DXVECTOR3(point.x + GetScrollPos(0), point.y + GetScrollPos(1), 0.f), iPickObjIndex);

			if (pTemp)
			{
				if (m_pPicked != pTemp)
				{
					m_pPicked = pTemp;
					m_bPickObj = true;
				}
				else
				{

					m_pPicked = nullptr;
					m_bObjDragged = false;
					m_bPickObj = false;
					iPickObjIndex = -1;

				}

			}
			else
			{
				m_pPicked = nullptr;
				m_bPickObj = false;
				m_bObjDragged = false;
				iPickObjIndex = -1;
			}

		}
	}
	else if (!m_bDrag)
	{
			OBJECT*		pTemp = m_pObjPick->PickObject(D3DXVECTOR3(point.x + GetScrollPos(0), point.y + GetScrollPos(1), 0.f), iPickObjIndex);
			if (pTemp)
			{
				if (m_vecPicked.empty())
				{
					m_vecPicked.push_back(pTemp);
				}
				else
				{
					auto	iter = find(m_vecPicked.begin(), m_vecPicked.end(), pTemp);

					if (iter == m_vecPicked.end())
					{
						m_vecPicked.push_back(pTemp);
					}
				}
			}
		
	}
	else
	{
		m_bDrag = false;
		if (m_bDragRange)
		{
			for (auto& iter : m_pObjPick->m_vecObj)
			{
				D3DXVECTOR3 Temp = iter->vPos;

				float fLeft, fRight, fTop, fBottom;

				if (m_DragPoint1.x < m_DragPoint2.x)
				{
					fLeft = m_DragPoint1.x;
					fRight = m_DragPoint2.x;
				}
				else
				{
					fLeft = m_DragPoint2.x;
					fRight = m_DragPoint1.x;
				}

				if (m_DragPoint1.y < m_DragPoint2.y)
				{
					fTop = m_DragPoint1.y;
					fBottom = m_DragPoint2.y;
				}
				else
				{
					fTop = m_DragPoint2.y;
					fBottom = m_DragPoint1.y;
				}

				if (Temp.x >= fLeft + GetScrollPos(0) && Temp.x <= fRight+ GetScrollPos(0) && Temp.y >= fTop+ GetScrollPos(1) && Temp.y <= fBottom + GetScrollPos(1))
				{
					m_vecPicked.push_back(iter);
				}
			}
			m_bDragRange = false;
			m_DragPoint1.x = m_DragPoint1.y = m_DragPoint2.x = m_DragPoint2.y = 0.f;
		}
	}


	Invalidate(FALSE);
	CScrollView::OnLButtonUp(nFlags, point);
}


void CToolView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (GetAsyncKeyState(VK_CONTROL))
	{
		m_bCtrl = true;
	}

	if (GetAsyncKeyState(VK_ESCAPE))
		m_bMouseRender = false;
	
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CToolView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_bCtrl)
	{
		m_bCtrl = false;
		m_vecPicked.clear();
	}
	Invalidate(FALSE);
	CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}
