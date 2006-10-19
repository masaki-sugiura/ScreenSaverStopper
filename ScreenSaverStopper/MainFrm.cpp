//	$Id$
//	ScreenSaverStopper
//	Copyright(C) 2006, M. Sugiura, All rights reserved.
//	DESCRIPTION: Implementations of CMainFrame class

// MainFrm.cpp : CMainFrame �N���X�̎���
//

#include "stdafx.h"
#include "ScreenSaverStopper.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	WM_PRIVATE_TASKBAR_ICON_MESSAGE	(WM_APP+100)

#define	ICON_ID		1

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_PRIVATE_TASKBAR_ICON_MESSAGE, &CMainFrame::OnTaskBarIconMessage)
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()


// CMainFrame �R���X�g���N�V����/�f�X�g���N�V����

CMainFrame::CMainFrame()
	:	m_config(__TEXT("ScreenSaverStopper.ini"))
{
	// TODO: �����o�������R�[�h�������ɒǉ����Ă��������B
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// �t���[���̃N���C�A���g�̈�S�̂��߂�r���[���쐬���܂��B
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("�r���[ �E�B���h�E���쐬�ł��܂���ł����B\n");
		return -1;
	}

	HRESULT hr = m_ssController.GetStatus(m_bInitialStatus);
	if (FAILED(hr)) {
		TRACE0("Failed to Get Initial Status\n");
		return -1;
	}

	BOOL bDisableAlwaysOnStart;
	hr = m_config.Read(CID_DISABLE_ALWAYS_ON_START, &bDisableAlwaysOnStart);
	if (FAILED(hr)) {
		TRACE0("Failed to Read Config\n");
		return -1;
	}

	hr = m_taskBarIcon.Register(ICON_ID, this->GetSafeHwnd(), WM_PRIVATE_TASKBAR_ICON_MESSAGE,
								bDisableAlwaysOnStart ? FALSE : m_bInitialStatus);
	if (FAILED(hr)) {
		TRACE0("Failed to register TaskBar Icon\n");
		return -1;
	}

	if (bDisableAlwaysOnStart && m_bInitialStatus) {
		hr = m_ssController.SetStatus(FALSE);
		if (FAILED(hr)) {
			TRACE0("Failed to SetStatus\n");
			return -1;
		}
	}

	m_uTaskBarCreatedMsg = ::RegisterWindowMessage(__TEXT("TaskbarCreated"));

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

HRESULT CMainFrame::HandleMenu()
{
	HMENU hMenu = ::LoadMenu(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	ASSERT(hMenu);

	HMENU hMenuChild = ::GetSubMenu(hMenu, 0);

	MENUITEMINFO miInfo;
	memset(&miInfo, 0, sizeof(miInfo));
	miInfo.cbSize	= sizeof(miInfo);
	miInfo.fMask	= MIIM_STATE;

	BOOL bDisableAlwaysOnStart, bSaveStatusOnQuit;
	HRESULT hr = m_config.Read(CID_DISABLE_ALWAYS_ON_START, &bDisableAlwaysOnStart);
	if (FAILED(hr)) {
		::DestroyMenu(hMenu);
		return hr;
	}

	hr = m_config.Read(CID_SAVE_STATUS_ON_QUIT, &bSaveStatusOnQuit);
	if (FAILED(hr)) {
		::DestroyMenu(hMenu);
		return hr;
	}

	miInfo.fState	= bDisableAlwaysOnStart ? MFS_CHECKED : MFS_UNCHECKED;
	::SetMenuItemInfo(hMenuChild, ID_DISABLE_ALWAYS_ON_START, FALSE, &miInfo);

	miInfo.fState	= bSaveStatusOnQuit ? MFS_CHECKED : MFS_UNCHECKED;
	::SetMenuItemInfo(hMenuChild, ID_SAVE_STATUS_ON_QUIT, FALSE, &miInfo);

	POINT pos;
	::GetCursorPos(&pos);

	SetForegroundWindow();
	SetFocus();

	int nSelect = ::TrackPopupMenuEx(hMenuChild, TPM_RETURNCMD | TPM_NONOTIFY | TPM_BOTTOMALIGN,
									 pos.x, pos.y, this->GetSafeHwnd(), NULL);

	PostMessage(WM_NULL);

	::DestroyMenu(hMenu);

	hr = S_OK;
	switch (nSelect) {
	case 0:	//	cancel
		break;
	case ID_APP_EXIT:
		hr = S_FALSE;
		break;
	case ID_DISABLE_ALWAYS_ON_START:
		bDisableAlwaysOnStart = !bDisableAlwaysOnStart;
		hr = m_config.Write(CID_DISABLE_ALWAYS_ON_START, &bDisableAlwaysOnStart);
		break;
	case ID_SAVE_STATUS_ON_QUIT:
		bSaveStatusOnQuit = !bSaveStatusOnQuit;
		hr = m_config.Write(CID_SAVE_STATUS_ON_QUIT, &bSaveStatusOnQuit);
		break;
	default:
		ASSERT(!__TEXT("Unknown ID is returned from TrackPopupMenuEx()"));
		hr = E_UNEXPECTED;
		break;
	}

	return hr;
}

LRESULT CMainFrame::DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == m_uTaskBarCreatedMsg) {
		//	�V�F���ċN�����̃A�C�R���ēo�^
		if (m_taskBarIcon.IsRegistered()) {
			m_taskBarIcon.Unregister();
			m_taskBarIcon.Register(ICON_ID, this->GetSafeHwnd(), WM_PRIVATE_TASKBAR_ICON_MESSAGE,
								   m_taskBarIcon.GetState());
		}
		return 0;
	} else {
		return CFrameWnd::DefWindowProc(uMsg, wParam, lParam);
	}
}

// CMainFrame �f�f

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame ���b�Z�[�W �n���h��

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// �r���[ �E�B���h�E�Ƀt�H�[�J�X��^���܂��B
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// �r���[�ɍŏ��ɃR�}���h����������@���^���܂��B
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// ����ȊO�̏ꍇ�́A����̏������s���܂��B
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnDestroy()
{
	m_taskBarIcon.Unregister();

	BOOL bSaveStatusOnQuit;
	HRESULT hr = m_config.Read(CID_SAVE_STATUS_ON_QUIT, &bSaveStatusOnQuit);
	if (SUCCEEDED(hr) && !bSaveStatusOnQuit) {
		m_ssController.SetStatus(m_bInitialStatus);
	}

	CFrameWnd::OnDestroy();

	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
}

LRESULT CMainFrame::OnTaskBarIconMessage(WPARAM wParam, LPARAM lParam)
{
	ASSERT(wParam == ICON_ID);

	HRESULT hr = S_OK;

	switch (lParam) {
	case WM_LBUTTONDOWN:
		{
			BOOL bNewState = !m_taskBarIcon.GetState();
			hr = m_ssController.SetStatus(bNewState);
			if (SUCCEEDED(hr)) {
				hr = m_taskBarIcon.SetState(bNewState);
			}
		}
		break;

	case WM_RBUTTONDOWN:
		hr = HandleMenu();
		if (hr == S_FALSE) {
			DestroyWindow();
		}
		break;

	default:
		break;
	}

	return hr;
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CFrameWnd::OnSettingChange(uFlags, lpszSection);

	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
#ifdef	_DEBUG
	::OutputDebugString(__TEXT("Enter OnSettingChange()\n"));
#endif

	BOOL bState = m_taskBarIcon.GetState();
	m_ssController.SetStatus(bState);
}
