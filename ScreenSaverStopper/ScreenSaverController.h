//	$Id$
//	ScreenSaverStopper
//	Copyright(C) 2006, M. Sugiura, All rights reserved.
//	DESCRIPTION: Declarations of CScreenSaverController class

#pragma once

class CScreenSaverController
{
public:
	CScreenSaverController();
	~CScreenSaverController();

	HRESULT SetStatus(BOOL bEnable);
	HRESULT GetStatus(BOOL& bEnable);
};
