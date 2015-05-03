#include <Windows.h>

#include "RegHelper.h"
#include "CADHelper.h"

RegHelper::RegHelper()
{

}

RegHelper::~RegHelper()
{

}

//获取机器码，机器码为CPU和磁盘序列号
QString RegHelper::makeMachineCode()
{
	unsigned long s1,s2;     
	TCHAR sel;   
	sel='1';   
	QString MyCpuID,CPUID1,CPUID2;   
	__asm{   
		mov eax,01h   
			xor edx,edx   
			cpuid   
			mov s1,edx   
			mov s2,eax   
	}   
	//CPUID1.Format(_T("%08X%08X"),s1,s2);  
	CPUID1 = CPUID1.sprintf("%08X%08X",s1,s2);
	__asm{   
		mov eax,03h   
			xor ecx,ecx   
			xor edx,edx   
			cpuid   
			mov s1,edx   
			mov s2,ecx   
	}   
	//CPUID2.Format(_T("%08X%08X"),s1,s2);   
	CPUID2 = CPUID2.sprintf("%08X%08X",s1,s2);

	MyCpuID=CPUID1+CPUID2;

	DWORD ser;
	TCHAR namebuf[128];
	TCHAR filebuf[128];
	QString pos = "c:\\";
	::GetVolumeInformation(LPTSTR(pos.toStdString().c_str()),namebuf,128,&ser,0,0,filebuf,128);
	QString strdisk;
	strdisk = strdisk.sprintf(("%d"),ser);
	//strdisk.Format(_T("%d"),ser);
	QString strmachine;
	strmachine=MyCpuID.mid(13,5); //从MyCpuID的第13位开始取5个
	strmachine+=strdisk.mid(3,5);//从strdisk的第3位开始取5个,合并生成机器码

	//QString code[16]={("ad"),("eh"),("im"),("np"),("ru"),("vy"),("zc"),("gk"),
	//	("pt"),("xb"),("fj"),("ox"),("wa"),("ei"),("nr"),("qu")};
	QString code[16]={("ad"),("eh"),("im"),("np"),("ru"),("vy"),("zc"),("gk"),
		("pt"),("xb"),("fj"),("ox"),("wa"),("ei"),("nr"),("qu")};

	QString reg,stred;
	int num;

	stred = strmachine ;
	stred= stred.toLower();
	for(int i=0;i<10;i++)
	{
		char p=stred.at(i).toLatin1();
		if(p>='a'&&p<='f')
			num=p-'a'+10;
		else
			num=p-'0';
		QString tmp=code[num];
		reg+=tmp;
	}
	m_machineCode = reg.toUpper();
	return m_machineCode;
}

void RegHelper::writeRegMsg(QString machineCode,QString Regmsg)
{
	//QString regID;
	//for(int i = m_machineCode.count() -1; i >= 0; i--)
	//	regID.append(m_machineCode.at(i));
	//QString regIDRet = QCryptographicHash::hash( regID.toAscii(), QCryptographicHash::Md5).toHex();
	//regIDRet = regIDRet.toUpper();
	QString cadPath = getCADPath("AcadLocation",LOCAL_MACHINE);
	QString toDir = cadPath;
	cadPath.append("\\JLcad.key");
	QFile *file = new QFile();
	file->setFileName(cadPath);
	file->open(QIODevice::WriteOnly);
	QString msg = machineCode + Regmsg;
	file->write(msg.toLatin1());
	file->close();

	//QDir dir(QDir::currentPath());
	//QString fromDir = dir.currentPath()+"/Datas/"+ "JL/" + "acad.exe";
	//writFiles(fromDir,toDir,"acad.exe");
	//QString msgB = fromDir;
	//msgB.append("\n");
	//msgB.append(toDir);
	//QMessageBox mBox;
	//mBox.setText(msgB);
	//mBox.exec();
}

void RegHelper::readRegMsg(	QString& msg)
{
	QString cadPath = getCADPath("AcadLocation",LOCAL_MACHINE);
	cadPath.append("\\JLcad.key");
	QFile *file = new QFile();
	file->setFileName(cadPath);
	file->open(QIODevice::ReadOnly);
	msg = QString(file->readAll());
	file->close();
}

QString RegHelper::creatRegCode(const QString& machineCode)
{
	QString regID;
	for(int i = machineCode.count() -1; i >= 0; i--)
		regID.append(machineCode.at(i));
	QString regIDRet = QCryptographicHash::hash( regID.toAscii(), QCryptographicHash::Md5).toHex();
	m_regCode = regIDRet.toUpper();
	return m_regCode;
}