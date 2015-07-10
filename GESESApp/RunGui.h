#ifndef RUN_GUI_H
#define RUN_GUI_H

#include  <windows.h>
#include  <tchar.h>
#include  <QDialog>
#include "RegHelper.h"

//#include "FileOpt.h"
#include "HardWare.h"
#include "Reg.h"
#include "RegTbOpt.h"
#define MAX_BUFF 255

class QTimer;
class QLabel;
class QPushButton;

class   RunGui : public   QDialog
{
	Q_OBJECT
public :
	RunGui ();
	~ RunGui ();

private slots:
	void startCADNull();
	void startCADByFile();
	void about();
	void updateTime();

protected:
	void closeEvent(QCloseEvent *event);  

private :
	void paintEvent(QPaintEvent *e);
	void getTime();
	void initDialog();
	void setBtnStyle();
	void cheakRegCode();

	QString lastPath;
	QTimer *myTimer;
	QString sTime; //当前时间
	QLabel *timeLabel;
	//QString windTitle;

	QPushButton *startNullBtn;
	QPushButton *startFileBtn;
	QPushButton *helpBtn;

	RegHelper *regHelper;
	bool m_registed;

protected:
	// 初使化注册, 包括判断是否注册、是否有异常情况等等
	// 返回值: 1、 未注册、ini文件不存在、或注册表中无值
	//         2、 硬盘ID与用户ID不符
	//         3、 ini文件中的值与注册表中加密的使用次数值不符合
	//         4、 已注册, 但使用次数已到

	UINT InitRegProc();

	// 未注册、ini文件不存在、或注册表中无值
	BOOL IsErrorOne();

	// 硬盘ID与用户ID不符
	BOOL IsErrorTwo();

	// ini文件中的值与注册表中加密的使用次数值不符合
	BOOL IsErrorThree();

	// 如果使用次数已到
	BOOL IsErrorFour();

	// 显示提示信息、以及注册对话框
	bool ShowRegDlg();

	// 处理注册函数
	void ProcReg();

public:
	CHardWare   m_hWare;     // 硬件信息操作类
	CRegTbOpt   m_regOpt;    // 注册表操作类
	//CFileOpt    m_fileOpt;   // 文件操作类
	CReg        m_reg;       // 注册码操作类
	//CRegDlg     m_regDlg;

	int         m_nIniSelect;           // 从INI文件中读出的选择次数的索引
	int         m_nIniUTime;            // INI文件中读出的剩余使用次数
	int         m_nRegUTime;            // 注册表中读出的剩余使用次数

	TCHAR        m_chWinPath[MAX_BUFF];  // 存放Easson2d.ini文件全路径
	DISKINFO    m_dInfo;                // 存放硬件信息
	CString     m_strUID;               // 存用户名(数字化后的)

	UINT        m_nErrorCode;           // 在注册程序初使化出错时,接收错误类型
	BOOL        m_bFirstReg;            // 是否是第一次注册(默认为FALSE)

};
#endif //RUN_GUI_H