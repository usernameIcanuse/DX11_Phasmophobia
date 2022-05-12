#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CMyForm 폼 뷰입니다.
class CTab1;
class CTab3;

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyForm();

public:
	CTab1* pDlg1;
	CTab3* pDlg3;

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	void		Release();

public:
	CFont	m_Font;
	CTabCtrl m_Tab;



	int		 m_iDrawID;
	int		 m_iDrawIDObj;


	bool	 m_bTile;


public:
	virtual void OnInitialUpdate();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnSaveData();
	afx_msg void OnLoadData();
};


