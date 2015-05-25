#include "StdAfx.h"
//#include <windows.h>
#include "EncodeHelper.h"

std::string EncodeHelper::UnicodeToANSI(const std::wstring& str)
{
	//unicode --> ansi
	std::string result;
	int len = ::WideCharToMultiByte(CP_ACP, NULL, str.c_str(), str.size(), NULL, 0, NULL, NULL);
	result.assign(len, 0);
	char* p = (char*)result.data();
	::WideCharToMultiByte(CP_ACP, NULL, str.data(), str.size(), p, len, NULL, NULL);
	return result;
}

std::wstring EncodeHelper::ANSIToUnicode(const std::string& str)
{
	//ansi --> unicode
	std::wstring result;
	int len = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	result.assign(len, 0);
	wchar_t* p = (wchar_t*)result.data();
	::MultiByteToWideChar(CP_ACP, NULL, str.data(), str.size(), p, len);

	// skip 0xfeff
	/*
	wchar_t a=result.at(0);
	if (a==0xfeff)
	{
		result = result.substr(1,result.size()-1);
	}
	*/

	return result;
}

std::string EncodeHelper::ANSIToUtf8(const std::string& str)
{
	//ansi --> unicode
	std::wstring resultTmp;
	int len = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	resultTmp.assign(len, 0);
	wchar_t* p = (wchar_t*)resultTmp.data();
	::MultiByteToWideChar(CP_ACP, NULL, str.data(), str.size(), p, len);

	//unicode --> utf8
	std::string result;
	len = ::WideCharToMultiByte(CP_UTF8, NULL, resultTmp.c_str(), resultTmp.size(), NULL, 0, NULL, NULL);
	result.assign(len, 0);
	char* pResult = (char*)result.data();
	::WideCharToMultiByte(CP_UTF8, NULL, resultTmp.data(), str.size(), pResult, len, NULL, NULL);

	return result;
}

std::string EncodeHelper::Utf8ToANSI(const std::string& str)
{
	//utf8 --> unicode
	std::wstring resultTmp;
	int len = ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.size(), NULL, 0);
	resultTmp.assign(len, 0);
	wchar_t* p = (wchar_t*)resultTmp.data();
	::MultiByteToWideChar(CP_UTF8, NULL, str.data(), str.size(), p, len);

	//unicode --> ansi
	std::string result;
	len = ::WideCharToMultiByte(CP_ACP, NULL, resultTmp.c_str(), str.size(), NULL, 0, NULL, NULL);
	result.assign(len, 0);
	char* pResult = (char*)result.data();
	::WideCharToMultiByte(CP_ACP, NULL, resultTmp.data(), str.size(), pResult, len, NULL, NULL);

	return result;
}

std::string EncodeHelper::UnicodeToUtf8(const std::wstring& str)
{
	// unicode --> utf8
	std::string result;
	int len = ::WideCharToMultiByte(CP_UTF8, NULL, str.c_str(), str.size(), NULL, 0, NULL, NULL);
	result.assign(len, 0);
	char* p = (char*)result.data();
	::WideCharToMultiByte(CP_UTF8, NULL, str.data(), str.size(), p, len, NULL, NULL);

	return result;
}

std::wstring EncodeHelper::Utf8ToUnicode(const std::string& str)
{
	// utf8 --> unicode
	std::wstring result;
	int len = ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.size(), NULL, 0);
	result.assign(len, 0);
	wchar_t* p = (wchar_t*)result.data();
	::MultiByteToWideChar(CP_UTF8, NULL, str.data(), str.size(), p, len);

	// skip 0xfeff
	/*
	wchar_t a=result.at(0);
	if (a==0xfeff)
	{
		result = result.substr(1,result.size()-1);
	}
	*/

	return result;
}