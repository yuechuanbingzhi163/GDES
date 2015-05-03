#pragma once


// Required for VS 2008 (fails on XP and Win2000 without this fix)
//#ifndef _WIN32_WINNT
//  #define _WIN32_WINNT 0x0500
//#endif

//#define STRICT

#define WIN32_LEAN_AND_MEAN	
#define _CRT_SECURE_NO_DEPRECATE

//#ifndef VC_EXTRALEAN
//#define VC_EXTRALEAN			//- Exclude rarely-used stuff from Windows headers
//#endif

//- Modify the following defines if you have to target a platform prior to the ones specified below.
//- Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER					//- Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0500		//- Change this to the appropriate value to target Windows 2000 or later.
#endif

#include <windows.h>
#include <objbase.h>

#include <UiLib\UIlib.h>

using namespace UiLib;

#include "resource.h"

//#include "vld.h"
