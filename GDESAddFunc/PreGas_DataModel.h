#pragma once

struct PreGas_DataRecord
{
	PreGas_DataRecord()
	{}

	PreGas_DataRecord(const CString& strIndx, const CString& strLength, const CString& strHeight,  const CString& strDencity, const CString& strDayAdvance, const CString& strRecovRate)
		:m_strIndx(strIndx)
		,m_strLength(strLength)
		,m_strHeight(strHeight)
		,m_strDencity(strDencity)
		,m_strDayAdvance(strDayAdvance)
		,m_strRecovDate(strRecovRate)
	{}

	CString	m_strIndx;
	CString	m_strLength;
	CString	m_strHeight;
	CString m_strDencity;
	CString	m_strDayAdvance;
	CString	m_strRecovDate;

	CString GetCellText(int col, bool title) const
	{
		switch(col)
		{
		case 0: { static const CString title0(_T("序号")); return title ? title0 : m_strIndx; }
		case 1: { static const CString title1(_T("平均长度(m)")); return title ? title1 : m_strLength; }
		case 2: { static const CString title2(_T("平均采高(m)")); return title ? title2 : m_strHeight; }
		case 3: { static const CString title3(_T("原煤视密度(t/m3)")); return title ? title3 : m_strDencity; }
		case 4: { static const CString title4(_T("平均日推进度(m/d)")); return title ? title4 : m_strDayAdvance; }
		case 5: { static const CString title5(_T("工作面回采率(%)")); return title ? title5 : m_strRecovDate; }
		default:{ static const CString emptyStr; return emptyStr; }
		}
	}

	int  GetColCount() const { return 6; }
};

class PreGas_DataModel
{
	vector<PreGas_DataRecord> m_Records;
	int	m_LookupTime;
	int m_RowMultiplier;
	int m_RowCount;

public:
	PreGas_DataModel()
		:m_RowMultiplier(0)
		,m_LookupTime(0)
		,m_RowCount(0)
	{
		InitDataModel();
	}

	void InitDataModel()
	{
		int oldCount = m_Records.size();
		if(oldCount > m_RowCount)
		{
			int dif = oldCount - m_RowCount;
			for(int i = dif-1; i >= 0; i--)
			{
				m_Records.pop_back();
			}
			return;
		}
		//m_Records.clear();
		for(int i = 0; i < m_RowCount - oldCount; i++)
		{
			CString strIndx;
			strIndx.Format(_T("%d"),i+oldCount+1);
			m_Records.push_back( PreGas_DataRecord(strIndx,_T(""), _T(""), _T(""),_T(""), _T("") ));
		}

	}

	CString GetCellText(size_t lookupId, int col) const
	{
		if (lookupId >= m_Records.size())
		{
			static CString oob(_T("Out of Bound"));
			return oob;
		}
		// How many times should we search sequential for the row ?
		for(int i=0; i < m_LookupTime; ++i)
		{
			for(size_t rowId = 0; rowId < m_Records.size(); ++rowId)
			{
				if (rowId==lookupId)
					break;
			}
		}
		return m_Records.at(lookupId).GetCellText(col, false);
	}

	size_t GetRowIds() const { return m_Records.size(); }
	int GetColCount() const { return PreGas_DataRecord().GetColCount(); }
	CString GetColTitle(int col) const { return PreGas_DataRecord().GetCellText(col, true); }

	vector<PreGas_DataRecord>& GetRecords() { return m_Records; }
	void SetRowMultiplier(int multiply) { if (m_RowMultiplier != multiply ) { m_RowMultiplier = multiply; InitDataModel(); } }
	void SetRowCount(int rowCount) { if (m_RowCount != rowCount ) { m_RowCount = rowCount;InitDataModel(); } }
	void SetRecords(vector<PreGas_DataRecord> records){ m_Records.clear();copy(records.begin(),records.end(),back_inserter(m_Records));}
	//vector<CListCtrl_DataRecord> GetRecords(){ return m_Records;}
};