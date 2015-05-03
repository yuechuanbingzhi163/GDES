#pragma once

enum EdgeType
{
	ET_COMMON   = 0,  // 普通巷道
	ET_USEAIR   = 1,  // 独立用风地点(回采工作面、变电所、掘进工作面)
	ET_VIRTUAL  = 2   // 虚拟分支
};

struct EdgeData
{
	EdgeData() : et(ET_COMMON), vt(0) {}

	AcDbObjectId objId;
	EdgeType et;   // 分支类型(默认为普通巷道-ET_COMMON)
	int vt;           // 通风类型(默认为进风-0)
};
