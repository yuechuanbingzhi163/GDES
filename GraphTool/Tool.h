#pragma once

#include "dlimexp.h"

//extern "C" GRAPH_EXPORT_API bool IsNeedAir(const AcDbObjectId& objId);
//extern "C" GRAPH_EXPORT_API bool IsPowerSubstation(const AcDbObjectId& objId);
extern "C" GRAPH_EXPORT_API void GetHosts(const AcDbObjectIdArray& objIds, AcDbObjectIdArray& hosts);

extern "C" GRAPH_EXPORT_API void FindMainFans(AcDbObjectIdArray& objIds);
extern "C" GRAPH_EXPORT_API void FindGates(AcDbObjectIdArray& objIds);

extern "C" GRAPH_EXPORT_API void FindTTunnels(AcDbObjectIdArray& objIds);
extern "C" GRAPH_EXPORT_API void FindLocalFans(const AcDbObjectIdArray& tObjIds, AcDbObjectIdArray& objIds);
extern "C" GRAPH_EXPORT_API void FindChimneys(const AcDbObjectIdArray& tObjIds, AcDbObjectIdArray& objIds);

extern "C" GRAPH_EXPORT_API void GetInsertPts(const AcDbObjectIdArray& objIds, AcGePoint3dArray& pts);

extern "C" GRAPH_EXPORT_API bool IsMainPowerSubtation(const AcDbObjectId& objId);
