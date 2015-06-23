#pragma once

/**
* 自定义消息参考资料:
	http://blog.csdn.net/code_robot/article/details/7724667
	http://blog.csdn.net/penpenandtongtong/article/details/18598907
*/

//增加图元
#define WM_ADD_GE WM_USER+1000
//删除图元
#define WM_DEL_GE WM_USER+1001
//修改图元
#define WM_CHANGE_GE WM_USER+1002

//自定义消息参数
struct ArxMsg
{
	CString clsName;     // 类型名称
	AcDbObjectId objId;  // 图元ID
};