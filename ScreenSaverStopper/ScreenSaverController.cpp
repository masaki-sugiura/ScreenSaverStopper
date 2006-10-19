//	$Id$
//	ScreenSaverStopper
//	Copyright(C) 2006, M. Sugiura, All rights reserved.
//	DESCRIPTION: Implementations of CScreenSaverController class

#include "StdAfx.h"
#include "ScreenSaverController.h"

CScreenSaverController::CScreenSaverController()
{
}

CScreenSaverController::~CScreenSaverController()
{
}

HRESULT CScreenSaverController::SetStatus(BOOL bEnable)
{
	BOOL bRet = ::SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, bEnable, NULL, 0);
	if (!bRet) {
		DWORD dwErr = ::GetLastError();
		return HRESULT_FROM_WIN32(dwErr);
	}

	return S_OK;
}

HRESULT CScreenSaverController::GetStatus(BOOL& bEnable)
{
	BOOL bRet = ::SystemParametersInfo(SPI_GETSCREENSAVEACTIVE, 0, &bEnable, 0);
	if (!bRet) {
		DWORD dwErr = ::GetLastError();
		return HRESULT_FROM_WIN32(dwErr);
	}

	return S_OK;
}

