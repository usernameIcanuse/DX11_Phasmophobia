#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CTab1 대화 상자입니다.

class CTab1 : public CDialogEx
{
	DECLARE_DYNAMIC(CTab1)

public:
	CTab1(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTab1();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeList1();

public:
	int		 m_iDrawID;

	virtual BOOL OnInitDialog();
	CListBox m_ListBox;
	CStatic m_Picture;


};
