#include "CADHelper.h"
#include <qt_windows.h>
#include <QSettings>
#include <QMessageBox>

#include "Registry.h"
#include "RegEntry.h"
#include <tchar.h>

#include "eoLogger.h"


//#include <psapi.h >
//#pragma comment(lib,"psapi.lib")

//检查进程是否存在
#include <tlhelp32.h> 
//返回 -1 没有找到
//返回  0 找到自身
//返回  1 找到除自身外的另一个

int FindProcess(LPCTSTR appName)  
{  
	int i=0;  
	PROCESSENTRY32 pe32;  
	pe32.dwSize = sizeof(pe32);   
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);  
	if(hProcessSnap == INVALID_HANDLE_VALUE)  
	{  
		i+=0;  
	}  
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);  
	while(bMore)  
	{  
		//printf(" 进程名称：%s \n", pe32.szExeFile);  
		if(stricmp(appName,pe32.szExeFile)==0)  
		{  
			//printf("进程运行中");  
			i+=1;  
		}  
		bMore = ::Process32Next(hProcessSnap, &pe32);  
	}  
	if(i>1)//大于1，排除自身  
	{           
		return 1;  
	}

	else if( i > 0 && i < 2 )
	{
		return 0;
	}

	else
	{  
		return -1;  
	}  
}  
static void msbbox(const QString& msg)
{
	QMessageBox msgBox;
	msgBox.setText(msg);
	msgBox.exec();
}
bool copyFileToPath( QString& sourceDir ,QString& toDir, bool coverFileIfExist )
{
	sourceDir.replace("/","\\");
	toDir.replace("/","\\");
	if (sourceDir == toDir)
	{ 
		return true; 
	} 
	bool ret1 = QFile::exists(sourceDir);
	bool ret2 = QFile::exists(toDir);
	if (!ret1 || !ret2)
	{ 
		//msbbox("文件不存在");
		//msbbox(sourceDir);
		//msbbox(toDir);
		return false; 
	} 
	QDir *createfile = new QDir; 
	bool exist = createfile->exists(toDir); 
	if (exist)
	{ 
		if(coverFileIfExist)
		{ 
			createfile->remove(toDir); 
		} 
	}//end if 

	if(!QFile::copy(sourceDir, toDir)) 
	{ 
		//msbbox("yyy");
		return false; 
	} 
	//msbbox("最后");
	//eo::log << sourceDir.toStdString().c_str() << "->文件拷贝成功->" << toDir.toStdString().c_str() << std::endl;
	return true; 
}

bool copyCadFile()
{
	QDir lsDir;
	//QString cadPath = lsDir.homePath();
	//cadPath.append("/AppData/Roaming/Autodesk/AutoCAD 2010/R18.0/chs/Support");
	
	QString cadPath = getCADPath("RoamableRootFolder",CURRENT_USER);
	cadPath.append("Support");
	lsDir.setPath(cadPath);

	//filepath == "C:\Users\Username\AppData\Roaming\Autodesk\AutoCAD 2010\R18.0\chs\Support"
	QString filepath = lsDir.filePath("acad.CUIX");
	//filepath == "C:\Users\Username\AppData\Roaming\Autodesk\AutoCAD 2010\R18.0\chs\Support\hd.txt"

	//获取当前程序(.exe)所在的路径
	QString CurrentPath = lsDir.currentPath();

	QString fromDir = CurrentPath;
	fromDir.append("\\Datas\\JL\\acad.CUIX");
	//文件来自:当前程序所在位置+"/Datas/JL/hd.txt"

	QString toDir = filepath;
	
	//QString toDir = testPath;
	//QMessageBox msgBox;
	//msgBox.setText(toDir);
	//msgBox.exec();
	if(!copyFileToPath(fromDir,toDir,true)) return false;

	filepath = lsDir.filePath("acad.mnr");
	CurrentPath = lsDir.currentPath();
	fromDir = CurrentPath;
	fromDir.append("\\Datas\\JL\\acad.mnr");
	toDir = filepath;
	return copyFileToPath(fromDir,toDir,true);
}

bool recorverCadFile()
{
	QDir lsDir;
	//QString cadPath = lsDir.homePath();
	//cadPath.append("/AppData/Roaming/Autodesk/AutoCAD 2010/R18.0/chs/Support");
	QString cadPath = getCADPath("RoamableRootFolder",CURRENT_USER);
	cadPath.append("/Support");
	lsDir.setPath(cadPath);

	//filepath == "C:\Users\Username\AppData\Roaming\Autodesk\AutoCAD 2010\R18.0\chs\Support"
	QString filepath = lsDir.filePath("acad.CUIX");
	//filepath == "C:\Users\Username\AppData\Roaming\Autodesk\AutoCAD 2010\R18.0\chs\Support\hd.txt"

	//获取当前程序(.exe)所在的路径
	QString CurrentPath = lsDir.currentPath();

	QString fromDir = CurrentPath;
	fromDir.append("/Datas/CAD/acad.CUIX");
	//文件来自:当前程序所在位置+"/Datas/JL/hd.txt"

	QString toDir = filepath;
	if(!copyFileToPath(fromDir,toDir,true)) return false;

	filepath = lsDir.filePath("acad.mnr");
	CurrentPath = lsDir.currentPath();
	fromDir = CurrentPath;
	fromDir.append("/Datas/CAD/acad.mnr");
	toDir = filepath;
	return copyFileToPath(fromDir,toDir,true);
}

QString GetCADPathByQt( QString locationKey ,pathType pat)
{
	QString path;
	if(CURRENT_USER == pat)
	{
		//path = "HKEY_CURRENT_USER\\SOFTWARE\\AutoDesk\\AutoCAD";
		path = "HKEY_CURRENT_USER\\Software\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:804";
	}
	else if(LOCAL_MACHINE == pat)
	{
		//path = "HKEY_LOCAL_MACHINE\\SOFTWARE\\AutoDesk\\AutoCAD";
		path = "HKEY_LOCAL_MACHINE\\Software\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:804";
	}
	QSettings *settings = new QSettings( path, QSettings::NativeFormat);
	//QStringList key = settings->allKeys();
	//int num = (int)key.size();
	QString cadPath = settings->value(locationKey).toString();
	delete settings;
	settings = NULL;
	return cadPath;
}

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
BOOL IsWow64() 
{ 
	BOOL bIsWow64 = FALSE; 
	LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress( 
		GetModuleHandle(TEXT("kernel32")),"IsWow64Process"); 
	if (NULL != fnIsWow64Process) 
	{ 
		if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64)) 
		{ 
			// handle error 
			//AfxMessageBox(_T("IsWow64 error!")); 
		} 
	}
	return bIsWow64; 
}

QString GetCADPathByWinAPI( QString locationKey ,pathType pat)
{
	QString cadPath;

	TCHAR cadKey[MAX_PATH];
	_tcscpy(cadKey, _T("Software\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:804"));

	HKEY hRoot = (CURRENT_USER == pat)?HKEY_CURRENT_USER:HKEY_LOCAL_MACHINE;
	CRegistry reg;

	 DWORD dwAccess = KEY_ALL_ACCESS;
	 //判断是否64位
	 if(IsWow64())
	 {
		 dwAccess |= KEY_WOW64_64KEY;
	 }
	bool ret = reg.Open( cadKey, hRoot, dwAccess);
	if(ret)
	{
		ret = false;
		int n = reg.Count();
		for(int i=0;i<n;i++)
		{
			CRegEntry* entry = reg.GetAt(i);
			if(locationKey.compare(entry->lpszName, Qt::CaseInsensitive) == 0)
			{
				cadPath = QString::fromStdString((tstring)*entry);
				ret = true; break;
			}
		}
		//SubKeyExists不好使!
		//std::string name = locationKey.toStdString();
		//ret = reg.SubKeyExists(name.c_str());
		//if(ret)
		//{
		//	cadPath = QString::fromStdString((std::string)reg[name.c_str()]);
		//}
	}
	reg.Close();

	return cadPath;
}

QString getCADPath( QString locationKey ,pathType pat)
{
//#ifdef _WIN64
	return GetCADPathByWinAPI(locationKey, pat);
//#else
	//return GetCADPathByQt(locationKey, pat);
//#endif
}

bool isAutoCADExist( QString& cadPath,const QStringList& argument)
{
	bool isExist = false;
	//CAD的安装目录
	cadPath = getCADPath("AcadLocation",LOCAL_MACHINE);
	if(cadPath.isEmpty()) 
	{
		cadPath = QObject::tr("Not install AutoCAD");
		QMessageBox msg;
		msg.setWindowTitle(QObject::tr("tip"));
		msg.resize(400,300);
		msg.setText(cadPath);
		msg.exec();
		isExist = false;
		return false;
	}

	else isExist = true;
	//cadPath.append("\\acad.exe");
	//cadPath.replace("\\","/");

	return isExist;
}

bool writeTestKeybyWinAPI(QString& cadPath)
{
	TCHAR cadKey[MAX_PATH];
	_tcscpy(cadKey, _T("Software\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:804\\Applications\\test"));

	DWORD dwAccess = KEY_ALL_ACCESS;
	//判断是否64位
	if(IsWow64())
	{
		dwAccess |= KEY_WOW64_64KEY;
	}

	CRegistry reg;
	bool ret = reg.Open( cadKey, HKEY_LOCAL_MACHINE, dwAccess);
	if(ret)
	{
		//if(reg.Count()>0)
		//{
			reg[_T("LOADCTRLS")] = (DWORD)2;
			cadPath.append("\\VVLoader.arx");
			reg[_T("LOADER")] = cadPath.toStdString();
		//}
	}
	reg.Close();

	return ret;
}

bool writeInfomationKeybyWinAPI(QString& cadPath)
{
	TCHAR cadKey[MAX_PATH];
	_tcscpy(cadKey, _T("Software\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:804\\InfoCenter"));

	DWORD dwAccess = KEY_ALL_ACCESS;
	//判断是否64位
	if(IsWow64())
	{
		dwAccess |= KEY_WOW64_64KEY;
	}

	CRegistry reg;
	bool ret = reg.Open( cadKey, HKEY_CURRENT_USER, dwAccess);
	if(ret)
	{
		ret = false;
		if(reg.Count()>0)
		{
			int n = reg.Count();
			for(int i=0;i<n;i++)
			{
				CRegEntry* entry = reg.GetAt(i);
				if(QString::compare("InfoCenterOn", entry->lpszName, Qt::CaseInsensitive) == 0)
				{
					*entry = 0;
					ret = true; break;
				}
			}
		}
	}
	reg.Close();

	return ret;
}

bool writeRegByWinAPI( QString& cadPath)
{
	if(!writeTestKeybyWinAPI(cadPath)) return false;
	if(!writeInfomationKeybyWinAPI(cadPath)) return false;
	return true;
}

void writeRegByQt( QString& cadPath)
{
/*2014.9.14--hd--hld--静远233
************************************************************************************
	问题:此处获得的是CAD的安装目录,为什么用CAD安装目录，还不用拷贝文件到安装目录？ *
	这个文件是怎么搜索的？？？用arx文件所在的目录为什么不行？？				       *
	解决过程:因为CAD是由RunGui.exe启动的，RunGui.exe所在的目录就是cad的工作目录。  *
	只要这个目录有需要加载的文件,不管注册表写的是CAD的安装目录还是文件所在的目录， *
	启动的时候都会加载文件。     												   *
	最终解决:说到底，只要"LOADCTRLS"的值是VVLoader.arx就可以了，然后满足启动目录下 *
	有VVLoader.arx这个文件，一些都正常了。                                         *
************************************************************************************
*/
	//arx文件的自动加载
	QSettings *reg = new QSettings("HKEY_LOCAL_MACHINE\\Software\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:804\\Applications\\test",
		QSettings::NativeFormat);
	if(!reg->allKeys().isEmpty()) return;
	reg->setValue("LOADCTRLS",2);
	cadPath.append("\\VVLoader.arx");
	reg->setValue("LOADER",cadPath);
	delete reg; 

	//标题搜索栏的去除
	QSettings *regFind = new QSettings("HKEY_CURRENT_USER\\Software\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:804\\InfoCenter",
		QSettings::NativeFormat);
	if(regFind->contains("InfoCenterOn"))
	{
		regFind->setValue("InfoCenterOn",0);
	}
	delete regFind;
}

void writeReg( QString& cadPath)
{
	writeRegByWinAPI(cadPath);
	//writeRegByQt(cadPath);
}

bool delTestKeybyWinAPI()
{
	TCHAR cadKey[MAX_PATH];
	_tcscpy(cadKey, _T("Software\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:804\\Applications\\test"));

	DWORD dwAccess = KEY_ALL_ACCESS;
	//判断是否64位
	if(IsWow64())
	{
		dwAccess |= KEY_WOW64_64KEY;
	}

	CRegistry reg;
	bool ret = reg.Open( cadKey, HKEY_LOCAL_MACHINE, dwAccess);
	if(ret)
	{
		reg.DeleteKey();
	}
	reg.Close();

	return ret;
}

bool delInfomationKeybyWinAPI()
{
	TCHAR cadKey[MAX_PATH];
	_tcscpy(cadKey, _T("Software\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:804\\InfoCenter"));

	DWORD dwAccess = KEY_ALL_ACCESS;
	//判断是否64位
	if(IsWow64())
	{
		dwAccess |= KEY_WOW64_64KEY;
	}

	CRegistry reg;
	bool ret = reg.Open( cadKey, HKEY_CURRENT_USER, dwAccess);
	if(ret)
	{
		ret = false;
		if(reg.Count()>0)
		{
			int n = reg.Count();
			for(int i=0;i<n;i++)
			{
				CRegEntry* entry = reg.GetAt(i);
				if(QString::compare(_T("InfoCenterOn"), entry->lpszName, Qt::CaseInsensitive) == 0)
				{
					*entry = 1;
					ret = true; break;
				}
			}
		}
	}
	reg.Close();

	return ret;
}

bool delRegByWinAPI()
{
	if(!delTestKeybyWinAPI()) return false;
	if(!delInfomationKeybyWinAPI()) return false;
	return true;
}

void deleteRegByQt()
{
	//自动加载注册表的删除
	QSettings *reg = new QSettings("HKEY_LOCAL_MACHINE\\Software\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:804\\Applications\\test",
		QSettings::NativeFormat);
	if (reg->allKeys().isEmpty())
	{
		return;
	}
	reg->clear();
	delete reg;

	//标题搜索栏的恢复
	QSettings *regFind = new QSettings("HKEY_CURRENT_USER\\Software\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:804\\InfoCenter",
		QSettings::NativeFormat);
	if(regFind->contains("InfoCenterOn"))
	{
		regFind->setValue("InfoCenterOn",1);
	}
	delete regFind; 
}

bool deleteReg()
{
	return delRegByWinAPI();
	//return deleteRegByQt();
}

void writFiles( QString& fromDir,QString& toDir, const QString fileName )
{
	QStringList args;
	//此处toDir为CAD的安装目录
	toDir = toDir + "\\" + fileName;
	//isAutoCADExist(toDir,args);
	if(!copyFileToPath(fromDir,toDir,true)) return;
}