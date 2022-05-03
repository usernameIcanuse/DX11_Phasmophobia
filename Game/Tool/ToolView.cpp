
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


HWND	g_hWnd;

// CToolView

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolView::~CToolView()
{
}

void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	g_hWnd = m_hWnd;

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (FAILED(CDevice::Get_Instance()->Initialize()))
		AfxMessageBox(L"Device Create Failed");

	/*m_pSingle = new CSingleTexture;
	m_pSingle->InsertTexture(L"../Texture/Cube.png");*/

	if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Cube.png", TEX_SINGLE, L"Cube")))
	{
		AfxMessageBox(L"Cube Image Insert Failed");
		return;
	}

	if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Stage/Terrain/Tile/Tile%d.png", TEX_MULTI, L"Terrain", L"Tile", 36)))
	{
		AfxMessageBox(L"Tile Image Insert Failed");
		return;
	}
}

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	
	D3DXMATRIX	matWorld, matScale, matRotZ, matTrans;

	D3DXMatrixIdentity(&matWorld);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(45.f));
	D3DXMatrixTranslation(&matTrans, 400.f, 300.f, 0.f);

	matWorld = matScale *  matTrans;

	CDevice::Get_Instance()->Render_Begin();

	// SetTransform : 스프라이크 컴객체와 행렬을 곱셈해주는 함수
	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	const TEXINFO*		pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", 2);

	float		fX = pTexInfo->tImgInfo.Width / 2.f;
	float		fY = pTexInfo->tImgInfo.Height / 2.f;

	/*float		fX = m_pSingle->Get_Texture()->tImgInfo.Width / 2.f;
	float		fY = m_pSingle->Get_Texture()->tImgInfo.Height / 2.f;*/
		
	// Draw : 스크린 공간에 2D 스프라이트 이미지를 출력
	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, // 텍스처 객체
												nullptr,	// 출력할 이미지 영역에 대한 렉트 구조체 포인터, null인 경우 이미지의 0, 0기준으로 출력
												&D3DXVECTOR3(fX, fY, 0.f),	// 출력할 이미지의 중심 축에 대한 vec3 구조체 포인터, null인 경우 0, 0이 중심 좌표
												nullptr,	// 위치 좌표에 대한 vec3 구조체 포인터, null인 경우 스크린 상 0,0,에 좌표 출력
												D3DCOLOR_ARGB(255, 255, 255, 255));	// 출력할 원본 이미지와 섞을 색상 값, 0xffffffff 값을 넣어주면 원본색 유지




	CDevice::Get_Instance()->Render_End();

}

void CToolView::OnDestroy()
{
	CView::OnDestroy();

	//Safe_Delete(m_pSingle);

	CTextureMgr::Get_Instance()->Destroy_Instance();
	CDevice::Get_Instance()->Destroy_Instance();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


#pragma region 불필요

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
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
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
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

	CView::OnLButtonDown(nFlags, point);

	// 직선의 방정식 y = a(기울기)x + b(y절편) 
}
