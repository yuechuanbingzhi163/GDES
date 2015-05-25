#include "reference.h"
#include "charset.h"

string chestnut_unicode_to_ansi(const wstring& str)
{
	//unicode --> ansi
	string result;
	int len = ::WideCharToMultiByte(CP_ACP, NULL, str.c_str(), str.size(), NULL, 0, NULL, NULL);
	result.assign(len, 0);
	char* p = (char*)result.data();
	::WideCharToMultiByte(CP_ACP, NULL, str.data(), str.size(), p, len, NULL, NULL);
	return result;
}

wstring chestnut_ansi_to_unicode(const string& str)
{
	//ansi --> unicode
	wstring result;
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

string chestnut_ansi_to_utf8(const string& str)
{
	//ansi --> unicode
	wstring resultTmp;
	int len = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	resultTmp.assign(len, 0);
	wchar_t* p = (wchar_t*)resultTmp.data();
	::MultiByteToWideChar(CP_ACP, NULL, str.data(), str.size(), p, len);

	//unicode --> utf8
	string result;
	len = ::WideCharToMultiByte(CP_UTF8, NULL, resultTmp.c_str(), resultTmp.size(), NULL, 0, NULL, NULL);
	result.assign(len, 0);
	char* pResult = (char*)result.data();
	::WideCharToMultiByte(CP_UTF8, NULL, resultTmp.data(), str.size(), pResult, len, NULL, NULL);

	return result;
}

string chestnut_utf8_to_ansi(const string& str)
{
	//utf8 --> unicode
	wstring resultTmp;
	int len = ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.size(), NULL, 0);
	resultTmp.assign(len, 0);
	wchar_t* p = (wchar_t*)resultTmp.data();
	::MultiByteToWideChar(CP_UTF8, NULL, str.data(), str.size(), p, len);

	//unicode --> ansi
	string result;
	len = ::WideCharToMultiByte(CP_ACP, NULL, resultTmp.c_str(), str.size(), NULL, 0, NULL, NULL);
	result.assign(len, 0);
	char* pResult = (char*)result.data();
	::WideCharToMultiByte(CP_ACP, NULL, resultTmp.data(), str.size(), pResult, len, NULL, NULL);

	return result;
}

string chestnut_unicode_to_utf8(const wstring& str)
{
	// unicode --> utf8
	string result;
	int len = ::WideCharToMultiByte(CP_UTF8, NULL, str.c_str(), str.size(), NULL, 0, NULL, NULL);
	result.assign(len, 0);
	char* p = (char*)result.data();
	::WideCharToMultiByte(CP_UTF8, NULL, str.data(), str.size(), p, len, NULL, NULL);

	return result;
}

wstring chestnut_utf8_to_unicode(const string& str)
{
	// utf8 --> unicode
	wstring result;
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