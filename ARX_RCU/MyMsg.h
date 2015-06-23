#pragma once

/**
* 自定义消息参考资料:
	http://blog.csdn.net/code_robot/article/details/7724667
	http://blog.csdn.net/penpenandtongtong/article/details/18598907
	用PostMessage传参数
	http://blog.csdn.net/tian_jinping/article/details/23264649
*/

//增加图元
#define WM_RCU_ADD WM_USER+1000
//删除图元
#define WM_RCU_DEL WM_USER+1001
//修改图元
#define WM_RCU_MODIFY WM_USER+1002
//刷新对话框
#define WM_RCU_UPDATE WM_USER+1003

//自定义消息参数
struct ArxMsg
{
	CString clsName;     // 类型名称
	AcDbObjectId objId;  // 图元ID
};