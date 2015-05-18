#include "stdafx.h"
#include "PropertyDataDlg.h"
#include "CaculFunc.h"
#include "ValueHelper.h"

#include "../MineGE/DataHelper.h"
#include "../MineGE/FieldHelper.h"
#include "../MineGE/PropertyDataUpdater.h"
#include "../MineGE/FuncFieldHelper.h"
#include "../MineGE/config.h"
#include "../MineGE/CustomProperties.h"

#include <map>
#include <vector>
#include <fstream>
// 需要包含<fstream>才能使用
#include "AcFStream.h"

typedef std::vector<CMFCPropertyGridProperty*> PropertyArray;
typedef std::map<CString, PropertyArray*> PropertyMap;

#define CMP(x1,x2) fabs(x1 - x2) < TOLERANCE
#define NOT_EQUAL_ZERO(x) !(CMP(_tstof(x),0))

//已知D、Q计算V
class SyncFunctorV : public SyncFunctor
{
public:
	SyncFunctorV(AcPropertyArray& pProps) : SyncFunctor(pProps) {}
	virtual double caculate()
	{
		//第1个是D，第2个是Q，第3个是V
		double valueD = m_pProps[0]->GetValue().dblVal;
		double valueQ = m_pProps[1]->GetValue().dblVal;
		//double valueV = m_pProps[2]->GetValue().dblVal;
		return valueQ / pow(valueD*0.001/0.1457,2);
	}
};

//已知D、V计算Q
class SyncFunctorQ : public SyncFunctor
{
public:
	SyncFunctorQ(AcPropertyArray& pProps) : SyncFunctor(pProps) {}
	virtual double caculate()
	{
		//第1个是D，第2个是Q，第3个是V
		double valueD = m_pProps[0]->GetValue().dblVal;
		//double valueQ = m_pProps[1]->GetValue().dblVal;
		double valueV = m_pProps[2]->GetValue().dblVal;
		return valueV * pow(valueD*0.001/0.1457, 2);
	}
};

//已知Q、V计算D
class SyncFunctorD : public SyncFunctor
{
public:
	SyncFunctorD(AcPropertyArray& pProps) : SyncFunctor(pProps) {}
	virtual double caculate()
	{
		//第1个是D，第2个是Q，第3个是V
		//double valueD = m_pProps[0]->GetValue().dblVal;
		double valueQ = m_pProps[1]->GetValue().dblVal;
		double valueV = m_pProps[2]->GetValue().dblVal;
		return 0.1457*sqrt(valueQ/valueV) * 1000;
	}
};

//已知GJ、Y计算H
class SyncFunctorH : public SyncFunctor
{
public:
	SyncFunctorH(AcPropertyArray& pProps) : SyncFunctor(pProps) {}
	virtual double caculate()
	{
		//第1个是H，第2个是GJ，第3个是Y
		//double valueH = m_pProps[0]->GetValue().dblVal;
		double valueGJ = m_pProps[1]->GetValue().dblVal;
		double valueY = m_pProps[2]->GetValue().dblVal;

		return 0;
	}
};

//已知H、Y计算Gj
class SyncFunctorGJ : public SyncFunctor
{
public:
	SyncFunctorGJ(AcPropertyArray& pProps) : SyncFunctor(pProps) {}
	virtual double caculate()
	{
		//第1个是H，第2个是GJ，第3个是Y
		double valueH = m_pProps[0]->GetValue().dblVal;
		//double valueGJ = m_pProps[1]->GetValue().dblVal;
		double valueY = m_pProps[2]->GetValue().dblVal;

		return -1.0*valueY + valueH;
	}
};

//已知H、GJ计算Y
class SyncFunctorY : public SyncFunctor
{
public:
	SyncFunctorY(AcPropertyArray& pProps) : SyncFunctor(pProps) {}
	virtual double caculate()
	{
		//第1个是H，第2个是GJ，第3个是Y
		double valueH = m_pProps[0]->GetValue().dblVal;
		double valueGJ = m_pProps[1]->GetValue().dblVal;
		//double valueY = m_pProps[2]->GetValue().dblVal;

		return fabs(-1*valueGJ + valueH);
	}
};

//已知GD、VD、FD计算DD
class SyncFunctorDD : public SyncFunctor
{
public:
	SyncFunctorDD(AcPropertyArray& pProps) : SyncFunctor(pProps) {}
	virtual double caculate()
	{
		double valueGD = m_pProps[0]->GetValue().dblVal;
		double valueVD = m_pProps[1]->GetValue().dblVal;
		double valueFD = m_pProps[2]->GetValue().dblVal;
		double valueDD = m_pProps[3]->GetValue().dblVal;

		if(valueFD <= 0) 
			return valueDD;
		else
			return valueVD/valueFD * 100;
	}
};

static void GetPropertyByFiled(const CString& fieldName,CMFCPropertyGridCtrl* propertyCtrl,PropertyMap& pm)
{
	int groupCount = propertyCtrl->GetPropertyCount();
	for( int i = 0; i < groupCount; i++)
	{
		CMFCPropertyGridProperty* pGroup = propertyCtrl->GetProperty(i);
		int propCount = pGroup->GetSubItemsCount();
		for( int j = 0; j < propCount; j++ )
		{
			CMFCPropertyGridProperty* pPro = pGroup->GetSubItem(j);
			if (pPro->GetName() == fieldName)
			{
				if(pm.find(fieldName) == pm.end())
				{
					pm[fieldName] = new PropertyArray;
				}
				pm[fieldName]->push_back(pPro);
			}
		}
	}
}

static void ClearPropertyMap(PropertyMap& pm)
{
	for(PropertyMap::iterator itr=pm.begin(); itr!=pm.end();++itr)
	{
		delete itr->second;
	}
}

//关联管径及流速
static void LinkSyncProperties_Gas(CMFCPropertyGridCtrl* propertyDataList, AcPropertyArray* pProps)
{
	PropertyMap pm;
	CString field1 = _T("瓦斯管内径");
	CString field2=_T("瓦斯管中的瓦斯流量");
	CString field3 = _T("瓦斯管中瓦斯平均流速");
	GetPropertyByFiled(field1,propertyDataList,pm);
	GetPropertyByFiled(field2,propertyDataList,pm);
	GetPropertyByFiled(field3,propertyDataList,pm);
	if(pm[field1] == NULL || pm[field2] == NULL || pm[field3] == NULL) return;

	CustomGridProperty *pProD = (CustomGridProperty*)pm[field1]->at(0);
	CustomGridProperty *pProQ = (CustomGridProperty*)pm[field2]->at(0);
	CustomGridProperty *pProV = (CustomGridProperty*)pm[field3]->at(0);

	pProps->append(pProD);
	pProps->append(pProQ);
	pProps->append(pProV);

	pProD->setSyncFun(new SyncFunctorD(*pProps));
	pProQ->setSyncFun(new SyncFunctorQ(*pProps));
	pProV->setSyncFun(new SyncFunctorV(*pProps));

	ClearPropertyMap(pm);
}

//关联压力
static void LinkSyncProperties_Pressure(CMFCPropertyGridCtrl* propertyDataList, AcPropertyArray* pProps)
{
	PropertyMap pm;
	CString field1 = _T("巷道绝对大气压");
	CString field2 = _T("管道内气体的绝对压力");
	CString field3 = _T("管路压差");
	GetPropertyByFiled(field1,propertyDataList,pm);
	GetPropertyByFiled(field2,propertyDataList,pm);
	GetPropertyByFiled(field3,propertyDataList,pm);
	if(pm[field1] == NULL || pm[field2] == NULL || pm[field3] == NULL) return;

	CustomGridProperty *pProH = (CustomGridProperty*)pm[field1]->at(0);
	CustomGridProperty *pProGJ = (CustomGridProperty*)pm[field2]->at(0);
	CustomGridProperty *pProY = (CustomGridProperty*)pm[field3]->at(0);

	pProps->append(pProH);
	pProps->append(pProGJ);
	pProps->append(pProY);

	//pProH->setSyncFun(new SyncFunctorH(*pProps));
	pProGJ->setSyncFun(new SyncFunctorGJ(*pProps));
	pProY->setSyncFun(new SyncFunctorY(*pProps));

	ClearPropertyMap(pm);
}

//关联瓦斯涌出量
static void LinkSyncProperties_Flow(CMFCPropertyGridCtrl* propertyDataList, AcPropertyArray* pProps)
{
	PropertyMap pm;
	CString field1 = _T("工作面瓦斯浓度");
	CString field2 = _T("风排瓦斯量");
	CString field3 = _T("工作面配风量");
	CString field4 = _T("瓦斯绝对涌出量");

	GetPropertyByFiled(field1,propertyDataList,pm);
	GetPropertyByFiled(field2,propertyDataList,pm);
	GetPropertyByFiled(field3,propertyDataList,pm);
	GetPropertyByFiled(field4,propertyDataList,pm);
	if(pm[field1] == NULL || pm[field2] == NULL || pm[field3] == NULL || pm[field4] == NULL) return;

	CustomGridProperty *pProGD = (CustomGridProperty*)pm[field1]->at(0);
	CustomGridProperty *pProVD = (CustomGridProperty*)pm[field2]->at(0);
	CustomGridProperty *pProFD = (CustomGridProperty*)pm[field3]->at(0);
	CustomGridProperty *pProDD = (CustomGridProperty*)pm[field4]->at(0);

	pProps->append(pProGD);
	pProps->append(pProVD);
	pProps->append(pProFD);
	pProps->append(pProDD);

	//pProGD->setSyncFun(new SyncFunctorD(*pProps));
	//pProVD->setSyncFun(new SyncFunctorQ(*pProps));
	//pProFD->setSyncFun(new SyncFunctorV(*pProps));
	pProDD->setSyncFun(new SyncFunctorDD(*pProps));

	ClearPropertyMap(pm);
}

static CString GetAppPathDir()
{
	TCHAR szModulePath[_MAX_PATH];
	GetModuleFileName( _hdllInstance, szModulePath, _MAX_PATH );

	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	_tsplitpath( szModulePath, drive, dir, NULL, NULL );

	TCHAR szPath[_MAX_PATH] = {0};
	_tmakepath( szPath, drive, dir, NULL, NULL );

	return CString( szPath );
}

static CString BuildPath( const CString& dir, const CString& fileName )
{
	CString path;
	path.Format( _T( "%s%s" ), dir, fileName );
	return path;
}

IMPLEMENT_DYNAMIC( PropertyDataDlg, CDialog )

PropertyDataDlg::PropertyDataDlg( CWnd* pParent /*=NULL*/,CString func )
: CDialog( PropertyDataDlg::IDD, pParent )
{
	m_showAll = true; // 默认显示全部数据
	m_func = func;
	//CMFCPropertyGridProperty::m_strFormatDouble = _T("%.4f");
	//CMFCPropertyGridProperty::m_strFormatFloat = _T("%.4f");
}

PropertyDataDlg::~PropertyDataDlg()
{
}

void PropertyDataDlg::DoDataExchange( CDataExchange* pDX )
{
	CDialog::DoDataExchange( pDX );
}

BEGIN_MESSAGE_MAP( PropertyDataDlg, CDialog )
	ON_BN_CLICKED( IDOK, &PropertyDataDlg::OnBnClickedOk )
	ON_REGISTERED_MESSAGE( AFX_WM_PROPERTY_CHANGED, OnPropertyChanged )
	ON_WM_TIMER()
END_MESSAGE_MAP()


BOOL PropertyDataDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//	ModifyStyle(0, WS_SIZEBOX);
	if(!m_func.IsEmpty() && _T("基本信息") != m_func)
	{
		GetDlgItem(IDOK)->SetWindowText(_T("计算"));
		GetDlgItem(IDCANCEL)->SetWindowText(_T("退出"));
	}

	if(!m_func.IsEmpty())
	{
		SetWindowText(m_func);
	}

	// 创建, 定位, 显示CMFCPropertyGridCtrl
	CRect rect;
	GetDlgItem( IDC_PROP_POS )->GetWindowRect( &rect );
	ScreenToClient( &rect );
	m_propertyDataList.Create( WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rect, this, ( UINT ) - 1 );
	//m_propertyDataList.SetAlphabeticMode(true);
	// 获取类型名称
	CString type;
	DataHelper::GetTypeName( m_objId, type );

	AcStringArray funcs,funcFieldsInfo;
	if(m_func.IsEmpty())
	{
		FuncFieldHelper::GetFunctions(funcs);
	}
	else
	{
		funcs.append(m_func);
	}
	int funcNum = funcs.length();
	for (int i = 0; i < funcNum; i++)
	{
		AcStringArray fields;
		FuncFieldHelper::GetFields(funcs[i].kACharPtr(),type,fields);
		if(fields.isEmpty()) continue;
		funcFieldsInfo.append(_T("$"));
		funcFieldsInfo.append(funcs[i]);
		for(int j = 0; j < fields.length(); j++)
		{
			funcFieldsInfo.append(fields[j]);
		}
	}

	// 创建属性数据控件
	PropertyDataUpdater::BuildPropGridCtrl( &m_propertyDataList, type,funcFieldsInfo );

	//关联瓦斯管计算功能的字段
	LinkSyncProperties_Gas(&m_propertyDataList, &m_gasProps);
	//关联压力计算功能的字段
	LinkSyncProperties_Pressure(&m_propertyDataList, &m_pressureProps);
	//关联瓦斯涌出量计算功能的字段
	LinkSyncProperties_Flow(&m_propertyDataList, &m_flowProps);

	// 填充属性数据
	bool ret = PropertyDataUpdater::ReadDataFromGE( &m_propertyDataList, m_objId );

	// 填充数据失败
	if( !ret )
	{
		m_propertyDataList.EnableWindow( FALSE );
		m_propertyDataList.ShowWindow( SW_HIDE );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}

void HighlightGridProperty(CMFCPropertyGridCtrl* pPropDataList, bool bHighlight)
{
	int nCount = pPropDataList->GetPropertyCount();
	for ( int i = 0; i < nCount; i++ )
	{
		CMFCPropertyGridProperty* pGroup = pPropDataList->GetProperty( i );
		for(int j = 0; j < pGroup->GetSubItemsCount(); j++)
		{
			CMFCPropertyGridProperty* pProp = pGroup->GetSubItem( j );
			if(!pProp->IsEnabled())
			{
				CustomGridProperty* cProp = (CustomGridProperty*)pProp;
				cProp->highlight(bHighlight);
			}
		}
	}
}

void PropertyDataDlg::OnBnClickedOk()
{
	UpdateData( TRUE );

	PropertyDataUpdater::WriteDataToGE( &m_propertyDataList, m_objId );


	if(m_func.IsEmpty() || _T("基本信息") == m_func)
	{
		OnOK();
		return;
	}

	//UpdateData(FALSE);
	if(!caculate())
	{
		//OnOK();
		return;
	}

	SetTimer(100,500,NULL);
	AfxMessageBox(_T("计算成功!"));

	HighlightGridProperty(&m_propertyDataList, true);

	// 填充属性数据
	bool ret = PropertyDataUpdater::ReadDataFromGE( &m_propertyDataList, m_objId );

	// 填充数据失败
	if( !ret )
	{
		m_propertyDataList.EnableWindow( FALSE );
		m_propertyDataList.ShowWindow( SW_HIDE );
	}

	HighlightGridProperty(&m_propertyDataList, m_objId);
}

void PropertyDataDlg::setMineGE( const AcDbObjectId& objId )
{
	m_objId = objId;
}

void PropertyDataDlg::addField( const CString& field )
{
	m_fields.append( field );
}

void PropertyDataDlg::showAllData( bool bFlag )
{
	m_showAll = bFlag;
}

void PropertyDataDlg::OnTimer( UINT_PTR nIDEvent )
{
	CDialog::OnTimer(nIDEvent);
	if (nIDEvent == 100)
	{
		KillTimer(nIDEvent);
		keybd_event(VK_RETURN,0,0,0);
		keybd_event(VK_RETURN,0,KEYEVENTF_KEYUP,0);//模拟"回车"命令
	}

}

bool PropertyDataDlg::caculate()
{
	CaculFunc cacul;
	cacul.m_objId = m_objId;

	if(m_func.IsEmpty()) return false;
	if (_T("参与评价的煤量") == m_func)
	{
		return cacul.coalResrevesCacul();
	}

	
	if (0 == m_func.CompareNoCase(_T("通过瓦斯抽采率计算")))
	{
		return cacul.remnantGasByQCacul();
	}

	if (_T("通过残余瓦斯压力反算") == m_func)
	{
		return cacul.remnantGasByPCacul();
	}

	if (_T("残余瓦斯压力") == m_func)
	{
		return cacul.remnantPressCacul();
	}

	if (_T("残存瓦斯含量") == m_func)
	{
		return cacul.remnantGasStandCacul();
	}

	if (_T("可解吸瓦斯含量") == m_func)
	{
		if(!cacul.analyGasCacul()) return false;
		else return true;
	}

	if (_T("工作面瓦斯抽采率") == m_func)
	{
		cacul.faceDrainGasRateCacul();
		return true;
	}

	if (_T("矿井瓦斯抽采率") == m_func)
	{
		cacul.mineDrainGasRateCacul();
		return true;
	}

	if (_T("本煤层瓦斯涌出量") == m_func)
	{
		cacul.miningFaceGasCacul();
		return true;
	}

	if (_T("邻近层瓦斯涌出量") == m_func)
	{
		cacul.nearbyFaceGasCacul();
		return true;
	}

	if (_T("管径及流速") == m_func)
	{
		cacul.gasTubeDiameterCacul();
		return true;
	}

	if (_T("管路阻力") == m_func)
	{
		return cacul.tubeFrictionResisCacul();
		//return true;
	}

	if (_T("管路计算") == m_func)
	{
		if(!cacul.gasTubeDiameterCacul()) return false;
		//ValueHelper::setGasAirValue(m_objId);
		if(!cacul.tubeFrictionResisCacul()) return false;
		return true;
	}

	if (_T("瓦斯泵的流量计算") == m_func)
	{
		if(!cacul.pumbFlowCacul()) return false;
		return true;
	}

	if (_T("瓦斯泵的压力计算") == m_func)
	{
		cacul.pumbPressCacul();
		return true;
	}

	if(_T("瓦斯泵计算") == m_func)
	{
		if(!cacul.pumbPressCacul()) return false;
		if(!cacul.pumbFlowCacul()) return false;
		return true;
	}

	if (_T("孔口负压参数") == m_func)
	{
		if(!cacul.drainDrillFyCacul()) return false;
		return true;
	}

	if (_T("终孔设计高度参考值") == m_func)
	{
		cacul.zkDbHeightCacul();
		return true;
	}

	if (_T("钻孔偏角参数") == m_func)
	{
		cacul.drillDeflectionCacul();
		return true;
	}

	if (_T("钻孔仰角参数") == m_func)
	{
		if(!cacul.drillElevationCacul()) return false;
		return true;
	}

	if (_T("钻孔长度参数") == m_func)
	{
		if(!cacul.drillLengthCacul()) return false;
		return true;
	}

	if (_T("钻孔有效长度") == m_func)
	{
		if(!cacul.drillEffectiveLenthCacul()) return false;
		return true;
	}

	if (_T("工作面推进有效长度") == m_func)
	{
		if(!cacul.faceEffectiveLenthCacul()) return false;
		return true;
	}

	if (_T("高位钻孔钻场间距") == m_func)
	{
		if(!cacul.highDrillDisCacul()) return false;
		return true;
	}

	if( _T("高位钻孔参数计算") == m_func )
	{
		if(!cacul.drainDrillFyCacul()) return false;
		if(!cacul.zkDbHeightCacul()) return false;
		if(!cacul.drillDeflectionCacul()) return false;
		if(!cacul.drillElevationCacul()) return false;
		if(!cacul.drillLengthCacul()) return false;
		if(!cacul.drillEffectiveLenthCacul()) return false;
		if(!cacul.faceEffectiveLenthCacul()) return false;
		if(!cacul.highDrillDisCacul()) return false;
		return true;
	}
	return true;
}

static void ReadAirFactor(const COleVariant& changedValue,CString& strValue)
{
	CString dataDirName = _T( "Datas\\" );
	CString fileName =BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T("gasAir.txt") );

	CString gasValueStr = (CString)changedValue;

	if(gasValueStr.IsEmpty()) gasValueStr = _T("0");
	AcIfstream inFile(fileName);
	AcStringArray fields,values;
	if(!inFile) return;
	CString gasValueStr0,gasAirValueStr;
	while( !inFile.eof() )
	{
		ACHAR gasValueChar[_MAX_PATH], gasAirValueChar[_MAX_PATH];
		inFile >> gasValueChar >> gasAirValueChar;
		if(inFile.fail()) break;
		gasValueStr0.Format(_T("%s"),gasValueChar);
		if(CMP(_tstof(gasValueStr), _tstof(gasValueStr0)))
		{
			gasAirValueStr.Format(_T("%s"),gasAirValueChar);
			break;
		}
	}

	inFile.close();
	strValue = gasAirValueStr;
}

static void AirFactorHelper(const COleVariant& changedValue,CMFCPropertyGridCtrl* propertyDataList)
{
	CString value;
	ReadAirFactor(changedValue,value);
	PropertyMap pm;
	CString field = _T("混合瓦斯对空气的相对密度");
	GetPropertyByFiled(field,propertyDataList,pm);
	if(pm[field] == NULL) return;

	PropertyArray &pa = *(pm[field]);
	for(PropertyArray::iterator a_itr=pa.begin(); a_itr!=pa.end();a_itr++)
	{
		CMFCPropertyGridProperty *pProp = *a_itr;
		COleVariant origin = pProp->GetOriginalValue();
		COleVariant v;
		v.vt = origin.vt;
		v = _tstof(value);
		pProp->SetValue(v);
	}
}

void PropertyDataDlg::SyncPropertyDatas(const CString& filedName, COleVariant& changedValue,CMFCPropertyGridProperty*pProp )
{
	int groupCount = m_propertyDataList.GetPropertyCount();
	for( int i = 0; i < groupCount; i++)
	{
		CMFCPropertyGridProperty* pGroup = m_propertyDataList.GetProperty(i);
		int propCount = pGroup->GetSubItemsCount();
		AcStringArray diams,pressures;
		//查找所有具有相同名字的字段，并且把值修改了
		for( int j = 0; j < propCount; j++ )
		{
			CMFCPropertyGridProperty* pPro = pGroup->GetSubItem(j);
			if (pPro->GetName() == filedName && pPro != pProp)
			{
				pPro->SetValue(changedValue);
			}
			if(filedName == _T("管道中的瓦斯浓度"))
			{
				AirFactorHelper(changedValue,&m_propertyDataList);
			}
		}
	}
}

LRESULT PropertyDataDlg::OnPropertyChanged( WPARAM wparam,  LPARAM lparam )
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*) lparam;
	CString filedName = pProp->GetName();
	COleVariant changedValue = pProp->GetValue();
	SyncPropertyDatas(filedName,changedValue,pProp);
	UpdateData(true);
	return 0;
}

