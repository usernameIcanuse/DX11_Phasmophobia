#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CTab1 ��ȭ �����Դϴ�.

class CTab1 : public CDialogEx
{
	DECLARE_DYNAMIC(CTab1)

public:
	CTab1(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTab1();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeList1();

public:
	int		 m_iDrawID;

	virtual BOOL OnInitDialog();
	CListBox m_ListBox;
	CStatic m_Picture;


};
