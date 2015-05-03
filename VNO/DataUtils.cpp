#include "DataUtils.h"

/**
 * 辅助函数--给定一个数组,计算绝对值最大的元素.
 * @param dvalues [in] 浮点数数组(元素个数不限).
 * @return 绝对值最大的值.
 */
 double DataUtils::MaxAbsValue(DoubleArray& dvalues)
{
    double maxD = abs(dvalues[0]);
    for(size_t i=0;i<dvalues.size();i++)
    {
        double d = abs(dvalues[i]);
        if((maxD- d)<0) maxD = d;
    }
    return maxD;
}

/**
 * 辅助函数--给定一个数组,计算所有数据绝对值的均方差.
 * @param dvalues [in] 浮点数数组(元素个数不限).
 * @return 均方差.
 */
 double DataUtils::MeanSquare(DoubleArray& dvalues)
{
    size_t n = dvalues.size();

    // 1、平均值
    double a = 0;
    for(size_t i=0;i<n;i++)
    {
        a += abs(dvalues[i]);
    }
    a = a/n;

    // 2、均方差
    double s = 0;
    for(size_t i=0;i<n;i++)
    {
        double v = abs(dvalues[i]);
        s += (a-v)*(a-v);
    }
    return (s/n);
}

/**
 * 辅助函数--给定一个数组,通过比较"绝对值最大的数据"与"精度"的大小,判断所有数据是否都满足误差要求.
 * 公式：( max(D)-precise ) <= 0,则认为该数组满足误差要求.
 * @param dvalues [in]  浮点数数组(元素个数不限).
 * @param precise [in]  误差精度.
 * @return 是否满足精度误差.
 * @note 也可以参考统计学中的一些误差计算方法.
 */
 bool DataUtils::IsMeetErrReq( DoubleArray& dvalues, double precise )
{
    if(dvalues.empty()) 
		return true;
	else
		return (MaxAbsValue(dvalues)-precise)<=0;
}

/**
 * 辅助函数--将数据放大一定的倍数.
 * @param v      [in]  要放大的数据.
 * @param coeff  [in]  放大倍数.
 * @return 放大一定倍数后的数值.
 * @note v = (1+v)*coeff.
 */
 double DataUtils::Magnify_Double(double v, double coeff)
{
    return (1+v)*coeff;
}
