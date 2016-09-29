#include "stdafx.h"
#include "Exceptions.h"

Exception::Exception(tstring msg)
	:m_strMessage(msg)
{
}


Exception::~Exception(void)
{
}


void GetWin32ErrorMessage(DWORD dwErrCode, tstring& msg)
{
	LPTSTR lpMsgBuf = NULL;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwErrCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );
	msg = tstring(lpMsgBuf);
	LocalFree(lpMsgBuf);
}

Win32ErrorException::Win32ErrorException()
	:Exception()
{
	DWORD dwErrCode = GetLastError();
	GetWin32ErrorMessage(dwErrCode, m_strMessage);
}

Win32ErrorException::Win32ErrorException(unsigned int uiErrCode)
	:Exception()
{
	GetWin32ErrorMessage(uiErrCode, m_strMessage);
}

void GetCrtErrorMessage(int iErrCode, tstring& msg)
{
	const PCTSTR errorMsgs[]= {
		NULL,
		_T("Operation not permitted."),
		_T("No such file or directory."),
		_T("No such process."),
		_T("Interrupted function"),
		_T("I/O error"),
		_T("No such device or address"),
		_T("Argument list too long"),
		_T("Exec format error"),
		_T("Bad file number"),
		_T("No spawned processes"),
		_T("No more processes or not enough memory or maximum nesting level reached"),
		_T("Not enough memory"),
		_T("Permission denied"),
		_T("Bad address"),
		NULL,
		_T("Device or resource busy"),
		_T("File exists"),
		_T("Cross-device link"),
		_T("No such device"),
		_T("Not a directory"),
		_T("Is a directory"),
		_T("Invalid argument"),
		_T("Too many files open in system"),
		_T("Too many open files"),
		_T("Inappropriate I/O control operation"),
		NULL,
		_T("File too large"),
		_T("No space left on device"),
		_T("Invalid seek"),
		_T("Read-only file system"),
		_T("Too many links"),
		_T("Broken pipe"),
		_T("Math argument"),
		_T("Result too large"),
		NULL,
		_T("Resource deadlock would occur"),
		NULL,
		_T("Filename too long"),
		_T("No locks available"),
		_T("Function not supported"),
		_T("Directory not empty"),
		_T("Illegal byte sequence"),
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		_T("")
	};

	if(iErrCode < 0 && iErrCode >= sizeof(errorMsgs)/sizeof(PCTSTR))
	{
		return;
	}
	if(errorMsgs[iErrCode] != NULL)
	{
		msg = errorMsgs[iErrCode];
	}
}

CrtErrorException::CrtErrorException()
	:Exception()
{
	GetCrtErrorMessage(errno, m_strMessage);
}

CrtErrorException::CrtErrorException(int crtErrNo)
	:Exception()
{
	GetCrtErrorMessage(crtErrNo, m_strMessage);
}