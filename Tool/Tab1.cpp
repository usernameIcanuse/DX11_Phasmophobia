// Tab1.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "Tab1.h"
#include "afxdialogex.h"
#include "TextureMgr.h"
#include "Device.h"


// CTab1 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CTab1, CDialogEx)

CTab1::CTab1(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CTab1::~CTab1()
{
}

void CTab1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE1, m_Picture);
	
}


BEGIN_MESSAGE_MAP(CTab1, CDialogEx)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST1, &CTab1::OnLbnSelchangeList1)
	
END_MESSAGE_MAP()


// CTab1 �޽��� ó�����Դϴ�.


void CTab1::OnLbnSelchangeList1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", m_iDrawID);

	if (nullptr == pTexInfo)
		return;

	D3DXMATRIX  matScale, matTrans, matWorld;

	float fRatioX = WINCX / TILECX;
	float fRatioY = WINCY / TILECY;

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

	UpdateData(FALSE);
}


BOOL CTab1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_ListBox.Dir(DDL_ARCHIVE, L"..\\Texture\\Stage\\Terrain\\Tile\\*.png");


	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


