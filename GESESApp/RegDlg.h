#ifndef REG_DLG_H
#define REG_DLG_H
#include <QtGui/QDialog>
#include "RegHelper.h"

class QLineEdit;
class QLabel;
class QPushButton;

class RegDlg : public QDialog
{
	Q_OBJECT

public:
	RegDlg(QWidget *parent = 0, Qt::WFlags flags = 0);
	~RegDlg();

	private slots:
		void Regist();
		void copyMachineCode();

public:
	QString regID;
	QString machineID;
	bool regSucs;
private:
	QLineEdit* machineIdEdit;
	QLabel* machineIdLabel;
	QLineEdit* regIdEdit;
	QLabel* regIdLabel;
	QPushButton* getRegIdBtn;
	QPushButton* copyBtn;

	RegHelper *m_regHelper;
	int m_nIndex;

};

#endif //REG_DLG_H