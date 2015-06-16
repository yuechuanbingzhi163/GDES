#include "StdAfx.h"
#include "ReactorHelper.h"
#include "DataObject_DbReactor.h"

/*
 * 用于记录与Document相关的reactor
 * 例如AcDbDataBaseReactor、AcEdInputPointMonitor等都是与文档相关的
 * 一个文档对象对应一个reactor，可以看做是"局部的"
 * 而AcEditorReactor属于AcEditor，可以看做是"全局的"
 * 从arx的消息流程可以看出，在kLoadAppMsg()中AcEditor有效，
 * 而其他的例如文档对象(AcApDocument)、数据库对象(AcDbDatabase)还是无效的
 * 只有在kLoadDwgMsg()中的文档和数据库对象才开始有效
 */
#include <map>
struct DocumentReactor
{
    DocumentReactor() : pDOReactor( 0 ) {}
    DataObject_DbReactor* pDOReactor;

    // ...
    // 集中在一起便于管理，也解决了多文档的问题
    // 1) 新建文档，发送kLoadDwgMsg消息
    // 2) 已有文档，加载arx程序，也会向所有的文档发送kLoadDwgMsg消息
    // 3) 关闭文档， 发送kUnloadDwgMsg消息
    // 4) 卸载arx程序，也会向所有的文档发送kUnloadDwgMsg消息
};

/*
 * long类型表示AcApDocument*的指针地址
 */
typedef std::map<long, DocumentReactor> DocumentReactorMap;

DocumentReactorMap* pDocumentReactorMap_RCU = 0;

void ReactorHelper::CreateDocumentReactorMap()
{
    if( pDocumentReactorMap_RCU == 0 )
    {
        pDocumentReactorMap_RCU = new DocumentReactorMap();
    }
}

void ReactorHelper::RemoveDocumentReactorMap()
{
    delete pDocumentReactorMap_RCU;
    pDocumentReactorMap_RCU = 0;
}

void ReactorHelper::AddDocumentReactor( AcApDocument* pDoc )
{
    if( pDoc == 0 ) return;
    if( pDocumentReactorMap_RCU == 0 ) return;
    long id = ( long )pDoc;
    DocumentReactorMap::iterator itr = pDocumentReactorMap_RCU->find( id );
    if( itr != pDocumentReactorMap_RCU->end() ) return;

    DocumentReactor dr;
    //dr.pTooltipMonitor = new MineGETooltipMonitor(pDoc);
    dr.pDOReactor = new DataObject_DbReactor( pDoc->database() );

    pDocumentReactorMap_RCU->insert( DocumentReactorMap::value_type( id, dr ) );
}

void ReactorHelper::RemoveDocumentReactor( AcApDocument* pDoc )
{
    if( pDoc == 0 ) return;
    if( pDocumentReactorMap_RCU == 0 ) return;
    long id = ( long )pDoc;
    DocumentReactorMap::iterator itr = pDocumentReactorMap_RCU->find( id );
    if( itr == pDocumentReactorMap_RCU->end() ) return;

    // 解除绑定
    DocumentReactor dr = itr->second;
    delete dr.pDOReactor;
    //delete dr.pTooltipMonitor;

    pDocumentReactorMap_RCU->erase( id );
}