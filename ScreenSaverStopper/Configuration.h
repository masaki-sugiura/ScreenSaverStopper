//	$Id$
//	ScreenSaverStopper
//	Copyright(C) 2006, M. Sugiura, All rights reserved.
//	DESCRIPTION: Declarations of CConfiguration class

#pragma once

#include "IConfiguration.h"

class CConfiguration : public IConfiguration
{
public:
	CConfiguration(LPCTSTR pszIniFileName);
	~CConfiguration();

	HRESULT Read(DWORD dwPropID, LPVOID pData);
	HRESULT Write(DWORD dwPropID, const LPVOID pData);

private:
	CString m_strIniFileName;
	CString m_strSectionName;

	class CBooleanEntry
	{
	public:
		CBooleanEntry(LPCTSTR pszKeyName);

		HRESULT Get(const CString& strIniFileName, const CString& strSectionName, BOOL& bValue);
		HRESULT Set(const CString& strIniFileName, const CString& strSectionName, BOOL bValue);

	private:
		CString m_strKeyName;
	};

	CBooleanEntry m_beDisableAlwaysOnStart;
	CBooleanEntry m_beSaveStatusOnQuit;
};
