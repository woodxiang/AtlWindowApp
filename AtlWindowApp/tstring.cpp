#include "stdafx.h"
#include "tstring.h"
#include <atlbase.h>

std::string ConvertString(std::wstring input)
{
	return CW2A(input.c_str()).m_psz;
}

bool SplitKeyValuePair(tstring& strInput, tstring& strKey, tstring& strValue)
{
	tstring::size_type index = strInput.find(_T('='));
	if(tstring::npos == index)
	{
		return false;
	}

	strKey = strInput.substr(0, index);
	strValue = strInput.substr(index+1);
	TRIM(strKey);
	TRIM(strValue);

	return true;
}
