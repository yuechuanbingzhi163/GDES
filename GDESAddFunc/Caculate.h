#pragma once

class Calculate
{
	public:
		static bool MineGasCapacityCacul(const AcStringArray& baseDatas,const AcStringArray& pumpDatas,CString& strPumpRet,CString& strSysRet);
		static bool MineGasRealCacul(const AcStringArray& datas,CString& strYearRet);
		static bool MinePreGasCacul(const AcStringArray& baseDatas,const AcStringArray& listDatas,CString& strRet);
		static bool MineRateGasCacul(const AcStringArray& datas,CString& strRet);
};