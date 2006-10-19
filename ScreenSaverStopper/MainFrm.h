//	$Id$
//	ScreenSaverStopper
//	Copyright(C) 2006, M. Sugiura, All rights reserved.
//	DESCRIPTION: Declarations of CMainFrame class

// MainFrm.h : CMainFrame クラスのインターフェイス
//


#pragma once

#include "ChildView.h"
#include "Configuration.h"
#include "ScreenSaverController.h"
#include "TaskBarIcon.h"

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// 属性
public:

// 操作
public:

// オーバーライド
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// 実装
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	CChildView				m_wndView;
	CConfiguration			m_config;
	CScreenSaverController	m_ssController;
	CTaskBarIcon			m_taskBarIcon;
	BOOL					m_bInitialStatus;
	UINT					m_uTaskBarCreatedMsg;

// 生成された、メッセージ割り当て関数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()

	HRESULT HandleMenu();

	LRESULT DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	afx_msg void OnDestroy();
	afx_msg LRESULT OnTaskBarIconMessage(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
};


