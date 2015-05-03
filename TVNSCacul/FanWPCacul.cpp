#include "StdAfx.h"
#include "FanWPCacul.h"
//#include "FanWPCaculHelper.h"

#include "VentWaysConfig.h"
#include "../DefGE/WindLibrary.h"
#include "../DefGE/Fan.h"
#include "../DefGE/Chimney.h"

#include <fstream>
#include "AcFStream.h"
//#include "../Sqlite/Sqlite.h"

//#include "../Sqlite/Sqlite.h"
extern CString BuildPath( const CString& dir, const CString& fileName );
extern CString GetAppPathDir();

void FanWPCaculHelper::ReadData(CString way,double Q,double H,cstringArray &names,cstringArray &minQs,cstringArray &maxQs,cstringArray &minHs,cstringArray &maxHs,cstringArray &ratedPowers)
{
	//Sqlite sql; 
	//Sqlite::m_Q = Q;
	//Sqlite::m_H = H;
	//sql.readDatabase( way);

	////cstringArray names,minQs,maxQs,minHs,maxHs;
	//CString dataDirName = _T( "Datas\\" );
	//CString fileName;
	//fileName =BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T( "hd.txt" ) );

	//AcIfstream inFile( fileName );
	//while(!inFile.eof())
	//{
	//	WCHAR name[_MAX_PATH], minQ[_MAX_PATH], maxQ[_MAX_PATH] ,minH[_MAX_PATH] ,maxH[_MAX_PATH],ratedPow[_MAX_PATH];
	//	inFile >> name >> minQ >> maxQ >> minH >> maxH >> ratedPow;
	//	//fin >> m_names[i] >> m_minQs[i] >> m_maxQs[i] >> m_minHs[i] >> m_maxHs[i];
	//	//m_names.push_back(name);
	//	if(NULL != name[0])
	//	{
	//		//cout << "Name:"<< name << "\tQ:" << minQ << "->" << maxQ << "\tH:"
	//		//	<< minH << "->" << maxH << endl;
	//		names.append(name);
	//		minQs.append((minQ));
	//		maxQs.append((maxQ));
	//		minHs.append((minH));
	//		maxHs.append((maxH));
	//		ratedPowers.append(ratedPow);
	//	}
	//}

	//int len = names.length();
	//for (int i = 0; i < len; i++)
	//{
	//	acutPrintf(_T("\nName:%s\tQ:%s->%s\tH:%s->%s"),names[i],minQs[i],maxQs[i],minHs[i],maxHs[i]);

	//}

}
//单列风筒串联，返回的是串联风筒的id数组
AcDbObjectIdArray FanWPCaculHelper::getLinkedChim(const AcDbObjectId& objId)
{
	AcDbObjectIdArray tagObjIds,objIds;
	DrawHelper::GetAllTagGEById( objId, tagObjIds );
	int tagNum = tagObjIds.length();
	for (int i=0; i < tagNum;i++)
	{
		if (ArxUtilHelper::IsEqualType( _T( "Chimney" ), tagObjIds[i]))
		{
			objIds.append(tagObjIds[i]);
		}
	}
	int len = objIds.length();
	if (0 == len)
	{
		AfxMessageBox(_T("风机没有连接风筒"),MB_OK | MB_ICONEXCLAMATION);
		return NULL;
	}

	AcTransaction *pTrans = actrTransactionManager->startTransaction();
	if( 0 == pTrans) return NULL;
	AcDbObject *pObjt;
	if( Acad::eOk != pTrans->getObject(pObjt,objId,AcDb::kForRead)) return NULL;
	LocalFan *pFan = LocalFan::cast(pObjt);
	AcGePoint3d pt = pFan->getInsertPt();
	//actrTransactionManager->endTransaction();

	AcDbObjectIdArray chimneyIds;
	//acutPrintf(_T("\n总风筒段数:%d"),len);
	for(int i = 0 ; i < len; i++)
	{
		//pTrans = actrTransactionManager->startTransaction();
		if( 0 == pTrans ) return NULL;
		if( Acad::eOk != pTrans->getObject(pObjt,objIds[i],AcDb::kForRead)) return NULL;
		Chimney *pChim = Chimney::cast(pObjt);
		if( 0 == pChim )
		{
			actrTransactionManager->abortTransaction();
			break;
		}
		AcGePoint3dArray pts = pChim->getControlPoint();
		//actrTransactionManager->endTransaction();

		int ptsNum = pts.length();
		if (pts[0] == pt && pt != pts[ptsNum-1] && !chimneyIds.contains(objIds[i]))
		{
			chimneyIds.append(objIds[i]);
			pt = pts[ptsNum-1];
			i = -1;
		}

		else if (pts[ptsNum-1] == pt && pt != pts[0] && !chimneyIds.contains(objIds[i]))
		{
			chimneyIds.append(objIds[i]);
			pt = pts[0];
			i = -1;
		}
	}
	actrTransactionManager->endTransaction();
	return chimneyIds;
}

static void QHRandWrite(const AcDbObjectId& objId,const CString& minQStr,const CString& maxQStr,const CString& minHStr,const CString& maxHStr)
{
	CString msgQ = minQStr + _T("~") + maxQStr + _T("(m3/min)");
	CString msgH = minHStr + _T("~") + maxHStr + _T("(Pa)");
	if (!DataHelper::SetPropertyData(objId,_T("风量范围"),msgQ)) return;
	if (!DataHelper::SetPropertyData(objId,_T("风压范围"),msgH)) return;
}

void FanWPCaculHelper::fanWPWrite(const AcDbObjectId& objId,CString name,double Q,double H,double theoryQ,double theoryH,double spaceH,
								  const CString& minQStr,const CString& maxQStr,const CString& minHStr,const CString& maxHStr)
{
	//acutPrintf(_T("\n理论风量：%lf\n理论风压:%lf\n实际风量:%lf\n实际风压:%lf"),theoryQ,theoryH,Q,H);
	//风机型号写入
	if (name.IsEmpty())
	{
		return;
	}
	else
	{
		DataHelper::SetPropertyData( objId, _T( "风机型号" ),  name);
	}
	//风机工作风量写入
	if (0 != Q)
	{
		CString wq;
		wq.Format(_T("%lf"),Q);
		DataHelper::SetPropertyData( objId, _T( "工作风量(m3/s)" ),  wq);
	}
	else
	{
		//AfxMessageBox(_T("请检测漏风备用系数和需风量的值!"),MB_OK | MB_ICONSTOP );
		return;
	}

	//风机工作风压写入
	if (0 != H)
	{
		CString wh;
		wh.Format(_T("%lf"),H);
		DataHelper::SetPropertyData( objId, _T( "工作风压(Pa)" ),  wh);
	}
	else
	{
		//AfxMessageBox(_T("请检测风阻值!"),MB_OK | MB_ICONSTOP );
		return;
	}

	//风机理论风量写入
	if (0 != theoryQ)
	{
		CString tq;
		tq.Format(_T("%lf"),theoryQ);
		DataHelper::SetPropertyData( objId, _T( "理论风量(m3/s)" ),  tq);
	}
	else
	{
		return;
	}

	//风机理论风压写入
	if (0 != theoryH)
	{
		CString th;
		th.Format(_T("%lf"),theoryH);
		DataHelper::SetPropertyData( objId, _T( "理论风压(Pa)" ),  th);
	}
	else
	{
		return;
	}

	//风机风压上限空间写入
	CString spH;
	spH.Format(_T("%lf"),spaceH);
	DataHelper::SetPropertyData( objId, _T( "风压上限空间(Pa)" ), spH);

	//风机所连风筒的出入口风量的写入
	AcDbObjectIdArray sortChims = getLinkedChim(objId);
	CString way;
	DataHelper::GetPropertyData(objId,_T("工作方式"),way);
	CString fanWorkQ;
	fanWorkQ.Format(_T("%lf"),Q);

	CString leakage;

	if (_T("压入式") == way)
	{
		DataHelper::SetPropertyData(sortChims[0],_T("入口风量(m3/s)"),fanWorkQ);
		DataHelper::GetPropertyData(sortChims[0],_T("漏风备用系数"),leakage);
		fanWorkQ.Format(_T("%lf"),(_tstof(fanWorkQ) / _tstof(leakage)));
		DataHelper::SetPropertyData(sortChims[0],_T("出口风量(m3/s)"),fanWorkQ);
	}
	if (_T("抽出式") == way)
	{
		DataHelper::SetPropertyData(sortChims[0],_T("出口风量(m3/s)"),fanWorkQ);
		DataHelper::GetPropertyData(sortChims[0],_T("漏风备用系数"),leakage);
		fanWorkQ.Format(_T("%lf"),(_tstof(fanWorkQ) / _tstof(leakage)));
		DataHelper::SetPropertyData(sortChims[0],_T("入口风量(m3/s)"),fanWorkQ);

	}

	for (int i = 1; i <= sortChims.length(); i++)
	{
		if (_T("压入式") == way)
		{
			DataHelper::SetPropertyData(sortChims[i-1],_T("出口风量(m3/s)"),fanWorkQ);
			DataHelper::SetPropertyData(sortChims[i],_T("入口风量(m3/s)"),fanWorkQ);
			DataHelper::GetPropertyData(sortChims[i],_T("漏风备用系数"),leakage);
			fanWorkQ.Format(_T("%lf"),(_tstof(fanWorkQ) / _tstof(leakage)));
			DataHelper::SetPropertyData(sortChims[i],_T("出口风量(m3/s)"),fanWorkQ);
		}
		if (_T("抽出式") == way)
		{
			DataHelper::SetPropertyData(sortChims[i-1],_T("入口风量(m3/s)"),fanWorkQ);
			DataHelper::SetPropertyData(sortChims[i],_T("出口风量(m3/s)"),fanWorkQ);
			DataHelper::GetPropertyData(sortChims[i],_T("漏风备用系数"),leakage);
			fanWorkQ.Format(_T("%lf"),(_tstof(fanWorkQ) / _tstof(leakage)));
			DataHelper::SetPropertyData(sortChims[i],_T("入口风量(m3/s)"),fanWorkQ);
		}
	}
	
	QHRandWrite(objId,minQStr,maxQStr,minHStr,maxHStr);
}

//多列风筒的并联，且在单列里面还有串联，返回的是最终的风筒风阻和漏风备用系数
ddoubleArray FanWPCaculHelper::getMultiLinkedChim( const AcDbObjectId& objId )
{
	AcDbObjectIdArray objIds;
	DrawHelper::GetAllTagGEById( objId, objIds );
	int len = objIds.length();
	if (0 == len)
	{
		AfxMessageBox(_T("风机没有连接风筒"),MB_OK | MB_ICONEXCLAMATION);
		return NULL;
	}

	AcDbEntity *pEnt;
	if(Acad::eOk != acdbOpenAcDbEntity(pEnt,objId,AcDb::kForRead)) return NULL;
	LocalFan *pFan = LocalFan::cast(pEnt);
	AcGePoint3d insertPt = pFan->getInsertPt();
	pEnt->close();

	//以风机的插入点为基准搜所有与风机连接的风筒
	AcDbObjectIdArray chimLinkFanIds;
	for(int i = 0 ; i < len; i++)
	{
		if(Acad::eOk != acdbOpenAcDbEntity(pEnt,objIds[i],AcDb::kForRead)) return NULL;
		Chimney *pChim = Chimney::cast(pEnt);
		AcGePoint3dArray pts = pChim->getControlPoint();
		pEnt->close();

		int ptsNum = pts.length();
		if ((pts[0] == insertPt || insertPt == pts[ptsNum-1]) && !chimLinkFanIds.contains(objIds[i]))
		{
			chimLinkFanIds.append(objIds[i]);
		}
	}

	//遍历所有与风机相交于一点的风筒
	doubleArray ps,rs;
	int clfNums = chimLinkFanIds.length();
	for (int i = 0; i < clfNums; i++)
	{
		if(Acad::eOk != acdbOpenAcDbEntity(pEnt,chimLinkFanIds[i],AcDb::kForRead)) return NULL;
		Chimney *pChim = Chimney::cast(pEnt);
		AcGePoint3dArray pts = pChim->getControlPoint();
		pEnt->close();

		CString leakStr,windageStr;
		DataHelper::GetPropertyData(chimLinkFanIds[i],_T("漏风备用系数"),leakStr);
		DataHelper::GetPropertyData(chimLinkFanIds[i],_T("风筒风阻"),windageStr);

		double p = _tstof(leakStr);
		double r = _tstof(windageStr);

		AcGePoint3d pt;
		if (insertPt != pts[0])
		{
			pt =  pts[0];
		}

		else if (insertPt != pts[pts.length()-1])
		{
			pt = pts[pts.length()-1];
		}

		AcDbObjectIdArray visitedIds;
		visitedIds.append(chimLinkFanIds[i]);

		//搜索串联的风筒
		for (int j = 0; j < len; j++)
		{
			if(Acad::eOk != acdbOpenAcDbEntity(pEnt,objIds[j],AcDb::kForRead)) return NULL;
			pChim = Chimney::cast(pEnt);
			pts = pChim->getControlPoint();
			pEnt->close();

			int num = pts.length();
			if (pts[0] == pt && !visitedIds.contains(objIds[j]))
			{
				DataHelper::GetPropertyData(objIds[j],_T("漏风备用系数"),leakStr);
				DataHelper::GetPropertyData(objIds[j],_T("风筒风阻"),windageStr);

				p *= _tstof(leakStr);
				r += _tstof(windageStr);

				visitedIds.append(objIds[j]);
				pt = pts[num - 1];
				j = -1;
			}

			else if (pts[num - 1] == pt && !visitedIds.contains(objIds[j]))
			{
				DataHelper::GetPropertyData(objIds[j],_T("漏风备用系数"),leakStr);
				DataHelper::GetPropertyData(objIds[j],_T("风筒风阻"),windageStr);

				p *= _tstof(leakStr);
				r += _tstof(windageStr);

				visitedIds.append(objIds[j]);
				pt = pts[0];
				j = -1;
			}

		}

		ps.append(p);
		rs.append(r);

	}

	ddoubleArray prs;
	prs.append(ps);
	prs.append(rs);
	return prs;

}

void FanWPCaculHelper::FanWPCacul(const AcDbObjectIdArray& fanIds,CString ttunelWay,CString method,CString rqQStr,const chooseType& chFanType )
{
	//acutPrintf(_T("\n风机参数计算测试...."));

	for (int i = 0 ; i < fanIds.length(); i++)
	{
		CString fanWay;
		if( !DataHelper::GetPropertyData(fanIds[i],_T("工作方式"),fanWay) );
		if (fanWay.IsEmpty())
		{
			AfxMessageBox(_T("选型失败，请查看风机属性！"),MB_OK | MB_ICONEXCLAMATION);
			return;
		}
	}
	if (SINGLE_FAN_CHIM == method)
	{
		singleQHCacul(fanIds,ttunelWay,rqQStr,chFanType);
	}

	if (AXILIARY_RAM == method)
	{
		axilQHCacul(fanIds,ttunelWay,rqQStr,chFanType);
	}

	if (HYBIRD_VENT == method || DISCONTI_RAM == method || FAN_IN_SERRIER == method || DRILLING_VENT == method)
	{
		seriesQHCacul(fanIds,ttunelWay,rqQStr,chFanType);
	}

	if (SINGLE_DOUBLE_ROW == method)
	{
		//AfxMessageBox(_T("暂不支持"));
		//return;
		mutiserQHCacul(fanIds,ttunelWay,rqQStr,chFanType);
	}

	if ( LIBRARY_VENT == method || CABINET_VENT == method)
	{
		libVentQHCacul(fanIds,ttunelWay,rqQStr,chFanType);
	}

}

void FanWPCaculHelper::classifyFanByWay( AcDbObjectIdArray& fanIds, AcDbObjectIdArray& pushInIds, AcDbObjectIdArray& pullOutIds )
{
	int len = fanIds.length();
	for (int i = 0; i < len; i++)
	{
		CString wayStr;
		DataHelper::GetPropertyData( fanIds[i], _T( "工作方式" ), wayStr );
		if (_T("压入式") == wayStr)
		{
			pushInIds.append(fanIds[i]);
		}

		else if (_T("抽出式") == wayStr)
		{
			pullOutIds.append(fanIds[i]);
		}

		else
		{
			//AfxMessageBox(_T("有局扇的工作方式没有设置!"),MB_OK | MB_ICONSTOP);
			return;
		}
	}

}

void FanWPCaculHelper::RealParamCaculQ( CString &name, const cstringArray& names,const cstringArray& minQs,const cstringArray& maxQs,const cstringArray& minHs,const cstringArray& maxHs ,double theoryQ,double r,double p,double& workQ,double& workH,double& spaceH,CString& minQStr,CString& maxQStr,CString& minHStr,CString& maxHStr,const cstringArray& ratedPowers,CString& ratedPower)
{
	int num = names.length();
	//workQ = 0; //风机工作风量
	//workH = 0; //风机工作风压
	//spaceH = 0; //风压上限空间
	double deltaQ = 1e100;
	for (int i = 0; i < num; i++)
	{
		//acutPrintf(_T("\nName:%s\tQ:%s->%s\tH:%s->%s"),names[i],minQs[i],maxQs[i],minHs[i],maxHs[i]);
		double minQ = _tstof(minQs[i])/60;
		double maxQ = _tstof(maxQs[i])/60;
		double minH = _tstof(minHs[i]);
		double maxH = _tstof(maxHs[i]);
		double m = (maxH - minH) / (minQ - maxQ);
		double n = m * m + 4 * r * (minH*minQ - maxH*maxQ)/ (p *(minQ - maxQ));
		double Qf = (m + sqrtf(n)) * p * 0.5 / r;
		//acutPrintf(_T("\nn:%lf,sqrtfn:%lf"),n,sqrtf(n));
		//acutPrintf(_T("\ndeltaQ:%lf"),Qf - theoryQ);
		//求工作风量和理论风量差值，取差值最小的型号作为所选风机
		//理论上工作风量是大于理论风量的，这里为了保险加上绝对值
		if ((deltaQ > (Qf - theoryQ)) && ((Qf - theoryQ) >= 0))
		{
			deltaQ = abs((Qf - theoryQ));
			workQ = Qf;
			workH = r * Qf * Qf / p;
			name = names[i];
			spaceH = maxH - workH;
			minQStr = minQs[i];
			maxQStr = maxQs[i];
			minHStr = minHs[i];
			maxHStr = maxHs[i];
			ratedPower = ratedPowers[i];
			//acutPrintf(_T("\nminQ:%lf,maxQ:%lf,minH:%lf,maxH:%lf"),minQ,maxQ,minH,maxH);
			//acutPrintf(_T("\nworkQ:%lf,workH:%lf,spaceH:%lf"),workQ,workH,spaceH);
		}
	}

}

void FanWPCaculHelper::RealParamCaculP( CString &name, const cstringArray& names,const cstringArray& minQs,const cstringArray& maxQs,const cstringArray& minHs,const cstringArray& maxHs ,double theoryQ,double r,double p,double& workQ,double& workH,double& spaceH,CString& minQStr,CString& maxQStr,CString& minHStr,CString& maxHStr,const cstringArray& ratedPowers,CString& ratedPower)
{
	int num = names.length();

	double minPower = 1e100;
	double deltaQ = 1e100;
	double minHQ = 1e100;

	for (int i = 0; i < num; i++)
	{
		//acutPrintf(_T("\nName:%s\tQ:%s->%s\tH:%s->%s"),names[i],minQs[i],maxQs[i],minHs[i],maxHs[i]);
		double minQ = _tstof(minQs[i])/60;
		double maxQ = _tstof(maxQs[i])/60;
		double minH = _tstof(minHs[i]);
		double maxH = _tstof(maxHs[i]);
		double m = (maxH - minH) / (minQ - maxQ);
		double n = m * m + 4 * r * (minH*minQ - maxH*maxQ)/ (p *(minQ - maxQ));
		double Qf = (m + sqrtf(n)) * p * 0.5 / r;
		CString powerStrAll = ratedPowers[i];

		CString powerLeft,powerRight;
		double power;
		if (-1 != powerStrAll.Find(_T("*")))
		{
			//字符串的分割
			AfxExtractSubString( powerLeft, (LPCTSTR)powerStrAll, 0, '*');
			AfxExtractSubString( powerRight, (LPCTSTR)powerStrAll, 1, '*');
			power = _tstof(powerLeft) * _tstof(powerRight);
		}
		else if(-1 == powerStrAll.Find(_T("*")))
		{
			power = _tstof(powerStrAll);
		}

		//double hq =  Qf * r * Qf * Qf / p;
		//acutPrintf(_T("\nhq:%.2lf"),hq);
		//if ( minHQ >= hq)
		//{
		//		if( minHQ == hq )
		//		{
		//			if ((deltaQ > (Qf - theoryQ)) && ((Qf - theoryQ) >= 0))
		//			{
		//				deltaQ = abs((Qf - theoryQ));
		//				workQ = Qf;
		//				workH = r * Qf * Qf / p;
		//				name = names[i];
		//				spaceH = maxH - workH;
		//				minQStr = minQs[i];
		//				maxQStr = maxQs[i];
		//				minHStr = minHs[i];
		//				maxHStr = maxHs[i];
		//				ratedPower = ratedPowers[i];
		//			}
		//		}
		//		else if( minHQ > hq )
		//		{
		//			minHQ = hq;
		//			workQ = Qf;
		//			workH = r * Qf * Qf / p;
		//			name = names[i];
		//			spaceH = maxH - workH;
		//			minQStr = minQs[i];
		//			maxQStr = maxQs[i];
		//			minHStr = minHs[i];
		//			maxHStr = maxHs[i];
		//			ratedPower = ratedPowers[i];
		//		}

		//}

		if ( minPower >= power )
		{
			//acutPrintf(_T("\n功率数:%lf"),power);
			if(minPower == power)
			{
				if ((deltaQ > (Qf - theoryQ)) && ((Qf - theoryQ) >= 0))
				{
					deltaQ = abs((Qf - theoryQ));
					workQ = Qf;
					workH = r * Qf * Qf / p;
					name = names[i];
					spaceH = maxH - workH;
					minQStr = minQs[i];
					maxQStr = maxQs[i];
					minHStr = minHs[i];
					maxHStr = maxHs[i];
					ratedPower = ratedPowers[i];
				}
			}
			else if(minPower > power)
			{
				minPower = power;
				workQ = Qf;
				workH = r * Qf * Qf / p;
				name = names[i];
				spaceH = maxH - workH;
				minQStr = minQs[i];
				maxQStr = maxQs[i];
				minHStr = minHs[i];
				maxHStr = maxHs[i];
				ratedPower = ratedPowers[i];
			}
			//acutPrintf(_T("\nminQ:%lf,maxQ:%lf,minH:%lf,maxH:%lf"),minQ,maxQ,minH,maxH);
			//acutPrintf(_T("\nworkQ:%lf,workH:%lf,spaceH:%lf"),workQ,workH,spaceH);
		}
	}

}

void FanWPCaculHelper::singleQHCacul( const AcDbObjectIdArray& fanIds,CString ttunelWay,CString rqQStr ,const chooseType& chFanType)
{
	if (rqQStr.IsEmpty())
	{
		AfxMessageBox(_T("需风量没有计算!"),MB_OK | MB_ICONSTOP);
		return;
	}
	int fanNum = fanIds.length();

	if (fanNum > 1) 
	{
		AfxMessageBox(_T("风机的数目和通风方式不配套"),MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	if (_T("长压短抽") == ttunelWay || _T("长抽短压") == ttunelWay)
	{
		AfxMessageBox(_T("单机单风筒没有所选的通风方法"),MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	SingleQHCacul *singleCacul = new SingleQHCacul(fanIds[0],_tstof(rqQStr),chFanType);
	singleCacul->caculateQH();
}


void FanWPCaculHelper::axilQHCacul( const AcDbObjectIdArray &fanIds,CString ttunelWay,CString rqQStr,const chooseType& chFanType )
{
	cheakMethod(fanIds,rqQStr);
	AxilQHCacul *axilCacul = new AxilQHCacul(fanIds,_tstof(rqQStr),chFanType);
	axilCacul->caculateQH(ttunelWay);
}


void FanWPCaculHelper::seriesQHCacul( const AcDbObjectIdArray& fanIds,CString ttunelWay,CString rqQStr,const chooseType& chFanType )
{
	cheakMethod(fanIds,rqQStr);
	SeriesQHCacul *seriesCacul = new SeriesQHCacul(fanIds,_tstof(rqQStr),chFanType);
	seriesCacul->caculateQH(ttunelWay);

}

void FanWPCaculHelper::mutiserQHCacul( const AcDbObjectIdArray& fanIds,CString ttunelWay,CString rqQStr,const chooseType& chFanType )
{
	cheakMethod(fanIds,rqQStr);
	MutiserQHCacul *mutiCacul = new MutiserQHCacul(fanIds,_tstof(rqQStr),chFanType);
	mutiCacul->caculateQH(ttunelWay);
}

void FanWPCaculHelper::libVentQHCacul(const AcDbObjectIdArray& fanIds,CString ttunelWay,CString rqQStr,const chooseType& chFanType )
{
	cheakMethod(fanIds,rqQStr);
	LibVentQHCacul *libCacul = new LibVentQHCacul(fanIds,_tstof(rqQStr),chFanType);
	libCacul->caculateQH(ttunelWay);
}

int FanWPCaculHelper::getMaxClassify(const AcDbObjectIdArray& objIds,CString randField )
{
	int objNum = objIds.length();
	int classifyIntMax = 0;
	bool randHaszero = false;
	for (int i = 0; i < objNum; i++)
	{
		CString classsifyStr;
		if(false == DataHelper::GetPropertyData(objIds[i],randField,classsifyStr)) return 0;
		if (classifyIntMax < _ttoi(classsifyStr))
		{
			classifyIntMax = _ttoi(classsifyStr);
		}
		if (_ttoi(classsifyStr) == 0)
		{
			randHaszero = true;
		}
	}

	if (false == randIsReasonable(objIds,randField,classifyIntMax) || randHaszero)
	{
		classifyIntMax = 0;
	}

	return classifyIntMax;

}

bool FanWPCaculHelper::randIsReasonable(const AcDbObjectIdArray& objIds,CString randField, int classifyIntMax )
{
	//acutPrintf(_T("\nxxxxxx"));
	int objNum = objIds.length();
	bool isReasonable = false;
	int ranNum = objNum;
	for(int j = 1; j <= classifyIntMax; j++)
	{
		int jRanNum = 0;
		for(int i = 0; i < objNum; i++)
		{
			CString classsifyStr;
			if(false == DataHelper::GetPropertyData(objIds[i],randField,classsifyStr)) return false;
			if (j == _ttoi(classsifyStr))
			{
				jRanNum += 1;
			}
		}
		if (jRanNum == 0)
		{
			isReasonable = false;
			return false;
		}
		ranNum = ranNum - jRanNum + 1;
	}
	//acutPrintf(_T("\nranNum:%d,classifyIntMax:%d,fanNum:%d"),ranNum,classifyIntMax,objNum);
	if (ranNum == classifyIntMax)
	{
		isReasonable = true;
	}
	return isReasonable;

}

void FanWPCaculHelper::cheakMethod( const AcDbObjectIdArray& fanIds,CString rqQStr )
{
	if (rqQStr.IsEmpty())
	{
		AfxMessageBox(_T("需风量没有计算!"),MB_OK | MB_ICONSTOP);
		return;
	}
	int fanNum = fanIds.length();

	//if (fanNum < 2) 
	//{
	//	AfxMessageBox(_T("风机的数目和通风方式不配套"),MB_OK | MB_ICONEXCLAMATION);
	//	return;
	//}
}

SingleQHCacul::SingleQHCacul( AcDbObjectId fanObjId,double reqQ,const chooseType& chFanType ):m_fanObjId(fanObjId),m_reqQ(reqQ),m_chFanType(chFanType)
{

}

SingleQHCacul::~SingleQHCacul()
{

}

void SingleQHCacul::caculateQH()
{
	AcDbObjectIdArray chimsObjIds = FanWPCaculHelper::getLinkedChim(m_fanObjId);
	//acutPrintf(_T("\n相连风筒数:%d"),chimsObjIds.length());
	if( 0 == chimsObjIds.length()) return;

	CString way;
	if (! DataHelper::GetPropertyData(m_fanObjId,_T("工作方式"),way)) return;

	int len = chimsObjIds.length();

	//if(0 == len) return;
	double p  = 1,r = 0;
	for(int i = len-1; i >= 0; i--)
	{
		CString leakStr,windageStr;
		if(!DataHelper::GetPropertyData(chimsObjIds[i],_T("漏风备用系数"),leakStr)) return;
		if(!DataHelper::GetPropertyData(chimsObjIds[i],_T("风筒风阻"),windageStr)) return;

		double p1 = _tstof(leakStr);
		double r1 = _tstof(windageStr);
		//acutPrintf(_T("\n漏风备用系数:%lf\n风筒风阻:%lf"),p1,r1);

		p = p * p1;
		r = r + r1;

	}

	m_workQ = p * m_reqQ;
	m_workH = p * r * m_reqQ * m_reqQ;
	acutPrintf(_T("\n风量：%.2lfm^3/min\t风压：%.2lfPa"),m_workQ*60,m_workH);

	cstringArray names,minQs,maxQs,minHs,maxHs,ratedPowers;
	FanWPCaculHelper::ReadData(way,m_workQ*60,m_workH,names,minQs,maxQs,minHs,maxHs,ratedPowers);
	
	int num = names.length();
	if (0 == num)
	{
		AfxMessageBox(_T("对局扇性能参数要求高\n实际技术条件不能满足"),MB_OK | MB_ICONSTOP);
		if (0 != m_workQ)
		{
			CString tq;
			tq.Format(_T("%lf"),m_workQ);
			DataHelper::SetPropertyData( m_fanObjId, _T( "理论风量(m3/s)" ),  tq);
		}

		//风机理论风压写入
		if (0 != m_workH)
		{
			CString th;
			th.Format(_T("%lf"),m_workH);
			DataHelper::SetPropertyData( m_fanObjId, _T( "理论风压(Pa)" ),  th);
		}

		return;
	}
	CString name;
	double workQ = 0; //风机工作风量
	double workH = 0; //风机工作风压
	double spaceH = 0; //风压上限空间

	//CString minQStr,maxQStr,minHStr,maxHStr,ratedPower;
	//FanWPCaculHelper::RealParamCaculQ(name,names,minQs,maxQs,minHs,maxHs,m_workQ,r,p,workQ,workH,spaceH,minQStr,maxQStr,minHStr,maxHStr,ratedPowers,ratedPower);
	//acutPrintf(_T("\n++++++++\n风量比较结果\n"));
	//acutPrintf(_T("\n最小风量:%s"),minQStr);
	//acutPrintf(_T("\n最大风量:%s"),maxQStr);
	//acutPrintf(_T("\n最小风压:%s"),minHStr);
	//acutPrintf(_T("\n最大风压:%s"),maxHStr);
	//acutPrintf(_T("\n额定功率:%s"),ratedPower);
	//acutPrintf(_T("\nH*Q:%.2lf"),workQ*workH);
	//acutPrintf(_T("\n工作风量:%.2lf"),workQ*60);
	//acutPrintf(_T("\n工作风压:%.2lf"),workH);
	//acutPrintf(_T("\n理论风量:%.2lf"),m_workQ*60);
	//acutPrintf(_T("\n理论风压:%.2lf"),m_workH);
	//acutPrintf(_T("\n上限空间:%.2lf"),spaceH);
	//itDatas.nameQ = name;
	//itDatas.qQ = minQStr + _T("~") + maxQStr;
	//itDatas.hQ = minHStr + _T("~") + maxHStr;
	//itDatas.pQ = ratedPower;

	CString minQStrP,maxQStrP,minHStrP,maxHStrP,ratedPowerP;
	switch(m_chFanType)
	{
		case POWER:
			FanWPCaculHelper::RealParamCaculP(name,names,minQs,maxQs,minHs,maxHs,m_workQ,r,p,workQ,workH,spaceH,minQStrP,maxQStrP,minHStrP,maxHStrP,ratedPowers,ratedPowerP);
			break;
		case QUANTITY:
			FanWPCaculHelper::RealParamCaculQ(name,names,minQs,maxQs,minHs,maxHs,m_workQ,r,p,workQ,workH,spaceH,minQStrP,maxQStrP,minHStrP,maxHStrP,ratedPowers,ratedPowerP);
			break;
		default:
			break;
	}
	//acutPrintf(_T("\n++++++++\n功率比较结果\n"));
	//acutPrintf(_T("\n最小风量:%s"),minQStrP);
	//acutPrintf(_T("\n最大风量:%s"),maxQStrP);
	//acutPrintf(_T("\n最小风压:%s"),minHStrP);
	//acutPrintf(_T("\n最大风压:%s"),maxHStrP);
	//acutPrintf(_T("\n额定功率:%s"),ratedPowerP);
	//acutPrintf(_T("\nH*Q:%.2lf"),workQ*workH);
	//acutPrintf(_T("\n工作风量:%.2lf"),workQ*60);
	//acutPrintf(_T("\n工作风压:%.2lf"),workH);
	//acutPrintf(_T("\n理论风量:%.2lf"),m_workQ*60);
	//acutPrintf(_T("\n理论风压:%.2lf"),m_workH);
	//acutPrintf(_T("\n上限空间:%.2lf"),spaceH);
	//itDatas.nameP = name;
	//itDatas.qP = minQStrP + _T("~") + maxQStrP;
	//itDatas.hP = minHStrP + _T("~") + maxHStrP;
	//itDatas.pP = ratedPowerP;

	//FanChooseDlg *chooseDlg = new FanChooseDlg(NULL,itDatas);
	//chooseDlg->DoModal();
	FanWPCaculHelper::fanWPWrite(m_fanObjId,name,workQ,workH,m_workQ,m_workH,spaceH,minQStrP,maxQStrP,minHStrP,maxHStrP);
}

AxilQHCacul::AxilQHCacul( AcDbObjectIdArray fanObjIds,double reqQ,const chooseType& chFanType ):m_fanObjIds(fanObjIds),m_reqQ(reqQ),m_chFanType(chFanType)
{

}

AxilQHCacul::~AxilQHCacul()
{

}

void AxilQHCacul::caculateQH(CString way)
{
	AcDbObjectIdArray pushInIds,pullOutIds;
	FanWPCaculHelper::classifyFanByWay(m_fanObjIds,pushInIds,pullOutIds);

	if (m_fanObjIds.isEmpty())
	{
		AfxMessageBox(_T("没有安装局扇!"),MB_OK | MB_ICONSTOP);
		return;
	}

	if (pullOutIds.isEmpty() && pushInIds.isEmpty())
	{
		AfxMessageBox(_T("局扇没有设置工作方式!"),MB_OK | MB_ICONSTOP);
		return;
	}

	if (pullOutIds.isEmpty() &&  (_T("抽出式") == way) )
	{
		AfxMessageBox(_T("没有设置抽出式风机!"),MB_OK | MB_ICONSTOP);
		return;
	}

	if (pushInIds.isEmpty() &&  (_T("压入式") == way) )
	{
		AfxMessageBox(_T("没有设置压入式风机!"),MB_OK | MB_ICONSTOP);
		return;
	}

	int fanNum = m_fanObjIds.length();
	int classifyIntMax = 0;

	//acutPrintf(_T("\n抽出式:%d"),pullOutIds.length());
	//acutPrintf(_T("\n压入式:%d"),pushInIds.length());

	if ((pullOutIds.isEmpty() || pushInIds.isEmpty()) && (_T("压入式") == way || _T("抽出式") == way))
	{
		classifyIntMax = FanWPCaculHelper::getMaxClassify(m_fanObjIds,_T("风机级别"));
		if( false == choseFansByRand(m_fanObjIds,classifyIntMax,0)) return;
	}

	else if (_T("长压短抽") == way)
	{
		classifyIntMax = FanWPCaculHelper::getMaxClassify(pullOutIds,_T("风机级别"));
		if( false == choseFansByRand(pullOutIds,classifyIntMax,0)) return;
		classifyIntMax = FanWPCaculHelper::getMaxClassify(pushInIds,_T("风机级别"));
		if( false == choseFansByRand(pushInIds,classifyIntMax,1)) return;
	}

	else if (_T("长抽短压") == way)
	{
		classifyIntMax = FanWPCaculHelper::getMaxClassify(pushInIds,_T("风机级别"));
		if( false == choseFansByRand(pushInIds,classifyIntMax,0)) return;
		classifyIntMax = FanWPCaculHelper::getMaxClassify(pullOutIds,_T("风机级别"));
		if( false == choseFansByRand(pullOutIds,classifyIntMax,1)) return;
	}

	else
	{
		//AfxMessageBox(_T("通风方式与通风方法不配套"),MB_OK | MB_ICONSTOP);
		return;
	}
}

bool AxilQHCacul::choseFansFromDB(AcDbObjectIdArray fanObjtIds,int flag)
{
	AcDbObjectIdArray chimsObjIds = FanWPCaculHelper::getLinkedChim(fanObjtIds[0]);
	CString way;
	DataHelper::GetPropertyData(fanObjtIds[0],_T("工作方式"),way);

	int len = chimsObjIds.length();
	if(0 == len) return false;
	int fanNum = fanObjtIds.length();
	double p = 1,r = 0;
	for(int i = len-1; i >= 0; i--)
	{
		CString leakStr,windageStr;
		DataHelper::GetPropertyData(chimsObjIds[i],_T("漏风备用系数"),leakStr);
		DataHelper::GetPropertyData(chimsObjIds[i],_T("风筒风阻"),windageStr);

		double p1 = _tstof(leakStr);
		double r1 = _tstof(windageStr);

		p = p1 * p;
		r = r1 + r;

	}

	m_workQ = p * m_reqQ / fanNum;
	m_workH = p * r * m_reqQ * m_reqQ / (fanNum * fanNum);
	acutPrintf(_T("\n风量：%.2lfm^3/min\t风压：%.2lfPa"),m_workQ*60,m_workH);

	cstringArray names,minQs,maxQs,minHs,maxHs,ratedPowers;
	FanWPCaculHelper::ReadData(way,m_workQ*60,m_workH,names,minQs,maxQs,minHs,maxHs,ratedPowers);

	int num = names.length();
	if (0 == num)
	{
		AfxMessageBox(_T("对局扇性能参数要求高\n实际技术条件不能满足"),MB_OK | MB_ICONSTOP);
		return false;
	}
	CString name;
	double workQ = 0; //风机工作风量
	double workH = 0; //风机工作风压
	double spaceH = 0; //风压上限空间

	//CString minQStr,maxQStr,minHStr,maxHStr;
	//FanWPCaculHelper::RealParamCacul(name,names,minQs,maxQs,minHs,maxHs,m_workQ,r,p,workQ,workH,spaceH,minQStr,maxQStr,minHStr,maxHStr);

	//CString minQStr,maxQStr,minHStr,maxHStr,ratedPower;
	//FanWPCaculHelper::RealParamCaculQ(name,names,minQs,maxQs,minHs,maxHs,m_workQ,r,p,workQ,workH,spaceH,minQStr,maxQStr,minHStr,maxHStr,ratedPowers,ratedPower);
	CString minQStrP,maxQStrP,minHStrP,maxHStrP,ratedPowerP;
	switch(m_chFanType)
	{
	case POWER:
		FanWPCaculHelper::RealParamCaculP(name,names,minQs,maxQs,minHs,maxHs,m_workQ,r,p,workQ,workH,spaceH,minQStrP,maxQStrP,minHStrP,maxHStrP,ratedPowers,ratedPowerP);
		break;
	case QUANTITY:
		FanWPCaculHelper::RealParamCaculQ(name,names,minQs,maxQs,minHs,maxHs,m_workQ,r,p,workQ,workH,spaceH,minQStrP,maxQStrP,minHStrP,maxHStrP,ratedPowers,ratedPowerP);
		break;
	default:
		break;
	}

	for (int i = 0; i < fanNum; i++)
	{
		FanWPCaculHelper::fanWPWrite(fanObjtIds[i],name,workQ,workH,m_workQ,m_workH,spaceH ,minQStrP,maxQStrP,minHStrP,maxHStrP );
		//acutPrintf(_T("\n%d->工作参数%f--%f"),i,workQ,workH);
	}

	AcDbObjectId tunelId;
	DrawHelper::GetHostGE(fanObjtIds[0],tunelId);

	CString areStr;
	DataHelper::GetPropertyData(tunelId,_T("断面面积"),areStr);

	if (areStr.IsEmpty())
	{
		AfxMessageBox(_T("巷道的断面面积没有给定!"),MB_OK | MB_ICONSTOP);
		return false;
	}
	if ( 0 == flag )
	{
		m_reqQ = fanNum * workQ * 1.5;	
	}

	else if( 1 ==flag )
	{
		m_reqQ = fanNum * workQ + 0.25 * _tstof(areStr);
	}

	return true;
}

bool AxilQHCacul::choseFansByRand( AcDbObjectIdArray objIds ,int classifyIntMax,int flag)
{
	bool comleted = true;
	if ( 0 == classifyIntMax )
	{
		AfxMessageBox( _T("风机级别设置有误!"),MB_OK | MB_ICONSTOP );
		comleted = false;
		return comleted;
	}
	int fanNum = objIds.length();

	//acutPrintf(_T("\n风机最大级别:%d"),classifyIntMax);
	//acutPrintf(_T("\n风机数目:%d"),fanNum);
	for (int i = 1; i <= classifyIntMax; i++)
	{
		AcDbObjectIdArray fanObjids;
		for (int j = 0; j < fanNum; j++)
		{
			CString classsifyStr;
			DataHelper::GetPropertyData(objIds[j],_T("风机级别"),classsifyStr);
			if ( i == _ttoi(classsifyStr) )
			{
				fanObjids.append(objIds[j]);
			}
		}
		if(false == choseFansFromDB(fanObjids,flag))
		{
			comleted = false;

			return comleted;
		}
	}
	return comleted;
}

SeriesQHCacul::SeriesQHCacul( AcDbObjectIdArray fanObjIds,double reqQ,const chooseType& chFanType ):AxilQHCacul(fanObjIds,reqQ,chFanType),m_fanObjIds(fanObjIds),m_reqQ(reqQ),m_chFanType(chFanType)
{

}

SeriesQHCacul::~SeriesQHCacul()
{

}

MutiserQHCacul::MutiserQHCacul( AcDbObjectIdArray fanObjIds,double reqQ,const chooseType& chFanType ):AxilQHCacul(fanObjIds,reqQ,chFanType),m_fanObjIds(fanObjIds),m_reqQ(reqQ),m_chFanType(chFanType)
{

}

MutiserQHCacul::~MutiserQHCacul()
{

}

void MutiserQHCacul::caculateQH(CString way)
{
	AcDbObjectIdArray pushInIds,pullOutIds;
	FanWPCaculHelper::classifyFanByWay(m_fanObjIds,pushInIds,pullOutIds);

	if (m_fanObjIds.isEmpty())
	{
		AfxMessageBox(_T("没有安装局扇!"),MB_OK | MB_ICONSTOP);
		return;
	}

	if (pullOutIds.isEmpty() && pushInIds.isEmpty())
	{
		AfxMessageBox(_T("局扇没有设置工作方式!"),MB_OK | MB_ICONSTOP);
		return;
	}

	if (pullOutIds.isEmpty() &&  (_T("抽出式") == way) )
	{
		AfxMessageBox(_T("没有设置抽出式风机!"),MB_OK | MB_ICONSTOP);
		return;
	}

	if (pushInIds.isEmpty() &&  (_T("压入式") == way) )
	{
		AfxMessageBox(_T("没有设置压入式风机!"),MB_OK | MB_ICONSTOP);
		return;
	}

	int fanNum = m_fanObjIds.length();
	int classifyIntMax = 0;

	//acutPrintf(_T("\n抽出式:%d"),pullOutIds.length());
	//acutPrintf(_T("\n压入式:%d"),pushInIds.length());

	if ((pullOutIds.isEmpty() || pushInIds.isEmpty()) && (_T("压入式") == way || _T("抽出式") == way))
	{
		classifyIntMax = FanWPCaculHelper::getMaxClassify(m_fanObjIds,_T("风机级别"));
		if( false == choseFansByRand(m_fanObjIds,classifyIntMax,0)) return;
	}

	else if (_T("长压短抽") == way)
	{
		classifyIntMax = FanWPCaculHelper::getMaxClassify(pullOutIds,_T("风机级别"));
		if( false == choseFansByRand(pullOutIds,classifyIntMax,0)) return;
		classifyIntMax = FanWPCaculHelper::getMaxClassify(pushInIds,_T("风机级别"));
		if( false == choseFansByRand(pushInIds,classifyIntMax,1)) return;
	}

	else if (_T("长抽短压") == way)
	{
		classifyIntMax = FanWPCaculHelper::getMaxClassify(pushInIds,_T("风机级别"));
		if( false == choseFansByRand(pushInIds,classifyIntMax,0)) return;
		classifyIntMax = FanWPCaculHelper::getMaxClassify(pullOutIds,_T("风机级别"));
		if( false == choseFansByRand(pullOutIds,classifyIntMax,1)) return;
	}

	else
	{
		AfxMessageBox(_T("通风方式与通风方法不配套"),MB_OK | MB_ICONSTOP);
		return;
	}

}

bool MutiserQHCacul::choseFansFromDB( AcDbObjectIdArray fanObjtIds,int flag )
{
	ddoubleArray prs = FanWPCaculHelper::getMultiLinkedChim(fanObjtIds[0]);
	if(prs.isEmpty()) return false;
	doubleArray ps = prs[0];
	doubleArray rs = prs[1];
	int num = ps.length();

	CString way;
	DataHelper::GetPropertyData(fanObjtIds[0],_T("工作方式"),way);

	doubleArray R;
	//coef.setLogicalLength(num);
	R.setLogicalLength(num);
	R.setAll(0);
	double coef = 0;
	//acutPrintf(_T("\n并联的风筒数:%d\nR:%d"),num,R.length());
	for(int i = 0; i < num; i++)
	{
		for (int j = 0; j < num; j++)
		{
			if (0 == rs[j])
			{
				AfxMessageBox(_T("参数不全，请检查各图元的参数!"),MB_OK | MB_ICONSTOP);
				return false;
			}
			R[i] += sqrtf(rs[i]/rs[j]);
		}
		coef += ps[i] / R[i];
		//acutPrintf(_T("\n漏风:%lf\t风阻:%lf\t系数:%lf"),ps[i],rs[i],coef);

	}

	int fanNum = fanObjtIds.length();

	m_workQ = coef * m_reqQ / fanNum;
	m_workH = rs[0] * ps[0] * pow(m_reqQ/R[0],2) / (fanNum * fanNum);
	acutPrintf(_T("\n风量：%.2lfm^3/min\t风压：%.2lfPa"),m_workQ*60,m_workH);

	cstringArray names,minQs,maxQs,minHs,maxHs,ratedPowers;
	FanWPCaculHelper::ReadData(way,m_workQ*60,m_workH,names,minQs,maxQs,minHs,maxHs,ratedPowers);

	int dbFannum = names.length();
	if (0 == dbFannum)
	{
		AfxMessageBox(_T("对局扇性能参数要求高\n实际技术条件不能满足"),MB_OK | MB_ICONSTOP);
		return false;
	}
	CString name;
	double workQ = 0; //风机工作风量
	double workH = 0; //风机工作风压
	double spaceH = 0; //风压上限空间
	double r = rs[0] / (R[0] * R[0]);

	//CString minQStr,maxQStr,minHStr,maxHStr;
	//FanWPCaculHelper::RealParamCacul(name,names,minQs,maxQs,minHs,maxHs,m_workQ,r,coef,workQ,workH,spaceH,minQStr,maxQStr,minHStr,maxHStr);

	//CString minQStr,maxQStr,minHStr,maxHStr,ratedPower;
	//FanWPCaculHelper::RealParamCaculQ(name,names,minQs,maxQs,minHs,maxHs,m_workQ,r,coef,workQ,workH,spaceH,minQStr,maxQStr,minHStr,maxHStr,ratedPowers,ratedPower);
	CString minQStrP,maxQStrP,minHStrP,maxHStrP,ratedPowerP;
	switch(m_chFanType)
	{
	case POWER:
		FanWPCaculHelper::RealParamCaculP(name,names,minQs,maxQs,minHs,maxHs,m_workQ,r,coef,workQ,workH,spaceH,minQStrP,maxQStrP,minHStrP,maxHStrP,ratedPowers,ratedPowerP);
		break;
	case QUANTITY:
		FanWPCaculHelper::RealParamCaculQ(name,names,minQs,maxQs,minHs,maxHs,m_workQ,r,coef,workQ,workH,spaceH,minQStrP,maxQStrP,minHStrP,maxHStrP,ratedPowers,ratedPowerP);
		break;
	default:
		break;
	}

	for (int i = 0; i < fanNum; i++)
	{
		FanWPCaculHelper::fanWPWrite(fanObjtIds[i],name,workQ,workH,m_workQ,m_workH,spaceH,minQStrP,maxQStrP,minHStrP,maxHStrP );
		//acutPrintf(_T("\n%d->工作参数%f--%f"),i,workQ,workH);
	}

	AcDbObjectId tunelId;
	DrawHelper::GetHostGE(fanObjtIds[0],tunelId);

	CString areStr;
	DataHelper::GetPropertyData(tunelId,_T("断面面积"),areStr);

	if (areStr.IsEmpty())
	{
		AfxMessageBox(_T("巷道的断面面积没有给定!"),MB_OK | MB_ICONSTOP);
		return false;
	}
	if ( 0 == flag )
	{
		m_reqQ = fanNum * workQ * 1.5;	
	}

	else if( 1 ==flag )
	{
		m_reqQ = fanNum * workQ + 0.25 * _tstof(areStr);
	}

	return true;

}

bool MutiserQHCacul::choseFansByRand( AcDbObjectIdArray objIds, int classifyIntMax,int flag )
{
	bool comleted = true;
	if ( 0 == classifyIntMax )
	{
		AfxMessageBox( _T("风机级别设置有误!"),MB_OK | MB_ICONSTOP );
		comleted = false;
		return comleted;
	}
	int fanNum = objIds.length();

	//acutPrintf(_T("\n风机最大级别:%d"),classifyIntMax);

	for (int i = 1; i <= classifyIntMax; i++)
	{
		AcDbObjectIdArray fanObjids;
		for (int j = 0; j < fanNum; j++)
		{
			CString classsifyStr;
			DataHelper::GetPropertyData(objIds[j],_T("风机级别"),classsifyStr);
			if ( i == _ttoi(classsifyStr) )
			{
				fanObjids.append(objIds[j]);
			}
		}
		if(false == choseFansFromDB(fanObjids,flag))
		{
			comleted = false;
			return comleted;
		}
	}
	return comleted;
}


LibVentQHCacul::LibVentQHCacul( AcDbObjectIdArray fanObjIds,double reqQ,const chooseType& chFanType ):AxilQHCacul(fanObjIds,reqQ,chFanType),m_fanObjIds(fanObjIds),m_reqQ(reqQ),m_chFanType(chFanType)
{

}

LibVentQHCacul::~LibVentQHCacul()
{

}

void LibVentQHCacul::caculateQH(CString way)
{
	setFanNumsInLibs();
	AcDbObjectIdArray pushInIds,pullOutIds;
	FanWPCaculHelper::classifyFanByWay(m_fanObjIds,pushInIds,pullOutIds);

	if (m_fanObjIds.isEmpty())
	{
		AfxMessageBox(_T("没有安装局扇!"),MB_OK | MB_ICONSTOP);
		return;
	}

	if (pullOutIds.isEmpty() && pushInIds.isEmpty())
	{
		AfxMessageBox(_T("局扇没有设置工作方式!"),MB_OK | MB_ICONSTOP);
		return;
	}

	if (pullOutIds.isEmpty() &&  (_T("抽出式") == way) )
	{
		AfxMessageBox(_T("没有设置抽出式风机!"),MB_OK | MB_ICONSTOP);
		return;
	}

	if (pushInIds.isEmpty() &&  (_T("压入式") == way) )
	{
		AfxMessageBox(_T("没有设置压入式风机!"),MB_OK | MB_ICONSTOP);
		return;
	}

	int fanNum = m_fanObjIds.length();
	int classifyIntMax = 0;

	//acutPrintf(_T("\n抽出式:%d"),pullOutIds.length());
	//acutPrintf(_T("\n压入式:%d"),pushInIds.length());

	if ((pullOutIds.isEmpty() || pushInIds.isEmpty()) && (_T("压入式") == way || _T("抽出式") == way))
	{
		classifyIntMax = FanWPCaculHelper::getMaxClassify(m_fanObjIds,_T("风机级别"));
		if( false == choseFansByRand(m_fanObjIds,classifyIntMax,0)) return;
	}

	else if (_T("长压短抽") == way)
	{
		classifyIntMax = FanWPCaculHelper::getMaxClassify(pullOutIds,_T("风机级别"));
		if( false == choseFansByRand(pullOutIds,classifyIntMax,0)) return;
		classifyIntMax = FanWPCaculHelper::getMaxClassify(pushInIds,_T("风机级别"));
		if( false == choseFansByRand(pushInIds,classifyIntMax,1)) return;
	}

	else if (_T("长抽短压") == way)
	{
		classifyIntMax = FanWPCaculHelper::getMaxClassify(pushInIds,_T("风机级别"));
		if( false == choseFansByRand(pushInIds,classifyIntMax,0)) return;
		classifyIntMax = FanWPCaculHelper::getMaxClassify(pullOutIds,_T("风机级别"));
		if( false == choseFansByRand(pullOutIds,classifyIntMax,1)) return;
	}

	else
	{
		AfxMessageBox(_T("通风方式与通风方法不配套"),MB_OK | MB_ICONSTOP);
		return;
	}
}

bool LibVentQHCacul::choseFansFromDB( AcDbObjectIdArray fanObjtIds,int flag )
{
	AcDbObjectIdArray chimsObjIds = FanWPCaculHelper::getLinkedChim(fanObjtIds[0]);
	CString way;
	DataHelper::GetPropertyData(fanObjtIds[0],_T("工作方式"),way);

	int len = chimsObjIds.length();
	if(0 == len) return false;
	int fanNum = fanObjtIds.length();
	double p = 1,r = 0;
	for(int i = len-1; i >= 0; i--)
	{
		CString leakStr,windageStr;
		DataHelper::GetPropertyData(chimsObjIds[i],_T("漏风备用系数"),leakStr);
		DataHelper::GetPropertyData(chimsObjIds[i],_T("风筒风阻"),windageStr);

		double p1 = _tstof(leakStr);
		double r1 = _tstof(windageStr);

		p = p1 * p;
		r = r1 + r;

	}

	m_workQ = p * m_reqQ / fanNum;
	m_workH = p * r * m_reqQ * m_reqQ / (fanNum * fanNum);
	acutPrintf(_T("\n风量：%.2lfm^3/min\t风压：%.2lfPa"),m_workQ*60,m_workH);

	cstringArray names,minQs,maxQs,minHs,maxHs,ratedPowers;
	FanWPCaculHelper::ReadData(way,m_workQ*60,m_workH,names,minQs,maxQs,minHs,maxHs,ratedPowers);

	int num = names.length();
	if (0 == num)
	{
		AfxMessageBox(_T("对局扇性能参数要求高\n实际技术条件不能满足"),MB_OK | MB_ICONSTOP);
		return false;
	}
	CString name;
	double workQ = 0; //风机工作风量
	double workH = 0; //风机工作风压
	double spaceH = 0; //风压上限空间

	//CString minQStr,maxQStr,minHStr,maxHStr;
	//FanWPCaculHelper::RealParamCacul(name,names,minQs,maxQs,minHs,maxHs,m_workQ,r,p,workQ,workH,spaceH,minQStr,maxQStr,minHStr,maxHStr);

	//CString minQStr,maxQStr,minHStr,maxHStr,ratedPower;
	//FanWPCaculHelper::RealParamCaculQ(name,names,minQs,maxQs,minHs,maxHs,m_workQ,r,p,workQ,workH,spaceH,minQStr,maxQStr,minHStr,maxHStr,ratedPowers,ratedPower);
	CString minQStrP,maxQStrP,minHStrP,maxHStrP,ratedPowerP;
	switch(m_chFanType)
	{
	case POWER:
		FanWPCaculHelper::RealParamCaculP(name,names,minQs,maxQs,minHs,maxHs,m_workQ,r,p,workQ,workH,spaceH,minQStrP,maxQStrP,minHStrP,maxHStrP,ratedPowers,ratedPowerP);
		break;
	case QUANTITY:
		FanWPCaculHelper::RealParamCaculQ(name,names,minQs,maxQs,minHs,maxHs,m_workQ,r,p,workQ,workH,spaceH,minQStrP,maxQStrP,minHStrP,maxHStrP,ratedPowers,ratedPowerP);
		break;
	default:
		break;
	}

	for (int i = 0; i < fanNum; i++)
	{
		FanWPCaculHelper::fanWPWrite(fanObjtIds[i],name,workQ,workH,m_workQ,m_workH,spaceH,minQStrP,maxQStrP,minHStrP,maxHStrP );
		//acutPrintf(_T("\n%d->工作参数%f--%f"),i,workQ,workH);
	}

	AcDbObjectId tunelId;
	DrawHelper::GetHostGE(fanObjtIds[0],tunelId);

	CString areStr;
	DataHelper::GetPropertyData(tunelId,_T("断面面积"),areStr);

	if (areStr.IsEmpty())
	{
		AfxMessageBox(_T("巷道的断面面积没有给定!"),MB_OK | MB_ICONSTOP);
		return false;
	}
	AcDbObjectId lib = getrelatedLib(fanObjtIds[0]);
	if ( 0 == flag && lib.isNull() )
	{
		m_reqQ = fanNum * workQ * 1.5;	
	}

	else if( 1 ==flag && lib.isNull() )
	{
		m_reqQ = fanNum * workQ + 0.25 * _tstof(areStr);
	}

	else
	{
		CString libLeakage;
		DataHelper::GetPropertyData(lib,_T("漏风备用系数"),libLeakage);
		if (libLeakage.IsEmpty()) 
		{
			AfxMessageBox(_T("风库的参数不全"),MB_OK | MB_ICONSTOP);
			return false;
		}
		m_reqQ = fanNum * workQ * _tstof(libLeakage);	
	}

	return true;

}

bool LibVentQHCacul::choseFansByRand( AcDbObjectIdArray objIds, int classifyIntMax,int flag )
{
	bool comleted = true;
	if ( 0 == classifyIntMax )
	{
		AfxMessageBox( _T("风机级别设置有误!"),MB_OK | MB_ICONSTOP );
		comleted = false;
		return comleted;
	}
	int fanNum = objIds.length();

	//acutPrintf(_T("\n风机最大级别:%d"),classifyIntMax);

	for (int i = 1; i <= classifyIntMax; i++)
	{
		AcDbObjectIdArray fanObjids;
		for (int j = 0; j < fanNum; j++)
		{
			CString classsifyStr;
			DataHelper::GetPropertyData(objIds[j],_T("风机级别"),classsifyStr);
			if ( i == _ttoi(classsifyStr) )
			{
				fanObjids.append(objIds[j]);
			}
		}
		if(false == choseFansFromDB(fanObjids,flag))
		{
			comleted = false;
			return comleted;
		}
	}
	return comleted;

}

void LibVentQHCacul::setFanNumsInLibs()
{
	int fanNum = m_fanObjIds.length();
	AcDbObjectIdArray libs;
	DrawHelper::FindMineGEs(_T("WindLibrary"),libs);
	int libNum = libs.length();
	AcTransaction *pTrans = actrTransactionManager->startTransaction();

	for (int i = 0 ; i < libNum; i++)
	{
		int fanNumInLib = 0;
		if(pTrans == 0) return;
		AcDbObject *pObjt;
		if(Acad::eOk != pTrans->getObject(pObjt,libs[i],AcDb::kForRead)) return;
		WindLibrary *lib = WindLibrary::cast(pObjt);
		AcGePoint3d libInsertPt = lib->getInsertPt();
		//double libAngle = lib->getDirectionAngle();
		//actrTransactionManager->endTransaction();
		//AcDbObjectIdArray fans;
		for (int j = 0 ; j < fanNum; j++)
		{
			//pTrans = actrTransactionManager->startTransaction();
			if(pTrans == 0) return;
			if(Acad::eOk != pTrans->getObject(pObjt,m_fanObjIds[j],AcDb::kForRead)) return;
			LocalFan *fan = LocalFan::cast(pObjt);
			AcGePoint3d faninsertPt = fan->getInsertPt();
			//actrTransactionManager->endTransaction();

			AcGeVector3d v = faninsertPt - libInsertPt;
			double lenth = v.length();

			if (lenth <= 65)
			{
				fanNumInLib += 1;
			}
		}
		if ( 0 == fanNumInLib)
		{
			//AfxMessageBox(_T("风库中没有风机!"),MB_OK | MB_ICONSTOP);
			return;
		}

		CString fanNumInlibStr;
		fanNumInlibStr.Format(_T("%d"),fanNumInLib);
		DataHelper::SetPropertyData(libs[i],_T("风机总台数"),fanNumInlibStr);
	}
	actrTransactionManager->endTransaction();

}

AcDbObjectId LibVentQHCacul::getrelatedLib(AcDbObjectId fanId)
{
	AcDbObjectIdArray libs;
	DrawHelper::FindMineGEs(_T("WindLibrary"),libs);
	int libNum = libs.length();
	AcDbObjectId libRet;
	AcTransaction *pTrans = actrTransactionManager->startTransaction();

	for (int i = 0 ; i < libNum; i++)
	{
		int fanNumInLib = 0;
		//AcTransaction *pTrans = actrTransactionManager->startTransaction();
		if(pTrans == 0) return 0;
		AcDbObject *pObjt;
		if(Acad::eOk != pTrans->getObject(pObjt,libs[i],AcDb::kForRead)) return 0;
		WindLibrary *lib = WindLibrary::cast(pObjt);
		AcGePoint3d libInsertPt = lib->getInsertPt();
		//actrTransactionManager->endTransaction();

		//pTrans = actrTransactionManager->startTransaction();
		if(pTrans == 0) return 0;
		if(Acad::eOk != pTrans->getObject(pObjt,fanId,AcDb::kForRead)) return 0;
		LocalFan *fan = LocalFan::cast(pObjt);
		AcGePoint3d faninsertPt = fan->getInsertPt();
		//actrTransactionManager->endTransaction();

		AcGeVector3d v = faninsertPt - libInsertPt;
		double lenth = v.length();
		if (lenth <= 65)
		{
			libRet = libs[i];
		}
	}

	actrTransactionManager->endTransaction();
	return libRet;
}