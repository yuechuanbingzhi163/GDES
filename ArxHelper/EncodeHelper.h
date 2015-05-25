#pragma once

#include "dlimexp.h"

#include <string>

/** 
* ×Ö·û´®±àÂë×ª»».
*/
class ARXHELPER_DLLIMPEXP EncodeHelper
{
public:
	std::string UnicodeToANSI(const std::wstring& str);
	std::wstring ANSIToUnicode(const std::string& str);
	std::string ANSIToUtf8(const std::string& str);
	std::string Utf8ToANSI(const std::string& str);
	std::string UnicodeToUtf8(const std::wstring& str);
	std::wstring Utf8ToUnicode(const std::string& str);
};
