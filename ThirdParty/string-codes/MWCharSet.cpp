#include <windows.h>
#include "MWCharSet.h"

namespace Myway
{

int CharSet::AnsiToUnicode(wchar_t * o, int size, const char * i)
{
    int len = MultiByteToWideChar(CP_ACP, 0, i, -1, NULL, 0);

    if (o == NULL)
        return len;

    len = MultiByteToWideChar(CP_ACP, 0, i, -1, o, len);

    return len;
}

int CharSet::UnicodeToAnsi(char * o, int size, const wchar_t * i)
{
    int len = WideCharToMultiByte(CP_ACP, 0, i, -1, NULL, 0, NULL, NULL);

    if (o == NULL)
        return len;

    len = WideCharToMultiByte(CP_ACP, 0, i, -1, o, len, NULL, NULL);

    return len;
}

int CharSet::Utf8ToAnsi(char * o, int size, const char * i)
{
    wchar_t buffer[2048];

    // convert to wide char
    int len = MultiByteToWideChar(CP_UTF8, 0, i, -1, NULL,0);

    assert (len < 2048);

    MultiByteToWideChar(CP_UTF8, 0, i, -1, buffer, len);

    // convert to ansi
    len = WideCharToMultiByte(CP_ACP, 0, buffer, -1, NULL, 0, NULL, NULL);

    if (o == NULL)
        return len;

    assert (len < size);

    len = WideCharToMultiByte(CP_ACP, 0, buffer, -1, o, len, NULL, NULL);

    return len;
}

int CharSet::Utf8ToUnicode(wchar_t * o, int size, const char * i)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, i, -1, NULL,0);

    if (o == NULL)
        return len;

    assert (len < size);

    len = MultiByteToWideChar(CP_UTF8, 0, i, -1, o, len);

    return len;
}

}