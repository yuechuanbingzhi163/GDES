#include "StdAfx.h"
#include "SListCtrl2.h"

void SListCtrl2::OnLButtonDblClick(UINT nFlags, SOUI::CPoint pt)
{
	__super::OnLButtonDown(nFlags,pt);
	m_nHoverItem = HitTest(pt);
	//BOOL hitCheckBox = HitCheckBox(pt);

	EventLCDblClick evt2(this);
	evt2.nCurSel = m_nHoverItem;
	FireEvent(evt2);
}

void SListCtrl2::OnRButtonDown(UINT nFlags, SOUI::CPoint pt)
{
	__super::OnRButtonDown(nFlags,pt);
}