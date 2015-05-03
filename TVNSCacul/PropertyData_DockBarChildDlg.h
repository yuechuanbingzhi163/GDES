#pragma once

#include "DockBarChildDlg.h"

class PropertyData_DockBarChildDlg : public DockBarChildDlg 
{
	DECLARE_DYNAMIC (PropertyData_DockBarChildDlg)

protected:
	PropertyData_DockBarChildDlg(UINT idd, CWnd *pParent =NULL, HINSTANCE hInstance = NULL) ;
	~PropertyData_DockBarChildDlg();

	virtual void readPropertyData() {}
	virtual void writePropertyData() {}
	virtual void initMoreChildDlg() {}
	
public:
	AcDbObjectId m_objId;
	void updatePropertyData(bool save);

protected:
	DECLARE_MESSAGE_MAP()
} ;
