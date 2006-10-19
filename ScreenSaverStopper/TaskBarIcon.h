//	$Id$
//	ScreenSaverStopper
//	Copyright(C) 2006, M. Sugiura, All rights reserved.
//	DESCRIPTION: Declarations of CTaskBarIcon class

#pragma once

#include "stdafx.h"

#include <afxmt.h>

class CTaskBarIcon
{
public:
	CTaskBarIcon();
	~CTaskBarIcon();

	BOOL IsRegistered() const
	{
		return m_ntfIconData.uID != 0 && m_ntfIconData.hWnd != NULL;
	}

	HRESULT Register(UINT uID, HWND hWnd, UINT nNotifyMsg, BOOL bInitialState);
	HRESULT Unregister();

	HRESULT SetState(BOOL bEnable);
	BOOL GetState() const
	{
		return m_bEnable;
	}

private:
	NOTIFYICONDATA		m_ntfIconData;
	HICON				m_hEnable, m_hDisable;
	BOOL				m_bEnable;
	CCriticalSection	m_csLock;

	HRESULT RegisterRaw(UINT uID, HWND hWnd, UINT uNotifyMsg, BOOL bInitialState);

	HRESULT Notify(int nCmd);
};
