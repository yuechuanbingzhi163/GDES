#pragma once

/*
 * 字段及数据初始化辅助类
 * 从文件中读取字段和数据
 */
class DataInitHelper
{
public:
	static void InitDataField(const CString& fileName);

	static void InitStrList(const CString& fileName);
	static void InitIntStrList(const CString& fileName);
	static void InitDataObjectList(const CString& fileName);
	static void InitSingleDataObject(const CString& fileName);
};
