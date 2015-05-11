#pragma once

class CFormatMsgDlg : public SOUI::SHostDialog
{
public:
    CFormatMsgDlg(void);
    ~CFormatMsgDlg(void);
    
    void OnOK();
    
	SOUI::SStringT m_strMsg;
    
    EVENT_MAP_BEGIN()
        EVENT_ID_COMMAND(IDOK,OnOK)
    EVENT_MAP_END()
        
};

