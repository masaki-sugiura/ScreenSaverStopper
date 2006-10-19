//	$Id$
//	ScreenSaverStopper
//	Copyright(C) 2006, M. Sugiura, All rights reserved.
//	DESCRIPTION: Implementations of CConfiguration class

#include "StdAfx.h"
#include "Configuration.h"

CConfiguration::CConfiguration(LPCTSTR pszIniFileName)
	:	m_strIniFileName(pszIniFileName)
	,	m_strSectionName(__TEXT("Configuration"))
	,	m_beDisableAlwaysOnStart(__TEXT("DisableAlwaysOnStart"))
	,	m_beSaveStatusOnQuit(__TEXT("SaveStatusOnQuit")) 
{
}

CConfiguration::~CConfiguration()
{
}

HRESULT CConfiguration::Read(DWORD dwPropID, LPVOID pData)
{
	ASSERT(pData);

	HRESULT hr;
	switch (dwPropID) {
	case CID_DISABLE_ALWAYS_ON_START:
		hr = m_beDisableAlwaysOnStart.Get(m_strIniFileName, m_strSectionName, *(BOOL*)pData);
		break;
	case CID_SAVE_STATUS_ON_QUIT:
		hr = m_beSaveStatusOnQuit.Get(m_strIniFileName, m_strSectionName, *(BOOL*)pData);
		break;
	default:
		ASSERT(!__TEXT("Unknown Property ID is specified!!"));
		return E_INVALIDARG;
	}

	return hr;
}

HRESULT CConfiguration::Write(DWORD dwPropID, const LPVOID pData)
{
	ASSERT(pData);

	HRESULT hr;
	switch (dwPropID) {
	case CID_DISABLE_ALWAYS_ON_START:
		hr = m_beDisableAlwaysOnStart.Set(m_strIniFileName, m_strSectionName, *(const BOOL*)pData);
		break;
	case CID_SAVE_STATUS_ON_QUIT:
		hr = m_beSaveStatusOnQuit.Set(m_strIniFileName, m_strSectionName, *(const BOOL*)pData);
		break;
	default:
		ASSERT(!__TEXT("Unknown Property ID is specified!!"));
		return E_INVALIDARG;
	}

	return hr;
}

CConfiguration::CBooleanEntry::CBooleanEntry(LPCTSTR pszKeyName)
	:	m_strKeyName(pszKeyName)
{
}

HRESULT CConfiguration::CBooleanEntry::Get(const CString& strIniFileName, const CString& strSectionName, BOOL& bValue)
{
	bValue = (BOOL)::GetPrivateProfileInt(strSectionName, m_strKeyName, -1, strIniFileName);
	if (bValue == -1) {
		bValue = FALSE;
		return S_FALSE;
	}

	return S_OK;
}

HRESULT CConfiguration::CBooleanEntry::Set(const CString& strIniFileName, const CString& strSectionName, BOOL bValue)
{
	TCHAR szBuffer[16];
	::wsprintf(szBuffer, __TEXT("%d"), bValue);

	BOOL bRet = ::WritePrivateProfileString(strSectionName, m_strKeyName, szBuffer, strIniFileName);
	if (!bRet) {
		DWORD dwErr = ::GetLastError();
		return HRESULT_FROM_WIN32(dwErr);
	}

	return S_OK;
}
