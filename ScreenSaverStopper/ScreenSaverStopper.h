//	$Id$
//	ScreenSaverStopper
//	Copyright(C) 2006, M. Sugiura, All rights reserved.
//	DESCRIPTION: Declarations of CScreenSaverStopperApp class

// ScreenSaverStopper.h : ScreenSaverStopper アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル


// CScreenSaverStopperApp:
// このクラスの実装については、ScreenSaverStopper.cpp を参照してください。
//

class CScreenSaverStopperApp : public CWinApp
{
public:
	CScreenSaverStopperApp();


// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

public:
	DECLARE_MESSAGE_MAP()
};

extern CScreenSaverStopperApp theApp;