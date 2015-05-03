#include "VentNetwork.h"

#include <cmath>

VentNetwork::VentNetwork() 
:ed(dg, 0), fd(dg, 0), nd(dg, 0), gd(dg, 0),
 sn(INVALID), tn(INVALID), airEdge(INVALID)
{
}

VentNetwork::~VentNetwork()
{
	for(Digraph::ArcIt e(dg); e!=INVALID; ++e)
	{
		delete ed[e]; ed[e] = 0;
		delete fd[e]; fd[e] = 0;
		delete gd[e]; gd[e] = 0;
	}
	for(Digraph::NodeIt n(dg); n!=INVALID; ++n)
	{
		delete nd[n]; nd[n] = 0;
	}
}

Digraph& VentNetwork::graph()
{
	return dg;
}

Digraph::Node VentNetwork::vSource() const
{
	return sn;
}

Digraph::Node VentNetwork::vTarget() const
{
	return tn;
}

Digraph::Arc VentNetwork::vAir() const
{
	return airEdge;
}

EdgeData*& VentNetwork::edge(Digraph::Arc e)
{
	return ed[e];
}

FanData*& VentNetwork::fan(Digraph::Arc e)
{
	return fd[e];
}

NodeData*& VentNetwork::node(Digraph::Node u)
{
	return nd[u];
}

GateData*& VentNetwork::gate(Digraph::Arc e)
{
	return gd[e];
}

void VentNetwork::addVirtualST()
{
	if(sn != INVALID || tn != INVALID) return;

	GraphUtils::AddVirtualSTNode(dg, sn, tn);
	for(Digraph::OutArcIt e(dg, sn); e!=INVALID; ++e)
	{
		ed[e] = new EdgeData();
		fd[e] = 0;
	}
	for(Digraph::InArcIt e(dg, tn); e!=INVALID; ++e)
	{
		ed[e] = new EdgeData();
		fd[e] = 0;
	}
}

void VentNetwork::delVirtualST()
{
	if(sn == INVALID && tn == INVALID) return;

	for(Digraph::OutArcIt e(dg, sn); e!=INVALID; ++e)
	{
		delete ed[e];
	}
	for(Digraph::InArcIt e(dg, tn); e!=INVALID; ++e)
	{
		delete ed[e];
	}
	GraphUtils::DeleteOutArcs(dg, sn);   // 删除虚拟源点的所有出边分支(虚拟分支)
	GraphUtils::DeleteInArcs(dg, tn);    // 删除虚拟源点的所有入边分支(虚拟分支)
	dg.erase(sn); dg.erase(tn);          // 删除虚拟源汇节点
	sn = tn = INVALID;
}

void VentNetwork::addAirEdge()
{
	if(sn == INVALID || tn == INVALID || airEdge != INVALID) return;
	// 增加虚拟大气分支
	airEdge = dg.addArc(tn, sn);
	// 初始化虚拟大气分支数据
	ed[airEdge] = new EdgeData();
	fd[airEdge] = 0;
}

void VentNetwork::delAirEdge()
{
	if(airEdge == INVALID) return;
	// 删除虚拟大气分支数据
	delete ed[airEdge];
	// 删除虚拟大气分支
	dg.erase(airEdge);
	airEdge = INVALID;
}

double VentNetwork::outArcFlow(Digraph::Node u) const
{
	double s = 0;
	for(Digraph::OutArcIt e(dg, u); e!=INVALID; ++e)
	{
		s += ed[e]->q;
	}
	return s;
}

double VentNetwork::inArcFlow(Digraph::Node u) const
{
	double s = 0;
	for(Digraph::InArcIt e(dg, u); e!=INVALID; ++e)
	{
		s += ed[e]->q;
	}
	return s;
}

int VentNetwork::countFixQ() const
{
	int fixq_count=0;
	for(Digraph::ArcIt e(dg);e!=INVALID;++e)
	{
		if(ed[e]->fq > 0)
		{
			fixq_count++;
		}
	}
	return fixq_count;
}

void VentNetwork::fixQEdges(EdgeArray& edges) const
{
	for(Digraph::ArcIt e(dg);e!=INVALID;++e)
	{
		if(ed[e]->fq > 0)
		{
			edges.push_back(e);
		}
	}
}

void VentNetwork::fanEdges(EdgeArray& edges) const
{
	for(Digraph::ArcIt e(dg); e!=INVALID; ++e)
	{
		if(fd[e] != 0)
		{
			edges.push_back(e);
		}
	}
}

void VentNetwork::gateEdges(EdgeArray& edges) const
{
	for(Digraph::ArcIt e(dg); e!=INVALID; ++e)
	{
		if(gd[e] != 0)
		{
			edges.push_back(e);
		}
	}
}

double VentNetwork::hPath(EdgeArray& p) const
{
	double S = 0;
	for(size_t i=0;i<p.size();i++)
	{
		Digraph::Arc e = p[i];
		S += ed[e]->f0();
	}
	return S;
}

void VentNetwork::adjustNegativeEdge()
{
	//查找负风量分支
	EdgeArray ne;
	for(Digraph::ArcIt e(dg);e!=INVALID;++e)
	{
		if(ed[e]->q < 0)
		{
			ne.push_back(e);
			//风量变正
			ed[e]->q = fabs(ed[e]->q);
		}
	}
	//反向负风量分支
	for(EdgeArray::iterator itr=ne.begin();itr!=ne.end();++itr)
	{
		GraphUtils::ReverseArc(dg,*itr);
	}
}

void VentNetwork::caclNodePressures()
{
	//标记节点压力是否已被计算过了
	Digraph::NodeMap<bool> hasPressCacl(dg);
	for(Digraph::NodeIt u(dg); u!=INVALID; ++u)
	{
		hasPressCacl[u] = false;
	}
	hasPressCacl[sn] = true;

	//标记虚拟源点的出边分支的末节点都已经计算过了
	//并将找到的节点依次添加到队列中,按出队的顺序计算节点压力
	std::queue<Digraph::Node> cNodes;
	for(Digraph::OutArcIt e(dg, sn); e!=INVALID; ++e)
	{
		Digraph::Node u = dg.target(e);
		hasPressCacl[u] = true;
		cNodes.push(u);
	}

	//顺序出队,依次计算节点压力
	while(!cNodes.empty())
	{
		Digraph::Node u = cNodes.front();
		for(Digraph::OutArcIt e(dg, u); e!=INVALID; ++e)
		{
			Digraph::Node v = dg.target(e);
			if(v == sn || v == tn || hasPressCacl[v]) continue;

			nd[v]->p = (nd[u]->p)-(ed[e]==0?0:ed[e]->f0());
			nd[v]->p +=((fd[e]==0)?0:fd[e]->h0(ed[e]->q));
			cNodes.push(v);
			hasPressCacl[v] = true;
		}
		cNodes.pop();
	}
}

void VentNetwork::adjustableEdges(const EdgeArray& p, EdgeArray& adjustEdges)
{
	for(EdgeArray::const_iterator itr=p.begin();itr!=p.end();++itr)
	{
		Digraph::Arc e = *itr;
		if(ed[e]->alevel != AL_4 )
		{
			adjustEdges.push_back(e);
		}
	}
}

void VentNetwork::filterVirutalEdges(const EdgeArray& pp, EdgeArray& p)
{
	for(size_t i=0;i<pp.size();i++)
	{
		Digraph::Arc e = pp[i];
		if(!ed[e]->isNull())
		{
			p.push_back(e);
		}
	}
}
