
// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#pragma once

#include "Include.h"
#include "MyForm.h"
#include "ToolView.h"
#include "MiniView.h"

class CMainFrame : public CFrameWnd
{
	
protected: // serialization������ ��������ϴ�.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// �����Դϴ�.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;
	
public:
	CSplitterWnd	  m_MainSplitter;		// CSplitterWnd : â������ ���� Ŭ����, CWnd�� �ڽ� Ŭ����
	CSplitterWnd	  m_SecondSplitter;		// �� ��° ���ø��͸� �̿��Ͽ� �ٸ� ���� ���¸� �����.

// ������ �޽��� �� �Լ�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};


