#include "stdafx.h"

// 执行zoom命令
void ExecuteZoomCommand1( const AcGePoint3d& minPt, const AcGePoint3d& maxPt )
{
    /*
     * 发送命令方法(一)
     * 命令按照在cad中的执行顺序输入即可
     * 空格表示回车
     * 注意：命令字符串最后一定要有一个空格(回车)
     */
    CString cmd;
    cmd.Format( _T( "ZOOM W %.3f,%.3f,%.3f %.3f,%.3f,%.3f " ), minPt.x, minPt.y, minPt.z, maxPt.x, maxPt.y, maxPt.z );
    acDocManager->sendStringToExecute( curDoc(), cmd, true, false, false ); // 不显示命令执行过程
}

void ExecuteZoomCommand2( const AcGePoint3d& minPt, const AcGePoint3d& maxPt )
{
    /*
     * 发送命令方法(二)
     * 注：acedCommand方法在modeless dialog中无法使用
     * 参考: "Code Differences under the Application Execution Context" topic in ARX docs
     */
    // 发送cad命令：ZOOM-->W (缩放到图元的窗口范围)
    acedCommand( RTSTR, _T( "ZOOM" ), RTSTR, _T( "W" ), RT3DPOINT, asDblArray( minPt ), RT3DPOINT, asDblArray( maxPt ), 0 );
    //acedCommand(RTSTR, _T("ZOOM"), RTSTR, _T("A"), 0);
}

// don't forget to add for Autocad2007/2008/2009 this line
//#import "acax17enu.tlb" no_implementation raw_interfaces_only named_guids

// for Autocad2010/2011/2012 this line
// 后面的"no_implementation raw_interfaces_only named_guids"不能删除!!!
//#import "acax18enu.tlb" no_implementation raw_interfaces_only named_guids
//
//static void ZommWindowUseCom(const AcGePoint3d& minPt, const AcGePoint3d& maxPt)
//{
//	AcGePoint3d plLowerLeft = minPt, ptUpperRight = maxPt; //-> it's your object extends coords
//
//	long i = 0;
//	CComPtr<AutoCAD::IAcadApplication> IApp;
//	CComPtr<IDispatch> pDisp = acedGetAcadWinApp()->GetIDispatch(TRUE);
//	if(!pDisp) return;
//	HRESULT hr = pDisp->QueryInterface(AutoCAD::IID_IAcadApplication,(void**)&IApp);
//
//
//	SAFEARRAYBOUND rgsaBound;
//	rgsaBound.lLbound = 0L;
//	rgsaBound.cElements = 3;
//	SAFEARRAY* pLLPoint = NULL;
//	pLLPoint = SafeArrayCreate(VT_R8, 1, &rgsaBound);
//	// X value
//
//	SafeArrayPutElement(pLLPoint, &i, &plLowerLeft.x); i++;
//	SafeArrayPutElement(pLLPoint, &i, &plLowerLeft.y); i++;// Y value
//	SafeArrayPutElement(pLLPoint, &i, &plLowerLeft.z);// Z value
//	VARIANT ptLL;
//	VariantInit(&ptLL);
//	V_VT(&ptLL) = VT_ARRAY | VT_R8;
//	V_ARRAY(&ptLL) = pLLPoint;
//
//
//	SAFEARRAY* pURPoint = NULL;
//	pURPoint = SafeArrayCreate(VT_R8, 1, &rgsaBound);
//	i = 0;
//	SafeArrayPutElement(pURPoint, &i, &ptUpperRight.x);i++;// X value
//	SafeArrayPutElement(pURPoint, &i, &ptUpperRight.y);i++;// Y value
//	SafeArrayPutElement(pURPoint, &i, &ptUpperRight.z);// Z value
//	VARIANT ptUR;
//	VariantInit(&ptUR);
//	V_VT(&ptUR) = VT_ARRAY | VT_R8;
//	V_ARRAY(&ptUR) = pURPoint;
//
//	IApp->ZoomWindow(ptLL,ptUR); // zooming
//
//	VariantClear(&ptLL);
//	VariantClear(&ptUR);
//}

static void GetCurrentView( AcDbViewTableRecord& view )
{
    struct resbuf rb;
    struct resbuf wcs, ucs, dcs; // 转换坐标时使用的坐标系

    wcs.restype = RTSHORT;
    wcs.resval.rint = 0;
    ucs.restype = RTSHORT;
    ucs.resval.rint = 1;
    dcs.restype = RTSHORT;
    dcs.resval.rint = 2;

    // 获得当前视口的“查看”模式
    acedGetVar( _T( "VIEWMODE" ), &rb );
    view.setPerspectiveEnabled( rb.resval.rint & 1 );
    view.setFrontClipEnabled( rb.resval.rint & 2 );
    view.setBackClipEnabled( rb.resval.rint & 4 );
    view.setFrontClipAtEye( !( rb.resval.rint & 16 ) );

    // 当前视口中视图的中心点（UCS坐标）
    acedGetVar( _T( "VIEWCTR" ), &rb );
    acedTrans( rb.resval.rpoint, &ucs, &dcs, 0, rb.resval.rpoint );
    view.setCenterPoint( AcGePoint2d( rb.resval.rpoint[X],
                                      rb.resval.rpoint[Y] ) );

    // 当前视口透视图中的镜头焦距长度（单位为毫米）
    acedGetVar( _T( "LENSLENGTH" ), &rb );
    view.setLensLength( rb.resval.rreal );

    // 当前视口中目标点的位置（以 UCS 坐标表示）
    acedGetVar( _T( "TARGET" ), &rb );
    acedTrans( rb.resval.rpoint, &ucs, &wcs, 0, rb.resval.rpoint );
    view.setTarget( AcGePoint3d( rb.resval.rpoint[X],
                                 rb.resval.rpoint[Y], rb.resval.rpoint[Z] ) );
    // 当前视口的观察方向（UCS）
    acedGetVar( _T( "VIEWDIR" ), &rb );
    acedTrans( rb.resval.rpoint, &ucs, &wcs, 1, rb.resval.rpoint );
    view.setViewDirection( AcGeVector3d( rb.resval.rpoint[X],
                                         rb.resval.rpoint[Y], rb.resval.rpoint[Z] ) );

    // 当前视口的视图高度（图形单位）
    acedGetVar( _T( "VIEWSIZE" ), &rb );
    view.setHeight( rb.resval.rreal );
    double height = rb.resval.rreal;

    // 以像素为单位的当前视口的大小（X 和 Y 值）
    acedGetVar( _T( "SCREENSIZE" ), &rb );
    view.setWidth( rb.resval.rpoint[X] / rb.resval.rpoint[Y] * height );

    // 当前视口的视图扭转角
    acedGetVar( _T( "VIEWTWIST" ), &rb );
    view.setViewTwist( rb.resval.rreal );

    // 将模型选项卡或最后一个布局选项卡置为当前
    acedGetVar( _T( "TILEMODE" ), &rb );
    int tileMode = rb.resval.rint;
    // 设置当前视口的标识码
    acedGetVar( _T( "CVPORT" ), &rb );
    int cvport = rb.resval.rint;

    // 是否是模型空间的视图
    bool paperspace = ( ( tileMode == 0 ) && ( cvport == 1 ) ) ? true : false;
    view.setIsPaperspaceView( paperspace );

    if ( !paperspace )
    {
        // 当前视口中前向剪裁平面到目标平面的偏移量
        acedGetVar( _T( "FRONTZ" ), &rb );
        view.setFrontClipDistance( rb.resval.rreal );

        // 获得当前视口后向剪裁平面到目标平面的偏移值
        acedGetVar( _T( "BACKZ" ), &rb );
        view.setBackClipDistance( rb.resval.rreal );
    }
    else
    {
        view.setFrontClipDistance( 0.0 );
        view.setBackClipDistance( 0.0 );
    }
}

void ZoomWindowUseView( const AcGePoint3d& minPt, const AcGePoint3d& maxPt )
{
    ads_pointp pt1 = asDblArray( minPt );
    ads_pointp pt2 = asDblArray( maxPt );

    struct resbuf wcs, ucs, dcs; // 转换坐标时使用的坐标系统
    wcs.restype = RTSHORT;
    wcs.resval.rint = 0;
    ucs.restype = RTSHORT;
    ucs.resval.rint = 1;
    dcs.restype = RTSHORT;
    dcs.resval.rint = 2;
    acedTrans( pt1, &ucs, &dcs, 0, pt1 );
    acedTrans( pt2, &ucs, &dcs, 0, pt2 );

    AcDbViewTableRecord view;
    GetCurrentView( view );

    // 设置视图的中心点
    view.setCenterPoint( AcGePoint2d( ( pt1[X] + pt2[X] ) / 2, ( pt1[Y] + pt2[Y] ) / 2 ) );

    // 设置视图的高度和宽度
    view.setHeight( fabs( pt1[Y] - pt2[Y] ) );
    view.setWidth( fabs( pt1[X] - pt2[X] ) );

    // 将视图对象设置为当前视图
    acedSetCurrentView( &view, NULL );
}

// google搜索后，有人说acgsGetGsView只有在3d模式才能正常返回
// 2d模式返回null，如果第2个参数bCreateIfNone=true，创建的view也是3d的
// 经过试验，也确实如此...
// 关键词："SHADEMODE and AcGsView",
// 来源: Autodesk Discussion Groups
static AcGsView* GetGsView()
{
    resbuf rbCvport;
    acedGetVar( _T( "CVPORT" ), &rbCvport );
    int iVport = rbCvport.resval.rint;

    resbuf rbTile;
    acedGetVar( _T( "TILEMODE" ), &rbTile );
    int iTile = rbTile.resval.rint;

    acutPrintf( _T( "\n视口:%d, 模型空间选项卡:%d" ), iVport, iTile );

    if( iTile == 0 && iVport == 1 )
    {
        acutPrintf( _T( "\n模型空间选项卡没有激活" ), iVport );
        return 0;
    }
    return acgsGetGsView( iVport, false );
}

void ZoomWindowUseGSView( const AcGePoint3d& minPt, const AcGePoint3d& maxPt )
{
    AcGsView* pView = GetGsView();
    if( pView == 0 )
    {
        acutPrintf( _T( "\n获取GsView指针失败" ) );
        return;
    }

    pView->zoomExtents( minPt, maxPt );
    pView->invalidate();
    pView->update();
}
