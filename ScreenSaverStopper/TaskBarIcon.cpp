//	$Id$
//	ScreenSaverStopper
//	Copyright(C) 2006, M. Sugiura, All rights reserved.
//	DESCRIPTION: Implementations of CTaskBarIcon class

#include "StdAfx.h"
#include "TaskBarIcon.h"
#include "resource.h"

#define	TIP_ENABLE	__TEXT("—LŒø")
#define	TIP_DISABLE	__TEXT("–³Œø")

CTaskBarIcon::CTaskBarIcon()
	:	m_bEnable(FALSE)
{
	m_hEnable	= ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ENABLE));
	m_hDisable	= ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_DISABLE));

	memset(&m_ntfIconData, 0, sizeof(m_ntfIconData));
	m_ntfIconData.cbSize = sizeof(m_ntfIconData);
}

CTaskBarIcon::~CTaskBarIcon()
{
	::DestroyIcon(m_hEnable);
	::DestroyIcon(m_hDisable);
}

HRESULT CTaskBarIcon::Register(UINT uID, HWND hWnd, UINT uNotifyMsg, BOOL bInitialState)
{
	if (IsRegistered()) {
//		return E_UNEXPECTED;
		Unregister();
	}

	int nCount = 10;

	do {
		HRESULT hr = RegisterRaw(uID, hWnd, uNotifyMsg, bInitialState);
		if (SUCCEEDED(hr)) {
			break;
		} else if (hr != HRESULT_FROM_WIN32(ERROR_TIMEOUT)) {
			return hr;
		}

		hr = SetState(bInitialState);
		if (SUCCEEDED(hr)) {
			break;
		}

		::Sleep(1000);

	} while (nCount-- > 0);

	if (nCount < 0) {
		return HRESULT_FROM_WIN32(ERROR_TIMEOUT);
	}

	m_bEnable = bInitialState;

	return S_OK;
}

HRESULT CTaskBarIcon::Unregister()
{
	CSingleLock lock(&m_csLock, TRUE);

	if (!IsRegistered()) {
		return S_OK;
	}

	HRESULT hr = Notify(NIM_DELETE);
	if (SUCCEEDED(hr)) {
		m_ntfIconData.uID = 0;
		m_ntfIconData.hWnd = NULL;
	}

	return hr;
}

HRESULT CTaskBarIcon::SetState(BOOL bEnable)
{
	CSingleLock lock(&m_csLock, TRUE);

	if (!IsRegistered()) {
		return E_UNEXPECTED;
	}

	m_ntfIconData.uFlags	= NIF_ICON | NIF_TIP;
	m_ntfIconData.hIcon		= bEnable ? m_hEnable : m_hDisable;
	_tcscpy(m_ntfIconData.szTip, bEnable ? TIP_ENABLE : TIP_DISABLE);

	HRESULT hr = Notify(NIM_MODIFY);
	if (SUCCEEDED(hr)) {
		m_bEnable = bEnable;
	}

	return hr;
}

HRESULT CTaskBarIcon::RegisterRaw(UINT uID, HWND hWnd, UINT uNotifyMsg, BOOL bInitialState)
{
	CSingleLock lock(&m_csLock, TRUE);

	m_ntfIconData.uID		= uID;
	m_ntfIconData.hWnd		= hWnd;
	m_ntfIconData.uFlags	= NIF_ICON | NIF_MESSAGE | NIF_TIP;
	m_ntfIconData.uCallbackMessage	= uNotifyMsg;
	m_ntfIconData.hIcon		= bInitialState ? m_hEnable : m_hDisable;
	_tcscpy(m_ntfIconData.szTip, bInitialState ? TIP_ENABLE : TIP_DISABLE);

	HRESULT hr = Notify(NIM_ADD);

	return hr;
}

HRESULT CTaskBarIcon::Notify(int nCmd)
{
	BOOL bRet = ::Shell_NotifyIcon(nCmd, &m_ntfIconData);
	if (!bRet) {
		DWORD dwErr = ::GetLastError();
		return HRESULT_FROM_WIN32(dwErr);
	}

	return S_OK;
}

