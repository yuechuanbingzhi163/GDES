#pragma once

#include "dlimexp.h"
#include "ConstData.h"

/*
 * 数据链接基类
 * 可以关联数据对象、MineGE及派生、ModelGE及派生
 *		1) 关联数据对象，可以关联所有的数据，但没有几何图形信息
 *		2) 关联图元(MineGE/ModelGE)，在关联数据的同时，也可以进行图形操作
 *			例如删除、着色、切换图层、修改/获取坐标等
 */
class MINEGE_DLLIMPEXP DataLink
{
public:
	virtual ~DataLink(void);

	/*
	 * 设置数据源
	 */
	void setDataSource(const AcDbObjectId& objId);

	/*
	 * 获取数据源
	 */
	AcDbObjectId getDataSource() const;

	/*
	 * 读取或更新数据
	 * 参数：
	 *    save -- false表示将数据更新到变量
	 *            true表示将变量更新到数据对象
	 */
	bool updateData(bool save=false);

protected:
	DataLink();
	virtual void regDatas() = 0;
	void linkStringData(const CString& field, CString* pValue);
	void linkIntData(const CString& field, int* pValue);
	void linkDoubleData(const CString& field, double* pValue);
	void linkBoolData(const CString& field, bool* pValue);
	void linkDateTimeData(const CString& field, COleDateTime* pValue);

private:
	void initDatas();
	bool checkFields();
	// 数据格式：字段名称->字段类型->映射的变量指针->...
	resbuf* m_pDatasToFileds;
	AcDbObjectId m_objId;
};
