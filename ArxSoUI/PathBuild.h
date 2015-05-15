#pragma once

#include <iostream>
#include <fstream>
using namespace std;
#include "AcFStream.h"

extern CString GetAppPathDir();

extern CString BuildPath( const CString& dir, const CString& fileName );