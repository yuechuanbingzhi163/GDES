#include "StdAfx.h"

#include "config.h"
#include "HelperClass.h"
#include "ReactorHelper.h"
#include "CurDrawTool.h"

#include "../ArxHelper/HelperClass.h"

// 定义注册服务名称
#ifndef MINEGE_SERVICE_NAME
#define MINEGE_SERVICE_NAME _T("MINEGE_SERVICE_NAME")
#endif

class CMineGEApp : public AcRxArxApp
{

public:
    CMineGEApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

        // 注册服务
        acrxRegisterService( MINEGE_SERVICE_NAME );

        ReactorHelper::CreateLinkedGE_EditorReactor();
        ReactorHelper::CreateDocumentReactorMap();

        acutPrintf( _T( "\nMineGE::On_kInitAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        // 删除AcEditorReactor
        ReactorHelper::RemoveLinkedGE_EditorReactor();
        ReactorHelper::RemoveDocumentReactorMap();

        // 删除服务
        delete acrxServiceDictionary->remove( MINEGE_SERVICE_NAME );

        acutPrintf( _T( "\nMineGE::On_kUnloadAppMsg\n" ) );

        return ( retCode ) ;
    }

    static void PrintAddress()
    {
        AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();
        AcEditor* pEditor = acedEditor;
        AcApDocument* pDoc = curDoc();
        AcDbDatabase* pDocDB = pDoc->database();
        AcEdInputPointManager* pIPM = pDoc->inputPointManager();

        acutPrintf( _T( "\nworking database: %ld" ), ( long )pDB );
        acutPrintf( _T( "\neditor: %ld" ), ( long )pEditor );
        acutPrintf( _T( "\ncurrent doc: %ld" ), ( long )pDoc );
        acutPrintf( _T( "\ncurrent doc database: %ld" ), ( long )pDocDB );
        acutPrintf( _T( "\ncurrent doc InputPointManager: %ld" ), ( long )pIPM );
    }

    // 可以用来处理与文档对象AcDocument相关联的reactor
    //  例如AcEdInputPointMonitor等
    // 在On_kLoadDwgMsg和On_kUnloadDwgMsg消息中文档对象还是有效的；
    // 而在On_kInitAppMsg和On_kUnloadAppMsg
    // 文档对象已经失效; 数据库对象同样也是如此
    virtual AcRx::AppRetCode On_kLoadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kLoadDwgMsg ( pkt ) ;

        // 注册dictionary和regAppName
        ArxDataTool::RegDict( PROPERTY_DATA_FIELD_DICT );
        ArxDataTool::RegDict( PROPERTY_DATA_FIELD_INFO_DICT );
        ArxDataTool::RegDict( TYPE_NAME_MAP_DICT );
        ArxDataTool::RegDict( STRING_LIST_DICT );
        ArxDataTool::RegDict( INT_LIST_DICT );
        ArxDataTool::RegDict( OBJECT_LIST_DICT );
        ArxDataTool::RegDict( OBJECT_LIST_DATA_DICT );
        ArxDataTool::RegDict( GLOBAL_SINGLE_INFO_DICT );
        ArxDataTool::RegDict( CUR_DRAW_DICT );
		ArxDataTool::RegDict(FUNC_FIELD_DICT);

        ArxDataTool::RegAppName( acdbHostApplicationServices()->workingDatabase(), DRAW_PARAMS_XDATA_GROUP );

        //PrintAddress();

        ReactorHelper::AddDocumentReactor( curDoc() );

        // 初始化所有的图元类型的当前可视化效果
        // 目前的代码实现只考虑新添加的图元类型
        InitAllCurDraws();

        acutPrintf( _T( "\nMineGE::On_kLoadDwgMsg\n" ) );
        return retCode;
    }

    virtual AcRx::AppRetCode On_kUnloadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadDwgMsg ( pkt ) ;

        ReactorHelper::RemoveDocumentReactor( curDoc() );

        acutPrintf( _T( "\nMineGE::On_kUnloadDwgMsg\n" ) );

        return retCode;
    }

    virtual void RegisterServerComponents ()
    {
    }

    static void MineGE_ZoomTest( void )
    {
        AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "请选择一个图元实体:" ) );
        if( objId.isNull() ) return;

        ArxEntityHelper::ZoomToEntity( objId );
    }

    /*static void MineGE_movedata(void)
    {
    	extern void MoveData();

    	MoveData();
    }*/
} ;

IMPLEMENT_ARX_ENTRYPOINT( CMineGEApp )

ACED_ARXCOMMAND_ENTRY_AUTO( CMineGEApp, MineGE,  _ZoomTest, ZoomTest, ACRX_CMD_TRANSPARENT, NULL )
//ACED_ARXCOMMAND_ENTRY_AUTO(CMineGEApp, MineGE,  _movedata, movedata, ACRX_CMD_TRANSPARENT, NULL)
