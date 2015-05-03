#pragma once

enum GASTUBETYPE
{
	PD_GAS_TUBE	 = 0,	//永久抽放瓦斯管路
	MPE_GAS_TUBE = 1,	//移动泵排瓦斯管路
	MPD_GAS_TUBE = 2	//移动泵抽瓦斯管路
};
class DrawCmd
{
public:
	/* 实现在DrawVent.cpp中 */
	//static void DrawJoint(void);
	static void DrawTunnel(void);
	static void DrawArcTunnel(void);
	static void DrawWS(void);

	/* 实现在DrawTTunnel.cpp中 */
	static void DrawTTunnel();
	static void ResetInTunnel();
	static void ShowInTunnel();

	static void DrawMainFan();
	static void DrawLocalFan();
	static void DrawFanTag();
	static void DrawGate(int flag);
	static void DrawCasement(int flag);
	static void DrawWindBridge();
	static void DrawJoint();
	static void DrawWall(int flag);
	static void DrawStorage(int flag);
	static void DrawDirection(void);
	static void DrawWindLibrary(void);

	//瓦斯抽采管路绘制
	static void DrawGasTube(GASTUBETYPE GEName);

	//瓦斯泵绘制
	static void DrawGasPump( void );

	//阀
	static void DrawValve( void );

	//放水器
	static void DrawTailrace( void );

	//流量计
	static void DrawFlowmeter( void );

	//阻火器
	static void DrawBackFire( void );

	//测定孔
	static void DrawDetermineHole( void );

	//钻孔
	static void DrawDrill( void );

	//温度传感器
	static void DrawTempeSensor( void );

	//流量传感器
	static void DrawFlowSensor( void );

	//压差传感器
	static void DrawDifferPressSensor( void );

	//瓦斯传感器
	static void DrawGasSensor( void );

	static void DrawChimney(void);
	//static void DrawCompass();

	/* 实现在AutoDraw.cpp中 */
	static void AutoDraw();
	static void AutoDraw2();

	/* 自动标注风流方向 */
	static void AutoDirection();

	static double ControlDirByMethods(AcDbObjectId objId,double angle);

	/* 自动更新瓦斯流量数据 */
	//static void UpdateAllGasFlow();
	// 通用编辑命令
	static void JunctionEnclose(void);
	static void ReverseDirection();
	static void ReBindEdge();
	static void ZoomMineGE();
	static void SplitByPoint(AcDbObjectId objId,AcGePoint3d spt,AcGePoint3d ept,AcGePoint3d pt);
	static void SplitDeal();
	static void MergeChimney();
	static void Merging(AcDbObjectId sourceId,AcDbObjectId mergeId);
	static void DrawQTagGE();
	static void DrawEffectRanGE();

	static void DrawGasFlow();
	static void AutoGasFlow();

	static void DeleteDirection();
	static void DeleteAllGasFlow();
	static void DeleteFanTag();
	static void DeleteQTag();
	static void DeleteEDTag();

	//static AcDbObjectId GetRelatedTW(AcDbObjectId objId);

	static void testHD();
};