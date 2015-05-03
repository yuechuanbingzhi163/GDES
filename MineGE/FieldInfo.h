#pragma once

#include "dlimexp.h"

#include "FieldType.h"

// 字段信息
class MINEGE_DLLIMPEXP FieldInfo
{
public:
	FieldInfo();
	
	/*
	 * 默认设置
	 * 以下3个条件必须同时满足
	 *     1) m_dt = DT_STRING
	 *     2) m_enable = true
	 *     3) m_descr = _T("")
	 */
	bool isDefault() const;
	bool isObjectListType() const;
	void revise();                     //  校正属性设置
	void copyFrom(const FieldInfo& info);

public:

	DATA_TYPE m_dt;                    // 默认为字符串类型(DT_STRING)
	
	int m_minValue2, m_maxValue2;      // 整数区间

	double m_minValue, m_maxValue;     // 浮点数区间

	LIST_TYPE m_lt;                    // 列表类型
	CString m_varName;                 // 列表变量名称
		
	bool m_enable;                     // 默认属性可编辑(true)
	CString m_descr;                   // 简要说明

	int m_tolrance;						//精度

	bool m_showInTooltip;              // 是否在tooltip中显示

private:
	void initDefault();                // 恢复默认设置
	void initIntMinMaxValue();         // 设置整型数默认值m_minValue2=m_maxValue2=0
	void initNumericMinMaxValue();     // 设置浮点数默认值m_minValue=m_maxValue=0
	void initListType();               // 设置列表默认设置m_lt=DT_LIST m_varName=_T("")
};
