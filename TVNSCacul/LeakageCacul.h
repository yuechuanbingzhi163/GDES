#pragma once

class LeakageCaculate
{
public:
	LeakageCaculate(AcDbObjectId objId);
	~LeakageCaculate();
	double LeakageCaculRet();

private:
	void initDatas();
	void metalCacul();
	void blanketCacul();
	//判断该风筒是否接了下一级风筒，如果是接头数加1
	bool isLinkedNext();

public:
	double m_leakage;

private:
	double m_hmLeakage;
	double m_leakageFactor;
	CString m_meterial;
	double m_chimLenth;
	int m_joints;
	double m_diam;
	double m_windage;

	AcDbObjectId m_objId;
};