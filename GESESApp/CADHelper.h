#ifndef FILE_COPY_H
#define FILE_COPY_H

#include <QDir>
#include <QFileInfoList>
#include <QString>
#include <windows.h>
/*
qCopyDirectory -- 拷贝目录
fromDir : 源目录
toDir   : 目标目录
bCoverIfFileExists : ture:同名时覆盖  false:同名时返回false,终止拷贝
返回: ture拷贝成功 false:拷贝未完成
*/
enum pathType
{
	CURRENT_USER = 0, //得到的目录C:\Users\hd\AppData\Roaming\Autodesk\AutoCAD 2010\R18.0\chs\Support
	LOCAL_MACHINE = 1, //得到的目录是CAD的安装目录
};
extern bool copyFileToPath( QString& sourceDir , QString& toDir, bool coverFileIfExist) ;

extern bool copyCadFile();

extern bool recorverCadFile();

extern void writeReg(QString& cadPath);

extern bool deleteReg();

//QString getPath(int index,QString keyorvalue,pathType pat);

extern  QString getCADPath(QString locationKey,pathType pat);

extern  bool isAutoCADExist(QString& cadPath,const QStringList& argument);

extern void writFiles( QString& fromDir, QString& toDir, const QString fileName);

extern int FindProcess(LPCTSTR appName);
//void ReadFluentInstallPath( QString& path );
#endif //FILE_COPY_H
