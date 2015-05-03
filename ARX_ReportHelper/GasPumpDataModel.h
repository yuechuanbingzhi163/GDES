#pragma once

struct GasPumpDataRecord
{
	GasPumpDataRecord()
	{}

	GasPumpDataRecord(const CString& sysName, const CString& pumpState,  const CString& pumpType, const CString& pumpAbility)
		:m_sysName(sysName)
		,m_pumpState(pumpState)
		,m_pumpType(pumpType)
		,m_pumpAbility(pumpAbility)
	{}

	CString m_sysName;
	CString m_pumpState;
	CString m_pumpType;
	CString m_pumpAbility;

	CString GetCellText(int col, bool title) const
	{
		switch(col)
		{
		case 0: { static const CString title0(_T("抽采系统")); return title ? title0 : m_sysName; }
		case 1: { static const CString title1(_T("抽采泵状态")); return title ? title1 : m_pumpState; }
		case 2: { static const CString title2(_T("型号")); return title ? title2 : m_pumpType; }
		case 3: { static const CString title2(_T("单泵最大抽采能力")); return title ? title2 : m_pumpAbility; }
		default:{ static const CString emptyStr; return emptyStr; }
		}
	}

	int  GetColCount() const { return 4; }
};

class GasPumpDataModel
{
	vector<GasPumpDataRecord> m_Records;
	int	m_LookupTime;
	int m_RowMultiplier;
	int m_RowCount;

public:
	GasPumpDataModel()
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
			int rSize = m_Records.size();
			if(rSize % 2 == 0)
			{
				m_Records.push_back( GasPumpDataRecord(_T(""), _T("运行泵"), _T(""), _T("") ));
			}
			else
			{
				m_Records.push_back( GasPumpDataRecord(_T(""), _T("备用泵"), _T(""), _T("") ));
			}
		}

		//if (m_RowMultiplier > 1)
		//{
		//	vector<GasPumpDataRecord> rowset(m_Records);
		//	m_Records.reserve((m_RowMultiplier-1) * rowset.size());
		//	for(int i = 0 ; i < m_RowMultiplier ; ++i)
		//	{
		//		m_Records.insert(m_Records.end(), rowset.begin(), rowset.end());
		//	}
		//}
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

	vector<CString> GetRunning() const
	{
		vector<CString> isRuning;
		for(size_t rowId = 0 ; rowId < m_Records.size(); ++rowId)
			isRuning.push_back( m_Records[rowId].m_pumpType );
		sort(isRuning.begin(), isRuning.end());
		isRuning.erase(unique(isRuning.begin(), isRuning.end()), isRuning.end());
		return isRuning;
	}

	size_t GetRowIds() const { return m_Records.size(); }
	int GetColCount() const { return GasPumpDataRecord().GetColCount(); }
	CString GetColTitle(int col) const { return GasPumpDataRecord().GetCellText(col, true); }

	vector<GasPumpDataRecord>& GetRecords() { return m_Records; }
	void SetRowMultiplier(int multiply) { if (m_RowMultiplier != multiply ) { m_RowMultiplier = multiply; InitDataModel(); } }
	void SetRowCount(int rowCount) { if (m_RowCount != rowCount ) { m_RowCount = rowCount;InitDataModel(); } }
	void SetRecords(vector<GasPumpDataRecord> records){ m_Records = records/*;InitDataModel()*/;}
};