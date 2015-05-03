#pragma once

/* 使用lemon库提供的数据结构定义通用的图(或网络) */

/* ListDigraph、Arc、Node等表示通风网络 */
#include <lemon/list_graph.h>

using namespace lemon;

/* 有向图 */
typedef ListDigraph Digraph;

// 链接lib
#ifdef _DEBUG
	#pragma comment(lib, "lemond.lib")
#else
	#pragma comment(lib, "lemon.lib")
#endif