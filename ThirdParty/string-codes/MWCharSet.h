#pragma once

#include "MWAllocObj.h"

namespace Myway
{

class MW_ENTRY CharSet : public AllocObj
{
public:
    CharSet() {}
    ~CharSet() {}

    static int AnsiToUnicode(wchar_t * o, int size, const char * i);
    static int UnicodeToAnsi(char * o, int size, const wchar_t * i);
    static int Utf8ToAnsi(char * o, int size, const char * i);
    static int Utf8ToUnicode(wchar_t * o, int size, const char * i);

};

}