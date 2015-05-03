#pragma once
#include "dlimexp.h"

// 数据对象
class MINEGE_DLLIMPEXP DataObject : public AcDbObject {

public:
	ACRX_DECLARE_MEMBERS(DataObject) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	DataObject () ;
	virtual ~DataObject () ;

	// 获取图元id
	AcDbObjectId getGE() const;

	// 设置图元id
	void setGE(const AcDbObjectId& objId);

	// 设置类型名称
	bool setType(const CString& type);

	// 获取类型名称
	CString getType() const;

	// 根据类型名称初始化数据
	// 清空所有字段，并添加空字符串到数据中
	void initData();

	// 获取数据, pos为0开始的索引值
	bool getData(int pos, CString& value) const;

	// 设置数据, pos为0开始的索引值
	bool setData(int pos, const CString& value);

	// 增加一个数据
	void addData();

	// 删除指定位置
	void removeData(int pos);

	// 清空
	void clearAll();

	// 内部使用
	void getAllData(AcStringArray& values);
	void setAllData(const AcStringArray& values);

	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const ;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) ;

private:
	bool isValidIndex(int pos) const;

private:
	CString m_type;          // 类型名称
	AcStringArray m_values;  // 数据值
	AcDbObjectId m_objId;    // 关联的图元(如果为null, 表示是一个纯字符串映射的数据对象)
	                         // 软指针引用(Soft Pointer),这样deepClone和wbloclClone都不会处理这个id
} ;

#ifdef MINEGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DataObject)
#endif
