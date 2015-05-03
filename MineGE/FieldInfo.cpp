#include "StdAfx.h"
#include "FieldInfo.h"

FieldInfo::FieldInfo()
{
    initDefault();
}

void FieldInfo::initDefault()
{
    m_dt = DT_STRING;
    m_enable = true;
    m_descr = _T( "" );
	m_tolrance = 4;
    m_showInTooltip = false;
    initListType();
    initIntMinMaxValue();
    initNumericMinMaxValue();
}

void FieldInfo::copyFrom( const FieldInfo& info )
{
    m_dt = info.m_dt;
    m_minValue2 = info.m_minValue2;
    m_maxValue2 = info.m_maxValue2;
    m_minValue = info.m_minValue;
    m_maxValue = info.m_maxValue;
    m_enable = info.m_enable;
    m_descr = info.m_descr;
    m_lt = info.m_lt;
    m_varName = info.m_varName;
	m_tolrance = info.m_tolrance;
    m_showInTooltip = info.m_showInTooltip;
}

void FieldInfo::initNumericMinMaxValue()
{
    m_minValue = 0;
    m_maxValue = 0;
}

void FieldInfo::initIntMinMaxValue()
{
    m_minValue2 = 0;
    m_maxValue2 = 0;
}

void FieldInfo::initListType()
{
    m_lt = LT_STRING;
    m_varName = _T( "" );
}

void FieldInfo::revise()
{
    switch( m_dt )
    {
    case DT_STRING:
    case DT_BOOL:
    case DT_DATE:
        initListType();
        initIntMinMaxValue();
        initNumericMinMaxValue();
        break;
    case DT_INT:
        initListType();
        initNumericMinMaxValue();
        break;
    case DT_NUMERIC:
        initListType();
        initIntMinMaxValue();
        break;
    case DT_LIST:
        initIntMinMaxValue();
        initNumericMinMaxValue();
        break;
    }
}

bool FieldInfo::isDefault() const
{
    return ( ( m_dt == DT_STRING ) && m_enable && ( m_descr.GetLength() == 0 ) && !m_showInTooltip );
}

bool FieldInfo::isObjectListType() const
{
    return ( ( m_dt == DT_LIST ) &&
             ( m_lt == LT_OBJECT ) &&
             ( m_varName.GetLength() != 0 ) );
}