// Tab3.cpp : implementation file
//

#include "stdafx.h"
#include "Tool.h"
#include "Tab3.h"
#include "afxdialogex.h"
#include "TextureMgr.h"
#include "Device.h"
#include "MainFrm.h"
#include "ToolView.h"

// CTab3 dialog

IMPLEMENT_DYNAMIC(CTab3, CDialogEx)

CTab3::CTab3(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG3, pParent) , m_iObjSize(0), m_iObjSize2(0), m_iDrawID(-1)
{

}

CTab3::~CTab3()
{
}

void CTab3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE1, m_Picture);
	DDX_Control(pDX, IDC_SLIDER1, m_sld);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	DDX_Control(pDX, IDC_SLIDER2, m_sld2);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
	DDX_Control(pDX, IDC_SLIDER3, m_sld3);
	DDX_Control(pDX, IDC_EDIT3, m_edit3);
}


BEGIN_MESSAGE_MAP(CTab3, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST1, &CTab3::OnLbnSelchangeList1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CTab3::OnNMCustomdrawSlider1)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT1, &CTab3::OnEnChangeEdit1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CTab3::OnNMCustomdrawSlider2)
	ON_EN_CHANGE(IDC_EDIT2, &CTab3::OnEnChangeEdit2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER3, &CTab3::OnNMCustomdrawSlider3)
	ON_EN_CHANGE(IDC_EDIT3, &CTab3::OnEnChangeEdit3)
	ON_BN_CLICKED(IDC_BUTTON1, &CTab3::OnDeletePickedObj)

END_MESSAGE_MAP()


// CTab3 message handlers


BOOL CTab3::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	//�����̴�
	m_sld.SetRange(5, 20);
	m_sld.SetPos(10);
	m_sld.SetLineSize(15);
	m_sld.SetPageSize(30);

	//edit box
	int      iPos = m_sld.GetPos();
	CString sPos;
	sPos.Format(_T("%d"), iPos);

	m_edit.SetWindowTextW(sPos);

	m_sld2.SetRange(5, 20);
	m_sld2.SetPos(10);
	m_sld2.SetLineSize(15);
	m_sld2.SetPageSize(30);

	//edit box
	int      iPos2 = m_sld2.GetPos();
	CString sPos2;
	sPos2.Format(_T("%d"), iPos2);

	m_edit2.SetWindowTextW(sPos2);

	m_sld3.SetRange(0, 360);
	m_sld3.SetPos(0);
	m_sld3.SetLineSize(15);
	m_sld3.SetPageSize(30);

	//edit box
	int      iPos3 = m_sld3.GetPos();
	CString sPos3;
	sPos3.Format(_T("%d"), iPos3);

	m_edit3.SetWindowTextW(sPos3);

	m_ListBox.Dir(DDL_ARCHIVE, L"..\\Texture\\Stage\\Object\\*.png");
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CTab3::OnLbnSelchangeList1()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	pView->m_bMouseRender = true;


	int iIndex = m_ListBox.GetCurSel();
	CString strFindName;

	m_ListBox.GetText(iIndex, strFindName);

	int i = 0;

	for (; i < strFindName.GetLength(); ++i)
	{
		if (isdigit(strFindName[i]))
			break;
	}

	strFindName.Delete(0, i);

	m_iDrawID = _ttoi(strFindName.GetString());

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Object", L"Object", m_iDrawID);

	if (nullptr == pTexInfo)
		return;

	
	D3DXMATRIX  matScale, matTrans, matWorld;

	float fRatioX = WINCX / OBJCX;
	float fRatioY = WINCY / OBJCY;

	D3DXMatrixScaling(&matScale, fRatioX, fRatioY, 0.f);
	D3DXMatrixTranslation(&matTrans, 400.f, 300.f, 0.f);
	matWorld = matScale*matTrans;

	float fCenterX = pTexInfo->tImgInfo.Width >> 1;
	float fCenterY = pTexInfo->tImgInfo.Height >> 1;

	CDevice::Get_Instance()->Render_Begin();
	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	CDevice::Get_Instance()->Render_End(m_Picture.m_hWnd);

	Invalidate(FALSE);
}


void CTab3::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.


	*pResult = 0;
}


void CTab3::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	if (pView->m_pPicked)
	{
		// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
		if (IDC_SLIDER1 == pScrollBar->GetDlgCtrlID())
		{
			//Slider Control �� �б�
			int iPos = m_sld.GetPos();

			//Edit Box�� ǥ��
			CString sPos;
			sPos.Format(_T("%d"), iPos);
			m_edit.SetWindowText(sPos);
		}
		else if (IDC_SLIDER2 == pScrollBar->GetDlgCtrlID())
		{
			//Slider Control �� �б�
			int iPos = m_sld2.GetPos();

			//Edit Box�� ǥ��
			CString sPos;
			sPos.Format(_T("%d"), iPos);
			m_edit2.SetWindowText(sPos);
		}

		else if (IDC_SLIDER3 == pScrollBar->GetDlgCtrlID())
		{
			//Slider Control �� �б�
			int iPos = m_sld3.GetPos();

			//Edit Box�� ǥ��
			CString sPos;
			sPos.Format(_T("%d"), iPos);
			m_edit3.SetWindowText(sPos);
		}



		CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	}
	
}


void CTab3::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	//Edit Box �� �б�


	CString sPos;
	m_edit.GetWindowText(sPos);
	m_iObjSize = _ttoi(sPos);

	//Slider Control�� ����
	m_sld.SetPos(m_iObjSize);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	if (pView->m_pPicked)
	{
		pView->m_pPicked->vSize.x = m_iObjSize*0.1f;
		pView->Invalidate(FALSE);

		CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMain->m_SecondSplitter.GetPane(0, 0));
		pMiniView->Invalidate(FALSE);
	}

}


void CTab3::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CTab3::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	CString sPos2;
	m_edit2.GetWindowText(sPos2);
	m_iObjSize2 = _ttoi(sPos2);

	//Slider Control�� ����
	m_sld2.SetPos(m_iObjSize2);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	if (pView->m_pPicked)
	{
		pView->m_pPicked->vSize.y = m_iObjSize2*0.1f;
		pView->Invalidate(FALSE);

		CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMain->m_SecondSplitter.GetPane(0, 0));
		pMiniView->Invalidate(FALSE);
	}
}


void CTab3::OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CTab3::OnEnChangeEdit3()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	CString sPos3;
	m_edit3.GetWindowText(sPos3);
	m_iObjSize3 = _ttoi(sPos3);

	//Slider Control�� ����
	m_sld3.SetPos(m_iObjSize3);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	if (pView->m_pPicked)
	{
		pView->m_pPicked->fRadian = m_iObjSize3*(D3DX_PI / 180.f);
		pView->Invalidate(FALSE);

		CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMain->m_SecondSplitter.GetPane(0, 0));
		pMiniView->Invalidate(FALSE);
	}
}


void CTab3::OnDeletePickedObj()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	if (!pView->m_vecPicked.empty())
	{
		for (auto& iter : pView->m_vecPicked)
		{
			auto iterIndex = find(pView->m_pObjPick->m_vecObj.begin(), pView->m_pObjPick->m_vecObj.end(), iter);

			if (iterIndex == pView->m_pObjPick->m_vecObj.end())
				continue;
			Safe_Delete(*iterIndex);
			iterIndex = pView->m_pObjPick->m_vecObj.erase(iterIndex);
		}

		pView->m_vecPicked.clear();
		pView->m_pPicked = nullptr;
		pView->m_bPickObj = false;
		pView->iPickObjIndex = -1;
		pView->m_bCtrl = false;

		pView->Invalidate(FALSE);

	}
	else if (pView->m_pPicked)
	{
		int	iIndex = pView->iPickObjIndex;

		pView->m_pObjPick->Delete_Object(iIndex);
		pView->m_pPicked = nullptr;
		pView->m_bPickObj = false;
		pView->iPickObjIndex = -1;

		pView->Invalidate(FALSE);
		CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMain->m_SecondSplitter.GetPane(0, 0));
		pMiniView->Invalidate(FALSE);
	}

}
