#pragma once

/*
 * 可视化界面辅助类
 * 放在一个类中便于管理
 * 也可使得代码看起来简洁一些
 */
class UIHelper
{
public:
	static void ShowDrawManagerDlg();    // 显示图元可视化效果配置/切换对话框
	static void ShowFieldManagerDlg();   // 显示字段管理对话框
	//static void ShowPropertyDataDlg();
	static void ShowVarListDlg();        // 显示变量列表管理对话框
	static void ShowTypeNameMapDlg();    // 显示类型名称映射对话框
	static void ShowDataObjectStateDlg();// 显示数据对象管理对话框
};
