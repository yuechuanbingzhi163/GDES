#pragma once

#include <UiLib\UIlib.h>
using namespace UiLib;

#include <string>
#include <map>
using namespace std;

#include "resource.h"
#include "ControlNames.h"

#ifdef _UNICODE
typedef wstring tstring;
#else
typedef string	tstring;
#endif

/*************************************************/

#include "Utils\FileUtil.h"
#include "Utils\WndShadow.h"

/*************************************************/
#include "zplay\libzplay.h"
#include "zplay\MusicPlayer.h"

#include "wke\wke.h"
#include "wke\WkeWebkit.h"

#include "Controls\PopupWidget.h"
#include "Controls\UIRadioList.h"
#include "Controls\UIMusicList.h"
#include "Controls\\UIColorPalette.h"
#include "Controls\UIHotButton.h"
#include "Controls\SkinPickerPictureItem.h"
//#include "Controls\CreateControlEx.h"
//#include "Controls\CustomWebEventHandler.h"
//#include "Controls\UIMenu.h"

/*************************************************/
#include "MessageDialog.h"
#include "MusicInfoDialog.h"
#include "FrameWnd.h"
#include "SettingDialog.h"
#include "PalletDialog.h"
#include "SkinPickerDialog.h"
#include "DropTargetEx.h"

#include <Wininet.h>
#pragma  comment(lib, "Wininet.lib")