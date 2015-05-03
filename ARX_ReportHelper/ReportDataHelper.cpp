#include "StdAfx.h"
#include "ReportDataHelper.h"

bool ReportDataHelper::SetObjectDatas( const AcDbObjectId& objId,const AcStringArray& values )
{
	if( objId.isNull() ) return false;

	AcTransaction* pTrans = actrTransactionManager->startTransaction();
	if( pTrans == 0 ) return false;

	AcDbObject* pObj;
	if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
	{
		actrTransactionManager->abortTransaction();
		return false;
	}
	DataObject* pDO = DataObject::cast( pObj );
	if( pDO == 0 )
	{
		actrTransactionManager->abortTransaction();
		return false;
	}

	pDO->clearAll();
	for(int i = 0;  i < values.length(); i++)
	{
		CString value = values[i].kACharPtr();
		pDO->addData();
		pDO->setData(i,value);
	}
	actrTransactionManager->endTransaction();
	return true;
}

bool ReportDataHelper::GetObjectDatas( const AcDbObjectId& objId,AcStringArray& values )
{
	if( objId.isNull() ) return false;

	AcTransaction* pTrans = actrTransactionManager->startTransaction();
	if( pTrans == 0 ) return false;

	AcDbObject* pObj;
	if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
	{
		actrTransactionManager->abortTransaction();
		return false;
	}
	DataObject* pDO = DataObject::cast( pObj );
	if( pDO == 0 )
	{
		actrTransactionManager->abortTransaction();
		return false;
	}
	pDO->getAllData(values);
	actrTransactionManager->endTransaction();
	return true;
}

/*
datas的存储方式是:
[名称][名称][名称]
[运转][运转][运转]
[备注][备注][备注]
*/

/*
datasVector的存储方式是:
名称|运转|备注
名称|运转|备注
*/

void ReportDataHelper::ReadDatas( const CString& name, ArrayVector& datasVector,int unitCount )
{
	AcStringArray getDatas;
	AcDbObjectId objId;
	ReporterDataObjectHelper::GetObjectId(name,objId);
	GetObjectDatas(objId,getDatas);
	//AcStringArray strName,strIsRunning,strReason;
	ArrayVector datas;
	int nCount = getDatas.length();
	if(nCount <= 0) return;
	datas.resize( unitCount );
	for(int i = 0;  i < nCount; i = i+2 )
	{
		int j = (i/2)% unitCount;
		datas[j].append(getDatas[i]);
	}

	datasVector.clear();
	for(int i = 0; i < nCount / (2 * unitCount); i++)
	{
		CString temp;
		AcStringArray temps;
		for(int j = 0; j < unitCount; j++)
		{
			temp.Format(_T("%s"),datas[j][i].kACharPtr());
			if(temp == ISNULL) temp = _T("");
			temps.append(temp);
		}
		datasVector.push_back(temps);
	}
}

void ReportDataHelper::WriteDatas( const CString& name,const ArrayVector& datas )
{
	AcDbObjectId objId;
	ReporterDataObjectHelper::GetObjectId(name,objId);
	AcStringArray values;
	for(int i = 0; i < datas[0].length();i++)
	{
		for(int j = 0; j < datas.size(); j++)
		{
			values.append(datas[j][i]);
			values.append(_T("$"));
		}
	}
	SetObjectDatas(objId,values);
}