#include "RegDlg.h"

#include "Reg.h"
#include "HardWare.h"

RegDlg::RegDlg(QWidget *parent, Qt::WFlags flags)
: QDialog(parent, flags)
{
	resize(300,120);
	QVBoxLayout *mainLayout = new QVBoxLayout;

	QGridLayout *upLayout = new QGridLayout;
	machineIdLabel = new QLabel(tr("machine"));
	upLayout->addWidget(machineIdLabel,0,0,1,1);

	m_regHelper = new RegHelper;
	machineIdEdit = new QLineEdit;

	CHardWare  hw;
	CReg       reg;
	DISKINFO   di;

	// 获取硬件信息
	if ( !hw.GetHDNum( &di ) )
	{
		hw.GetBiosNum( &di );
	}

	// 把硬编码转换成数字ID
	machineID = reg.DisPoseID( di.szSystemInfo );

	machineIdEdit->setText(machineID);
	if (!machineIdEdit->text().isEmpty())
	{
		machineIdEdit->setReadOnly(true);
	}
	else
	{
		machineIdEdit->setReadOnly(false);
	}

	upLayout->addWidget(machineIdEdit,0,1,1,3);

	regIdLabel = new QLabel(tr("regId"));
	upLayout->addWidget(regIdLabel,1,0,1,1);

	regIdEdit = new QLineEdit;
	upLayout->addWidget(regIdEdit,1,1,1,3);

	QGridLayout *downLayout = new QGridLayout;
	getRegIdBtn = new QPushButton(tr("regist"));
	copyBtn = new QPushButton(tr("copyMachineCode"));
	copyBtn->setDisabled(false);

	downLayout->addWidget(getRegIdBtn,0,0,1,1);
	downLayout->addWidget(copyBtn,0,1,1,1);

	QVBoxLayout *msgLayout = new QVBoxLayout;
	QLabel *msgLabel = new QLabel(tr("please connect writer")+"\n"+tr("hunanhd@163.com"));
	msgLayout->addWidget(msgLabel);
	mainLayout->addLayout(msgLayout);

	mainLayout->addLayout(upLayout);
	mainLayout->addLayout(downLayout);

	connect(getRegIdBtn,SIGNAL(clicked()),this,SLOT(Regist()));
	connect(copyBtn,SIGNAL(clicked()),this,SLOT(copyMachineCode()));

	setLayout(mainLayout);
	setWindowTitle(tr("register"));
	regSucs = false;
}

RegDlg::~RegDlg()
{

}

void RegDlg::Regist()
{
	machineID.clear();
	regID.clear();
	regSucs = false;

	CReg          reg;
	CString       tmp;                           // 存放临时变量


	machineID = machineIdEdit->text(); 


	regID = regIdEdit->text();
	// 判断注册码是否已经输入完整
	if ( regID.length() < 16 )
	{
		regID.clear();     // 清空变量
		close();               // 返回, 不处理
	}

	regID.toUpper();     // 转为大写字母

	// 比较注册码是否正确
	m_nIndex = reg.CmpReg((CString) machineID.toStdString().c_str(), (CString)regID.toStdString().c_str() );

	// 如果注册码不正确
	if ( !m_nIndex )
	{
		//Sleep( 3000 );         // 暂停线程3秒
		MessageBox( NULL, "注册码错误", "错误提示",
			MB_OK | MB_ICONINFORMATION );

		//AfxMessageBox("xxxx");
		regSucs = false;
		
	}
	else
	{
		MessageBox( NULL, "注册成功", "提示",
			MB_OK | MB_ICONINFORMATION );
		regSucs = true;
	}
	close();
}


void RegDlg::copyMachineCode()
{
	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setText(machineIdEdit->text());
	QMessageBox msg;
	msg.setWindowTitle(tr("tip"));
	msg.setText(tr("copy success")+ "\n"  + tr("hunanhd@163.com"));
	msg.exec();
	//copyBtn->setDisabled(true);
}