// MyForm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"
#include "TextureMgr.h"
#include "Device.h"

#include "Tab1.h"
#include "Tab3.h"

#include "MainFrm.h"
#include "ToolView.h"


// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM),m_bTile(true)
{

}

CMyForm::~CMyForm()
{
	Release();
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TAB1, m_Tab);
}

void CMyForm::Release()
{
	Safe_Delete(pDlg1);
	Safe_Delete(pDlg3);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)

	ON_WM_DROPFILES()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMyForm::OnTcnSelchangeTab1)
	
	ON_BN_CLICKED(IDC_BUTTON1, &CMyForm::OnSaveData)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyForm::OnLoadData)

END_MESSAGE_MAP()


// CMyForm �����Դϴ�.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm �޽��� ó�����Դϴ�.


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	//CreatePointFont(��Ʈ ũ��, �۾�ü)
	//m_Font.CreatePointFont(180, L"�ü�");

	// GetDlgItem : ���̾�α׿� ��ġ�� ���ҽ��� ID���� ���� ������ �Լ�
	


	m_Tab.InsertItem(0, _T("Tile"));
	m_Tab.InsertItem(1, _T("Object"));


	m_Tab.SetCurSel(0);

	CRect rect;
	m_Tab.GetWindowRect(&rect);

	pDlg1 = new CTab1;
	pDlg1->Create(IDD_DIALOG1, &m_Tab);
	pDlg1->MoveWindow(0, 25, rect.Width(), rect.Height());
	pDlg1->ShowWindow(SW_SHOW);


	pDlg3 = new CTab3;
	pDlg3->Create(IDD_DIALOG3, &m_Tab);
	pDlg3->MoveWindow(0, 25, rect.Width(), rect.Height());
	pDlg3->ShowWindow(SW_HIDE);

}






void CMyForm::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int sel = m_Tab.GetCurSel();

	switch (sel)
	{
	case 0:
		pDlg1->ShowWindow(SW_SHOW);
		pDlg3->ShowWindow(SW_HIDE);
		m_bTile = true;
		break;
	case 1:
		pDlg1->ShowWindow(SW_HIDE);
		pDlg3->ShowWindow(SW_SHOW);
		m_bTile = false;
		break;


	}
	*pResult = 0;
}



void CMyForm::OnSaveData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(FALSE, L"dat", L"*.dat");

	TCHAR szFilePath[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	lstrcat(szFilePath, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	if (IDOK == Dlg.DoModal())
	{
		CString wstrFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwByte = 0;
		CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CToolView* pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
		vector<TILE*> vecTile = pView->m_pTerrain->Get_TileVector();
		vector<OBJECT*> vecObject = pView->m_pObjPick->Get_ObjVector();

		int		TileSize = pView->m_pTerrain->Get_TileVector().size();
		int		ObjSize = pView->m_pObjPick->Get_ObjVector().size();


	
		WriteFile(hFile, &TileSize, sizeof(int), &dwByte, nullptr);
		

		for (auto& pTile : vecTile)
			WriteFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);


		WriteFile(hFile, &ObjSize, sizeof(int), &dwByte, nullptr);

		for (auto& pObj : vecObject)
			WriteFile(hFile, pObj, sizeof(OBJECT), &dwByte, nullptr);

		CloseHandle(hFile);
	}
}


void CMyForm::OnLoadData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(TRUE, L"dat", L"*.dat");

	TCHAR szFilePath[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	lstrcat(szFilePath, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	if (IDOK == Dlg.DoModal())
	{
		CString wstrFilePath = Dlg.GetPathName();
		
		HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
			return;


		CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CToolView* pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));



		pView->m_pTerrain->Relese_Terrain();
	    pView->m_pObjPick->Relese_Object();


		DWORD dwByte = 0;
		TILE* pTile = nullptr;
		OBJECT* pObj = nullptr;
	

		int		TileSize = 0;
		ReadFile(hFile, &TileSize, sizeof(int), &dwByte, nullptr);


		vector<TILE*> vecTile;
		vector<OBJECT*> vecObj;
		vecTile.reserve(TILEX * TILEY);
		vecObj.reserve(TILEX * TILEY);
		for(int i=0;i<TileSize;++i)
		{
			pTile = new TILE;
			ReadFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);
			if (0 == dwByte)
			{
				SAFE_DELETE(pTile);
				break;
			}
			vecTile.emplace_back(pTile);
		}
		pView->m_pTerrain->Set_Vector(vecTile);

		int		ObjSize = 0;
		ReadFile(hFile, &ObjSize, sizeof(int), &dwByte, nullptr);


		for(int i=0;i<ObjSize;++i)
		{
			pObj = new OBJECT;
			ReadFile(hFile, pObj, sizeof(OBJECT), &dwByte, nullptr);
			if (0 == dwByte)
			{
				SAFE_DELETE(pObj);
				break;
			}
			vecObj.emplace_back(pObj);
		}


		
		pView->m_pObjPick->Set_Vector(vecObj);



		pView->Invalidate(FALSE);

		CloseHandle(hFile);
	}
}

// ���� ������ �����ϰ� ���� ����
// �ҷ����� �Ҷ��� ������ �ҷ����� �����ŭ�� �ҷ�����



