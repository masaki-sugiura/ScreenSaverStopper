//	$Id$
//	ScreenSaverStopper
//	Copyright(C) 2006, M. Sugiura, All rights reserved.
//	DESCRIPTION: Declarations of CScreenSaverStopperApp class

// ScreenSaverStopper.h : ScreenSaverStopper �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"       // ���C�� �V���{��


// CScreenSaverStopperApp:
// ���̃N���X�̎����ɂ��ẮAScreenSaverStopper.cpp ���Q�Ƃ��Ă��������B
//

class CScreenSaverStopperApp : public CWinApp
{
public:
	CScreenSaverStopperApp();


// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();

// ����

public:
	DECLARE_MESSAGE_MAP()
};

extern CScreenSaverStopperApp theApp;