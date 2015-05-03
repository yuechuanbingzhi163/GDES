#pragma once

#include "dlimexp.h"

typedef AcArray<CMFCPropertyGridProperty*> AcPropertyArray;
typedef double (*SyncFunc)(AcPropertyArray& properties);

//属性同步计算类
//通过派生重载实现虚函数caculate()来计算不同的属性值
class MINEGE_DLLIMPEXP SyncFunctor
{
public:
	SyncFunctor(AcPropertyArray& pProps);
	//纯虚函数
	virtual double caculate() = 0;

protected:
	//要同步计算的有关联的属性对象
	//用户需要自己构造这个数组，将关联的属性对象按照顺序添加进来
	AcPropertyArray& m_pProps;
};

//所有自定义属性的基类
class MINEGE_DLLIMPEXP CustomGridProperty : public CMFCPropertyGridProperty
{
public:
	CustomGridProperty(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL);
	~CustomGridProperty();
	//高亮显示切换
	void highlight(bool bHighlight = true);
	//设置同步对象
	void setSyncFun(SyncFunctor* fn);

protected:
	//高亮绘制属性值
	virtual void OnDrawValue(CDC* pDC, CRect rect);
	//高亮绘制属性名字(未实现)
	virtual void OnDrawName(CDC* pDC, CRect rect);
	//双击同步计算属性值
	virtual BOOL OnDblClk(CPoint point);

protected:
	//是否高亮
	bool m_bHighlight;
	//属性名字和属性值的矩形框大小
	CRect m_valueRect, m_nameRect;
	//同步对象指针(由用户指定)
	SyncFunctor* m_fn;
};

// 整数属性
class MINEGE_DLLIMPEXP CIntProp : public CustomGridProperty
{
public:
	CIntProp(const CString& strName, const COleVariant& varValue, int nMinValue, int nMaxValue, LPCTSTR lpszDescr = NULL);

	virtual BOOL OnUpdateValue();
private:
	int m_nMinValue;
	int m_nMaxValue;
};

// 浮点数属性
class MINEGE_DLLIMPEXP CNumericProp : public CustomGridProperty
{
public:
	CNumericProp(const CString& strName, const COleVariant& varValue, double dMinValue, double dMaxValue, unsigned short precise, LPCTSTR lpszDescr = NULL);

	virtual BOOL OnUpdateValue();
	virtual CString FormatProperty();

private:
	double m_dMinValue;
	double m_dMaxValue;
	unsigned short m_precise; // 小数点精度
};

class MINEGE_DLLIMPEXP IntStrProp : public CustomGridProperty
{
public:
	IntStrProp(const CString& name, int value, const AcDbIntArray& intValues, const AcStringArray& strValues, LPCTSTR lpszDescr = NULL);
	virtual BOOL OnUpdateValue();
	virtual CString FormatProperty();

private:
	CString Int2Str(int intValue);
	int Str2Int(const CString& strValue);

	//int m_value;
	AcDbIntArray m_intValues;
	AcStringArray m_strValues;
};

class MINEGE_DLLIMPEXP DateTimeProperty : public CustomGridProperty
{
public:
	DateTimeProperty(const CString& strName, const COleDateTime& timeSrc, LPCTSTR lpszDescr = NULL);

	virtual CString FormatProperty();
	virtual BOOL OnUpdateValue();

protected:
	virtual CWnd* CreateInPlaceEdit(CRect rectEdit, BOOL& bDefaultFormat);
	virtual void OnDrawDescription(CDC* pDC, CRect rect);
};

class MINEGE_DLLIMPEXP MyDateTimeCtrl : public CDateTimeCtrl
{
public:
	MyDateTimeCtrl(	DateTimeProperty* pProp) : m_pProp(pProp) {}

	DECLARE_MESSAGE_MAP()
	afx_msg void OnDtnDatetimechange(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnNMKillfocus(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnDtnCloseup(NMHDR *pNMHDR, LRESULT *pResult);
	// 使用NM_KILLFOCUS和DTN_CLOSEUP这2个消息也能够达成目标

protected:
	DateTimeProperty* m_pProp;
};


class MINEGE_DLLIMPEXP DataObjectProperty : public CustomGridProperty
{
public:
	DataObjectProperty(const CString& name, const CString& value, const AcStringArray& strValues, LPCTSTR lpszDescr = NULL);

//protected:
//	virtual BOOL HasButton() const { return TRUE; } // 对于list之类的属性无效，并没有显示一个...按钮
//	virtual void OnClickButton(CPoint point) { AfxMessageBox(_T("helllo")); }
};
// 还有一个ClickProp，参见MFCPropertyGridCtrlHelper.cpp末尾部分