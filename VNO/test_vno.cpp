#include <iostream>
using namespace std;

#include <vno/vno.h>

typedef Digraph::NodeMap<int> NodeIdMap;
typedef Digraph::ArcMap<int> EdgeIdMap;

static void PrintPath(EdgeArray& p, NodeIdMap& nid, EdgeIdMap& eid)
{
	cout<<"p={";
	for(int i=0;i<p.size();i++)
	{
		Digraph::Arc e = p[i];
		cout<<" "<<eid[e]<<" ";
	}
	cout<<"}"<<endl;
}

static void PrintPaths(	IntArray& pathNums, EdgeArray& allPath, NodeIdMap& nid, EdgeIdMap& eid)
{
	int s = 0;
	for(int i=0;i<pathNums.size();i++)
	{
		int n = pathNums[i];
		EdgeArray p;
		std::copy(allPath.begin()+s, allPath.begin()+s+n, std::back_inserter(p));
		s += n;
		cout<<"第"<<i+1<<"条路径:"<<endl;
		PrintPath(p, nid, eid);
	}
}

static string GTT_String(GasTubeType type)
{
	string ret = "未知";
	switch(type)
	{
	case GTT_NEGATIVE:
		ret = "负压管";
		break;
	case GTT_POSITIVE:
		ret = "正压管";
		break;
	}
	return ret;
}

static void PrintGasTubeMaps(Digraph& dg, GasTubeTypeMap& gtt, EdgeIdMap& eid)
{
	for(Digraph::ArcIt e(dg); e!=INVALID; ++e)
	{
		cout<<"e"<<eid[e]<<"  "<<GTT_String(gtt[e])<<endl;
	}
}
void vno_test()
{
    // 1) 通风网络
    Digraph dg;
    // 记录节点编号
    NodeIdMap nid(dg);
    //记录分支编号
    EdgeIdMap eid(dg);

    // 构建通风网络
    Digraph::Node v1 = dg.addNode();
    Digraph::Node v2 = dg.addNode();
    Digraph::Node v3 = dg.addNode();
    Digraph::Node v4 = dg.addNode();
    Digraph::Node v5 = dg.addNode();
    Digraph::Node v6 = dg.addNode();
    Digraph::Node v7 = dg.addNode();
    Digraph::Node v8 = dg.addNode();
    Digraph::Node v9 = dg.addNode();
    Digraph::Node v10 = dg.addNode();
    Digraph::Arc e1 = dg.addArc(v1, v2); // v1 -> v2
    Digraph::Arc e2 = dg.addArc(v2, v3); // v2 -> v3
    Digraph::Arc e3 = dg.addArc(v2, v4); // v2 -> v4
    Digraph::Arc e4 = dg.addArc(v3, v5); // v3 -> v5
    Digraph::Arc e5 = dg.addArc(v4, v5); // v3 -> v4
    Digraph::Arc e6 = dg.addArc(v3, v6); // v4 -> v5
    Digraph::Arc e7 = dg.addArc(v5, v7); // v5 -> v6
    Digraph::Arc e8 = dg.addArc(v4, v8); // v5 -> v6
    Digraph::Arc e9 = dg.addArc(v6, v7); // v5 -> v6
    Digraph::Arc e10 = dg.addArc(v7, v8); // v5 -> v6
    Digraph::Arc e11 = dg.addArc(v6, v9); // v5 -> v6
    Digraph::Arc e12 = dg.addArc(v8, v9); // v5 -> v6
    Digraph::Arc e13 = dg.addArc(v9, v10); // v5 -> v6

    // 记录节点和分支编号(方便打印输出)
    nid[v1]=1; nid[v2]=2; nid[v3]=3; nid[v4]=4; nid[v5]=5; nid[v6]=6;
    nid[v7]=7; nid[v8]=8; nid[v9]=9; nid[v10]=10;
    eid[e1]=1; eid[e2]=2; eid[e3]=3; eid[e4]=4; eid[e5]=5; eid[e6]=6; eid[e7]=7;
    eid[e8]=8; eid[e9]=9; eid[e10]=10; eid[e11]=11; eid[e12]=12; eid[e13]=13;

	ArcFilter arc_filter(dg, true);
	IntArray pathNums;
	EdgeArray allPath;
	//遍历两点之间的所有路径
	dfs_all_path(dg, arc_filter, v1, v10, pathNums, allPath);
	std::cout<<"搜索v"<<nid[v1]<<"-->v"<<nid[v10]<<"之间的所有路径:\n";
	//打印所有路径
	PrintPaths(pathNums, allPath, nid, eid);

	cout<<"\n-----------------------------\n";
	//指定哪些点是泵(可以不只一个)
	NodeArray pumps;
	pumps.push_back(v3);
	pumps.push_back(v5);
	GasTubeTypeMap gtt(dg);
	//正负压管划分
	if(!partition_gas_tubes(dg, arc_filter, pumps, gtt))
	{
		std::cout<<"出现泵串联的情况"<<std::endl;
	}
	//打印正负压管划分情况
	PrintGasTubeMaps(dg, gtt, eid);
}

void vno_test2()
{
    // 1) 通风网络
    Digraph dg;
    // 记录节点编号
    NodeIdMap nid(dg);
    //记录分支编号
    EdgeIdMap eid(dg);

    // 构建通风网络
    Digraph::Node v1 = dg.addNode();
    Digraph::Node v2 = dg.addNode();
    Digraph::Node v3 = dg.addNode();
    Digraph::Node v4 = dg.addNode();
    Digraph::Node v5 = dg.addNode();
    Digraph::Node v6 = dg.addNode();
    Digraph::Node v7 = dg.addNode();
    Digraph::Node v8 = dg.addNode();
    Digraph::Node v9 = dg.addNode();
    Digraph::Node v10 = dg.addNode();
    Digraph::Node v11 = dg.addNode();
    Digraph::Node v12 = dg.addNode();
    Digraph::Arc e1 = dg.addArc(v1, v2); // v1 -> v2
    Digraph::Arc e2 = dg.addArc(v1, v5); // v2 -> v3
    Digraph::Arc e3 = dg.addArc(v2, v3); // v2 -> v4
    Digraph::Arc e4 = dg.addArc(v3, v4); // v3 -> v5
    Digraph::Arc e5 = dg.addArc(v5, v4); // v4 -> v5
    Digraph::Arc e6 = dg.addArc(v2, v6); // v3 -> v4
    Digraph::Arc e7 = dg.addArc(v3, v7); // v5 -> v6
    Digraph::Arc e8 = dg.addArc(v4, v9); // v1 -> v2
    Digraph::Arc e9 = dg.addArc(v5, v10); // v2 -> v3
    Digraph::Arc e10 = dg.addArc(v6, v7); // v2 -> v4
    Digraph::Arc e11 = dg.addArc(v7, v8); // v3 -> v5
    Digraph::Arc e12 = dg.addArc(v9, v8); // v4 -> v5
    Digraph::Arc e13 = dg.addArc(v9, v10); // v3 -> v4
    Digraph::Arc e14 = dg.addArc(v6, v11); // v5 -> v6
    Digraph::Arc e15 = dg.addArc(v8, v11); // v1 -> v2
    Digraph::Arc e16 = dg.addArc(v10, v11); // v2 -> v3
    Digraph::Arc e17 = dg.addArc(v11, v12); // v2 -> v3

    nid[v1]=1; nid[v2]=2; nid[v3]=3; nid[v4]=4; nid[v5]=5; nid[v6]=6;
    nid[v7]=7; nid[v8]=8; nid[v9]=9; nid[v10]=10; nid[v11]=11; nid[v12]=12;
    eid[e1]=1; eid[e2]=2; eid[e3]=3; eid[e4]=4; eid[e5]=5; eid[e6]=6; eid[e7]=7;
    eid[e8]=8; eid[e9]=9; eid[e10]=10; eid[e11]=11; eid[e12]=12; eid[e13]=13; eid[e14]=14;
    eid[e15]=15; eid[e16]=16; eid[e17]=17;

}

void vno_test3()
{
    // 1) 通风网络
    Digraph dg;
    // 记录节点编号
    NodeIdMap nid(dg);
    //记录分支编号
    EdgeIdMap eid(dg);

    // 构建通风网络
    Digraph::Node v1 = dg.addNode();
    Digraph::Node v2 = dg.addNode();
    Digraph::Node v3 = dg.addNode();
    Digraph::Node v4 = dg.addNode();
    Digraph::Node v5 = dg.addNode();
    Digraph::Node v6 = dg.addNode();
    Digraph::Arc e1 = dg.addArc(v1, v2); // v1 -> v2
    Digraph::Arc e2 = dg.addArc(v2, v3); // v2 -> v3
    Digraph::Arc e3 = dg.addArc(v2, v4); // v2 -> v4
    Digraph::Arc e4 = dg.addArc(v3, v5); // v3 -> v5
    Digraph::Arc e6 = dg.addArc(v4, v5); // v4 -> v5
    Digraph::Arc e5 = dg.addArc(v3, v4); // v3 -> v4
    Digraph::Arc e7 = dg.addArc(v5, v6); // v5 -> v6

    // 记录节点和分支编号(方便打印输出)
    nid[v1]=1; nid[v2]=2; nid[v3]=3; nid[v4]=4; nid[v5]=5; nid[v6]=6;
    eid[e1]=1; eid[e2]=2; eid[e3]=3; eid[e4]=4; eid[e5]=5; eid[e6]=6; eid[e7]=7;

}