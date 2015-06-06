#pragma once

#include "acui.h"
#include "resource.h"

class PropertyDataBaseDlg : public CAcUiDialog 
{
	DECLARE_DYNAMIC (PropertyDataBaseDlg)

protected:
	PropertyDataBaseDlg(UINT idd, CWnd *pParent =NULL, HINSTANCE hInstance = NULL) ;
	~PropertyDataBaseDlg();

	virtual void readPropertyData() {}
	virtual void writePropertyData() {}
	
public:
	AcDbObjectId m_objId;

protected:
	DECLARE_MESSAGE_MAP()
} ;
