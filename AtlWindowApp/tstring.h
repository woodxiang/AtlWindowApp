#pragma once

#include <tchar.h>
#include <string>
#include <iostream>

#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(P)          (P)
#endif

#ifndef tstring

typedef std::basic_string<TCHAR> tstring;

#endif

#ifdef _UNICODE

#define STDTIN std::wcin
#define STDTOUT	std::wcout
#define COUT std::wcout
#define CIN	std::wcin

#define IFSTREAM std::wifstream
#define OFSTREAM std::wofstream
#define ISTREAM std::wistream
#define OSTREAM std::wostream
#define STREAMBUF std::wstreambuf
#define TSTRINGSTREAM std::wstringstream
#define CERR std::wcerr
#define PATH_STRING(path) (path).wstring()
#else

#define STDTIN std::cin
#define STDTOUT std::cout
#define COUT std::cout
#define CIN	std::cin

#define IFSTREAM std::ifstream
#define OFSTREAM std::ofstream
#define ISTREAM std::istream
#define OSTREAM std::ostream
#define STREAMBUF std::streambuf
#define TSTRINGSTREAM std::stringstream
#define CERR std::cerr
#define PATH_STRING(path) (path).string()
#endif

#ifdef PROGRESS_DISABLED
#define	REPORTPROGRESS ;/##/
#else
#define REPORTPROGRESS COUT
#endif

#ifdef _DEBUG

#define DEBUG_TRACE COUT

#else

#define DEBUG_TRACE /##/

#endif

#define INIT_STRUCTURE(a) ZeroMemory(&a, sizeof(a))


// Convert Wide string to string
//
std::string ConvertString(std::wstring input);

bool SplitKeyValuePair(tstring& strInput, tstring& strKey, tstring& strValue);

template <typename T, T space>
std::basic_string<T>& Trim(std::basic_string<T>& str)
{
	std::basic_string<T>::size_type size = str.length();
	std::basic_string<T>::size_type srcIndex = 0, nShift = size, nCutFrom = 0;
	for (; srcIndex < size; srcIndex++)
	{
		if (str[srcIndex] != space && str[srcIndex] != space + '\t' - ' ')
		{
			nShift = srcIndex;
			break;
		}
	}

	nCutFrom = srcIndex;

	for (; srcIndex < size; srcIndex++)
	{
		if (str[srcIndex] != space && str[srcIndex] != space + '\t' - ' ')
		{
			nCutFrom = srcIndex + 1;
		}
	}

	if (nShift > 0)
	{
		str.erase(0, nShift);
	}

	if (nCutFrom < size)
	{
		str.erase(nCutFrom - nShift, size - nCutFrom);
	}

	return str;
}

#define TRIM Trim<TCHAR, _T(' ')>
