#pragma once

#include <stdexcept>
#include "tstring.h"

class Exception
{
protected:
	tstring m_strMessage;

protected:
	Exception() {}
public:
	Exception(tstring msg);
	~Exception(void);

	tstring& get_Message()
	{
		return m_strMessage;
	}
};

// To throw a exception with win32 error
//
class Win32ErrorException
	: public Exception
{
public:
	Win32ErrorException();

	Win32ErrorException(unsigned int uiErrCode);
};

class FatalErrorException
	: public Exception
{
public:
	FatalErrorException(tstring msg)
		:Exception(msg)
	{
	}

	FatalErrorException(TCHAR* msg)
		:Exception(msg)
	{
	}
};

class CrtErrorException
	: public Exception
{
public:
	CrtErrorException();
	CrtErrorException(int crtErrNo);
};