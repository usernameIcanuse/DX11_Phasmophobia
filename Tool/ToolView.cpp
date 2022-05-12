
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
//�־ȵ�
BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// ǥ�� �μ� ����Դϴ�.
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

// CToolView ����/�Ҹ�

CToolView::CToolView()
	:iDrawID(0),
	iObjID(0),m_bPickObj(false),m_pPicked(nullptr),m_bObjDragged(false), m_bLButton(false),
	iPickObjIndex(-1),m_bCtrl(false),m_bDrag(false), m_bDragRange(false), m_iID(-1), m_bMouseRender(false)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CToolView::~CToolView()
{
	Release();
}

void CToolView::OnInitialUpdate()
{


	// SetScrollSizes : ��ũ�� ���� ����� �����ϴ� �Լ�
	// MM_TEXT : �ȼ� ������ �����ϰڴٴ� �ɼ�
	// 2���� : ��ũ�� ����, ���� ũ��

	SetScrollSizes(MM_TEXT, CSize(TILECX * TILEX, TILEY * (TILECY / 2)));


	// AfxGetMainWnd : ������ ���� �����츦 ��ȯ�ϴ� ���� �Լ�
	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();

	RECT	rcWnd{};

	// GetWindowRect : ���� �������� ��Ʈ ������ ���� ������ִ� �Լ�
	pMainFrm->GetWindowRect(&rcWnd);

	// SetRect : ������ ���ڰ����� ��Ʈ�� ������ ������ִ� �Լ�
	// ���� 0,0 ��ǥ �������� �ٽ� ��Ʈ�� �����ϴ� ��
	SetRect(&rcWnd, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);

	RECT	rcMainView{};

	// GetClientRect : ���� ��â�� ��Ʈ ������ ������ �Լ�
	GetClientRect(&rcMainView);

	// �������� ����, ���� ����� ���Ѵ�
	float	fRowFrm = float(rcWnd.right - rcMainView.right);
	float	fColFrm = float(rcWnd.bottom - rcMainView.bottom);

	/*SetWindowPos : ���ڰ���� ���Ӱ� ������ ��ġ�� ũ�⸦ �����ϴ� �Լ�

	1. ��ġ�� �������� Z������ ���� ������(NULL : ���� �������� ����)
	2,3. X,Y��ǥ
	4,5. ����, ���� ũ��
	6. ��ġ�� �������� ũ�� �� ��ġ ���� �ɼ�(SWP_NOZORDER : ���� ������ �����ϰ���)*/

	pMainFrm->SetWindowPos(nullptr, 0, 0, int(WINCX+ 400 + fRowFrm), int(WINCY+400 + fColFrm), SWP_NOZORDER);
	
	CScrollView::OnInitialUpdate();

	g_hWnd = m_hWnd;

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

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

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	

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

	
		D3DXMatrixTranslation(&matTrans, m_vMouse.x,  // x�� 0
			m_vMouse.y,  // y�� 1
			0.f);
		

		matWorld = matTrans;


		RECT	rc{};

		// GetClientRect : ���� Ŭ���̾�Ʈ�� ��Ʈ�� ����
		// ::�Լ� : ���� �Լ� ȣ�� ����, �Ϲ����� Ŭ���� ����Լ��� �����ϱ� ���� ǥ���ϱ⵵ �Ѵ�.
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
			pen.CreatePen(PS_SOLID, 5, RGB(255, 174, 201));    // ������ �� ����

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
			pen.CreatePen(PS_SOLID, 5, RGB(200, 174, 200));    // ������ �� ����

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
		pen.CreatePen(PS_SOLID, 5, RGB(255, 174, 201));    // ������ �� ����

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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}




#pragma region ���ʿ�

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}
void CToolView::Release()
{
	Safe_Delete(m_pTerrain);
	Safe_Delete(m_pObjPick);
}
// CToolView �׸���



// CToolView �μ�

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����


#pragma endregion ���ʿ�






void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.


	CMainFrame*		pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	if (!pMain)
		return;
	CMiniView*		pMiniView = dynamic_cast<CMiniView*>(pMain->m_SecondSplitter.GetPane(0, 0));
	//1,0 â�� ���������� 
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnMouseMove(nFlags, point);
	
	CMainFrame*		pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	if (!pMain)
		return;
	CMiniView*		pMiniView = dynamic_cast<CMiniView*>(pMain->m_SecondSplitter.GetPane(0, 0));
	//1,0 â�� ���������� 
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
