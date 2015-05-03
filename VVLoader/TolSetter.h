#pragma once

// 系统误差设置
class TolSetter
{
public:
	TolSetter(double eq)
	{
		m_originEq = AcGeContext::gTol.equalPoint();
		setEq(eq);
	}

	void setEq(double eq)
	{
		AcGeContext::gTol.setEqualPoint(eq);
	}

	~TolSetter()
	{
		AcGeContext::gTol.setEqualPoint(m_originEq);
	}

private:
	double m_originEq;
};

/*
使用方法:
TolSetter ts(0.001); // 对象析构的时候自动还原系统误差

等价于:
// 备份系统误差
double ep = AcGeContext::gTol.equalPoint();
// 设置误差
AcGeContext::gTol.setEqualPoint(0.001); // 10-3

// do something
// ...

// 还原系统误差
AcGeContext::gTol.setEqualPoint(ep);
*/