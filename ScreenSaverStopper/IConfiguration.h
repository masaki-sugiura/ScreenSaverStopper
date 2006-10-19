//	$Id$
//	ScreenSaverStopper
//	Copyright(C) 2006, M. Sugiura, All rights reserved.
//	DESCRIPTION: Declarations of IConfiguration interface

#pragma once

#include "stdafx.h"

enum CONFIG_ID
{
	CID_DISABLE_ALWAYS_ON_START = 1,
	CID_SAVE_STATUS_ON_QUIT = 2
};

class IConfiguration
{
public:
	virtual HRESULT Read(DWORD dwPropID, LPVOID pData) = 0;
	virtual HRESULT Write(DWORD dwPropID, const LPVOID pData) = 0;
};
