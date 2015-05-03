#ifndef SKIN_CHANGE_EVENT_HPP
#define SKIN_CHANGE_EVENT_HPP

#include "observer_impl_base.hpp"

struct SkinChangedParam
{
	DWORD bkcolor;
	CDuiString bgimage;
};

typedef class QQ::ObserverImpl<BOOL, SkinChangedParam> SkinChangedObserver;
typedef class QQ::ReceiverImpl<BOOL, SkinChangedParam> SkinChangedReceiver;


#endif // SKIN_CHANGE_EVENT_HPP