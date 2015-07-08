#pragma  once
#include "../MineGE/DataListHelper.h"
#include "../MineGE/DataHelper.h"

#include <iostream>
#include <fstream>
using namespace std;

//AcFStream.h一定要放在iostream，fstream等标准库头文件的后面!!!(切记)
#include "AcFStream.h"

#define ISNULL _T("**")

//初始化全局变量(new构造对象，并初始化com)
extern bool initword();
//卸载全局变量(释放内存，释放com)
extern void uninitword();
//生成报告
extern bool CreatReport(const CString& savePath,const AcDbObjectId& drill_site);

extern void OpenWordDocument( const CString& filePath );
/**
修改的地方：
1、将wordOperate.cpp里的CmyWord MyWord改成CmyWord* MyWord;
2、我个人认为要手动初始化CmyWord* MyWord全局变量
   将CmyWord* MyWord的声明迁移到CmyWord.h，定义放在CmyWord.cpp里
   -->参见CmyWorld.h尾部的注释说明!!!
3、将wordOperate.cpp里的代码都复制到CreateReport.cpp里，并删除wordOperate.h和wordOperate.cpp文件
4、在CreateReport的头文件里增加2个全局函数，用于初始化和卸载com(initword和uninitword)
   不建议在CmyWord的构造函数里调用CoInitialize()函数!
   -->参见CreateReport.h和CreateReport.cpp中的函数实现!!
5、修改了CreatReport()函数的签名，去掉第一个模版文件参数
   -->参见CreateReport.h CreateReport.cpp acrxEntryPoint.cpp中的WordTest_ARXWORD()函数!!
*/
