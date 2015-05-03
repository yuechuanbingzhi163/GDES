#ifndef REG_HELPER_H
#define REG_HELPER_H
#include <QtGui>

class RegHelper
{
public:
	RegHelper();
	~RegHelper();

public:
	QString makeMachineCode();
	QString creatRegCode(const QString& machineCode);
	void writeRegMsg(QString machineCode,QString Regmsg);
	void readRegMsg(QString &msg);

private:
	QString m_machineCode;
	QString m_regCode;

};
#endif //REG_HELPER_H