#ifndef CACUL_FUNC_H
#define CACUL_FUNC_H

#define PI 3.1415926535897932384626433832795
//计算类，包含抽采所需计算的所有公式和变量，CaculFunc是计算类名
class CaculFunc
{
public:
	bool coalResrevesCacul();		//参与评价的煤量计算
	bool remnantGasByQCacul();		//残余瓦斯含量计算(通过瓦斯抽采量)
	bool remnantGasByPCacul();		//残余瓦斯含量计算(通过瓦斯压力计算)
	bool remnantPressCacul();		//残余瓦斯压力计算
	bool remnantGasStandCacul();	//残存瓦斯含量计算 
	bool analyGasCacul();			//可解吸瓦斯含量计算
	bool faceDrainGasRateCacul();	//工作面瓦斯抽采率计算
	bool mineDrainGasRateCacul();	//矿井瓦斯抽采率计算
	bool miningFaceGasCacul();		//本煤层瓦斯涌出量计算
	bool nearbyFaceGasCacul();		//邻近煤层瓦斯涌出量计算
	bool gasTubeDiameterCacul();	//瓦斯管内径计算
	bool tubeFrictionResisCacul();	//管路摩擦阻力计算
	//void tubePartResisCacul();		//管路局部阻力计算
	bool pumbFlowCacul();			//瓦斯泵流量计算
	//void groundZyAllLossCacul();	//正压段管路全部阻力损失计算
	//void underFyAllLossCacul();		//负压管路的全部阻力损失计算
	bool pumbPressCacul();			//瓦斯泵压力计算
	bool drainDrillFyCacul();		//孔口负压计算
	bool zkDbHeightCacul();			//终孔位置距煤层底板的垂直距离
	bool drillDeflectionCacul();	//钻孔偏角
	bool drillElevationCacul();		//钻孔仰角计算
	bool drillLengthCacul();		//钻孔长度计算
	bool drillEffectiveLenthCacul();//钻孔有效长度计算 
	bool faceEffectiveLenthCacul();//工作面推进有效长度计算
	bool highDrillDisCacul();		//高位钻孔钻场间距计算

public:
	//计算参数定义
	//参与评价的煤量计算所需参数 
	double unitCoalResreves;		//评价单元参与计算煤炭储量
	double unitTrendlenth;			//评价单元走向长度
	double unitTendencyLenth;		//评价单元倾向长度
	double unitTrendWidthz1;		//走向方向巷道瓦斯预排等值宽度1
	double unitTrendWidthz2;		//走向方向巷道瓦斯预排等值宽度2
	double unitTendcyWidthz1;		//倾向方向巷道瓦斯预排等值宽度1
	double unitTendcyWidthz2;		//倾向方向巷道瓦斯预排等值宽度2
	double drainGasRadius;			//抽采钻孔有效影响半径
	double averMinePly;				//平均煤层厚度
	double mineDencity;				//煤的密度

	//残余瓦斯含量
	double remnantGas;				//残余瓦斯含量
	//通过瓦斯抽采计算 
	double oldgasContent;			//原始瓦斯含量
	double drainGasAmo;				//评价单元钻孔抽放瓦斯总量
	//通过残余瓦斯含量计算 
	double adsorptionA;				//吸附常数a
	double adsorptionB;				//吸附常数b
	double absoGasPress;			//残余相对瓦斯压力 
	double qualityWater;			//水分
	double qualityAsh;				//灰分
	double poriness;				//孔隙率 
	
	//残存瓦斯含量
	double remnantGasStand;			//标准大气压下的残存瓦斯含量

	//可解吸瓦斯含量
	double analysisGas;				//可解吸瓦斯含量

	//瓦斯抽采率
	//工作面瓦斯抽采率
	double faceDrainGasRate;		//工作面瓦斯抽采率
	double averFaceDrainGas;		//当月工作面月平均瓦斯抽采量
	double faceWindGas;				//当月工作面风排瓦斯量
	//矿井瓦斯抽采率
	double mineDrainGasRate;		//矿井瓦斯抽采率
	double averMineDrainGas;		//当月矿井平均瓦斯抽采
	double mineWindGas;				//当月矿井排风瓦斯含量

	//工作面瓦斯涌出量
	//本煤层瓦斯涌出量
	double miningFaceGas;			//本开采层涌入工作面的绝对瓦斯量
	double outFaceRemnantGas;		//运出采区采落煤炭的残余瓦斯含量
	double protoDayYield;			//拟定日产量
	double faceTendencyLenth;		//工作面倾向长度
	double faceTendencyWidthz1;		//倾向方向巷道瓦斯预排等值宽度1
	double faceTendencyWidthz2;		//倾向方向巷道瓦斯预排等值宽度2

	//邻近层瓦斯涌出量
	double nearbyFaceGas;		//上下邻近层涌入开采层的瓦斯涌出量
	//double nearbyMinePlyi;		//第i临近层的煤厚
	//邻近层瓦斯涌出量的计算需要考虑

	//泵站管网能力核算
	//瓦斯直径
	double gasTubeDiameter;		//瓦斯管内径
	double gasTubeTraffic;		//瓦斯管中的瓦斯流量
	double averGasTubeSpeed;	//瓦斯管中瓦斯平均流速

	//管路阻力
	double tubeFrictionResistance;		//管路中的摩擦阻力
	double tubeLenth;					//管路长度
	double gasAirDencity;				//混合瓦斯对空气的相对密度
	double tubeFactor;					//系数
	double tubeGasPress;				//管道内气体的绝对压力
	double tubeTemperature;				//管路温度
	double tubePartResistance;			//管路中的局部阻力

	double rangeFactor;			//局部阻力计算时的范围
	//瓦斯泵的流量计算
	double pumbFlow;			//瓦斯泵的额定流量
	double pureMineDrainGas;	//矿井抽放瓦斯纯总量
	double mineDrainGasDencity;	//矿井瓦斯抽放浓度
	double llReserveFactor;		//瓦斯泵流量备用系数
	double pumbEfficiency;		//瓦斯泵的机械效率

	//正压段管路全部阻力损失
	double groundZyAllLoss;		//正压管路的全部阻力损失
	double groundZyMcLoss;		//正压管路的摩擦阻力损失
	double groundZyJbLoss;	    //正压管路的局部阻力损失
	double zyNeed;      	    //用户所需正压

	//负压段管路全部阻力损失
	double underFyAllLoss;	     //负压管路的全部阻力损失
	double drainDrillFy;          //抽放钻孔所需负压
	double underFyMcLoss;	     //负压管路的摩擦阻力损失
	double underFyJbLoss;	     //负压的局部阻力损失

	//瓦斯泵的压力
	double pumbPress;               //瓦斯泵的压力
	double ylReserveFactor;	     //瓦斯泵压力备用系数


	//孔口负压计算
	double reserveFactor;	     //备用系数

	//终孔设计高度参考值
	double zkDbHeight;             //终孔位置距煤层底板的垂直距离
	double maoluoHeight;            //冒落带的高度
	double liexiHeight;             //裂隙带的高度

	//偏角
	double drillDeflection;         //钻孔偏角
	double zkTrendKkDis;         //终孔点至开孔位置水平距离
	double zkTrendcyHfDis;        //终孔点至回风巷垂距

	//仰角
	double drillElevation;          //钻孔仰角
	double kkHeight;                //开孔高度


	//钻孔长度
	double drillLength;             //钻孔长度

	//钻孔有效长度
	double drillEffectiveLenth;   //钻孔有效长度

	//工作面推进有效长度
	double faceEffectiveLenth;    //工作面推进有效长度

	//高位钻孔钻场间距
	double highDrillDis;           //高位钻孔钻场间距

	AcDbObjectId m_objId;		//字段类型ID
};

#endif //CACUL_FUNC_H