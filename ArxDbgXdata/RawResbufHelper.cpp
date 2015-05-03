#include "StdAfx.h"
#include "RawResbufHelper.h"

inline void
copyAdsPt( ads_point pt1, ads_point pt2 )
{
    pt1[0] = pt2[0];
    pt1[1] = pt2[1];
    pt1[2] = pt2[2];
}

/////////////////////////////////////////////////////////////////////

inline void
copyEname( ads_name ename1, ads_name ename2 )
{
    ename1[0] = ename2[0];
    ename1[1] = ename2[1];
}

/////////////////////////////////////////////////////////////////////////
///////////////////  GENERIC FUNCTIONS ON RAW RESBUFS  //////////////////
/////////////////////////////////////////////////////////////////////////

/************************************************************************
**
**  dxfCodeToDataType:
**
**  **jma
**
******************************************/

int
dxfCodeToDataType( int resType )
{
    // which data type is this value
    if ( ( resType >= 0 ) && ( resType <= 9 ) )
        return RTSTR;
    else if ( ( resType >= 10 ) && ( resType <= 17 ) )
        return RT3DPOINT;
    else if ( ( resType >= 38 ) && ( resType <= 59 ) )
        return RTREAL;
    else if ( ( resType >= 60 ) && ( resType <= 79 ) )
        return RTSHORT;
    else if ( ( resType >= 90 ) && ( resType <= 99 ) )
        return RTLONG;
    else if ( ( resType == 100 ) || ( resType == 101 ) || ( resType == 102 ) || ( resType == 105 ) )
        return RTSTR;
    else if ( ( resType >= 110 ) && ( resType <= 119 ) )
        return RT3DPOINT;
    else if ( ( resType >= 140 ) && ( resType <= 149 ) )
        return RTREAL;
    else if ( ( resType >= 170 ) && ( resType <= 179 ) )
        return RTSHORT;
    else if ( ( resType >= 210 ) && ( resType <= 219 ) )
        return RT3DPOINT;
    else if ( ( resType >= 270 ) && ( resType <= 299 ) )
        return RTSHORT;
    else if ( ( resType >= 300 ) && ( resType <= 309 ) )
        return RTSTR;
    else if ( ( resType >= 310 ) && ( resType <= 369 ) )
        return RTENAME;
    else if ( ( resType >= 370 ) && ( resType <= 379 ) )
        return RTSHORT;
    else if ( ( resType >= 380 ) && ( resType <= 389 ) )
        return RTSHORT;
    else if ( ( resType >= 390 ) && ( resType <= 399 ) )
        return RTENAME;
    else if ( ( resType >= 400 ) && ( resType <= 409 ) )
        return RTSHORT;
    else if ( ( resType >= 410 ) && ( resType <= 419 ) )
        return RTSTR;
    else if ( resType == 1004 )
        return resType;        // binary chunk
    else if ( ( resType >= 999 ) && ( resType <= 1009 ) )
        return RTSTR;
    else if ( ( resType >= 1010 ) && ( resType <= 1013 ) )
        return RT3DPOINT;
    else if ( ( resType >= 1038 ) && ( resType <= 1059 ) )
        return RTREAL;
    else if ( ( resType >= 1060 ) && ( resType <= 1070 ) )
        return RTSHORT;
    else if ( ( resType == 1071 ) )
        return RTLONG;
    else if ( ( resType < 0 ) || ( resType > 4999 ) )
        return resType;
    else
        return RTNONE;
}

/**********************************************************************
**
**  bytesToHexStr:
**
**  **jma
**
*****************************************/

// Unicode: Leaving as char type because it is a buffer
LPCTSTR
bytesToHexStr( char* buffer, int len, CString& hexStr )
{
    // Unicode: Leaving as char type because it is a byte
    unsigned char k;
    int j;
    CString tmpStr;

    hexStr.Empty();    // make sure nothing in it yet since we are concatenating it

    for ( j = 0; j < len; j++ )
    {
        k = static_cast<unsigned char>( buffer[j] );
        tmpStr.Format( _T( "%02hX" ), k );
        hexStr += tmpStr;
    }
    return hexStr;
}

/*********************************************************************
**
**  dxfToStr:
**
**  **jma
**
******************************************/

void
dxfToStr( const resbuf* rb, CString& dxfCodeStr, CString& valueStr )
{
    int dataType = dxfCodeToDataType( rb->restype );
    dxfCodeStr.Format( _T( "%d" ), rb->restype );

    CString tmpStr;

    switch ( dataType )
    {
    case RTSHORT:
        valueStr.Format( _T( "%d" ), rb->resval.rint );
        break;
    case RTLONG:
        valueStr.Format( _T( "%ld" ), rb->resval.rlong );
        break;
    case RTREAL:
        valueStr.Format( _T( "%g" ), rb->resval.rreal );
        break;
    case RTSTR:
        if ( rb->resval.rstring == NULL )
            valueStr = _T( "(NULL)" );
        else
            valueStr.Format( _T( "\"%s\"" ), rb->resval.rstring );
        break;
    case RT3DPOINT:
        valueStr.Format( _T( "(%g, %g, %g)" ), rb->resval.rpoint[X],
                         rb->resval.rpoint[Y], rb->resval.rpoint[Z] );
        break;
    case RTPOINT:
        valueStr.Format( _T( "(%g, %g)" ), rb->resval.rpoint[X], rb->resval.rpoint[Y] );
        break;
    case 1004:
        valueStr.Format( _T( "Binary Chunk: \"%s\"" ),
                         bytesToHexStr( rb->resval.rbinary.buf,
                                        rb->resval.rbinary.clen, tmpStr ) );
        break;
    case -6:
        valueStr = _T( "Extension Dictionary" );
        break;
    case -5:
        valueStr = _T( "Persistent Reactors" );
        break;
    case -4:
        valueStr.Format( _T( "Conditional Operator: \"%s\"" ), rb->resval.rstring );
        break;
    case -3:
        valueStr = _T( "Start of Xdata" );
        break;
    case -2:
        valueStr.Format( _T( "<Entity Name Reference: %8lx>" ), rb->resval.rlname[0] );
        break;
    case -1:
    case RTENAME:
        if ( ( rb->restype >= 330 ) && ( rb->restype < 340 ) )
            valueStr.Format( _T( "<Soft Pointer: %8lx>" ), rb->resval.rlname[0] );
        else if( ( rb->restype >= 340 ) && ( rb->restype < 350 ) )
            valueStr.Format( _T( "<Hard Pointer: %8lx>" ), rb->resval.rlname[0] );
        else if( ( rb->restype >= 350 ) && ( rb->restype < 360 ) )
            valueStr.Format( _T( "<Soft Ownership: %8lx>" ), rb->resval.rlname[0] );
        else if( ( rb->restype >= 360 ) && ( rb->restype < 370 ) )
            valueStr.Format( _T( "<Hard Ownership: %8lx>" ), rb->resval.rlname[0] );
        else if( ( rb->restype >= 390 ) && ( rb->restype < 399 ) )
            valueStr.Format( _T( "<Hard Pointer: %8lx>" ), rb->resval.rlname[0] );
        else
            valueStr.Format( _T( "<Entity Name: %8lx>" ), rb->resval.rlname[0] );
        break;
    case RTPICKS:
        valueStr.Format( _T( "<Selection Set: %8lx>" ), rb->resval.rlname[0] );
        break;
    case RTLB:
        valueStr = _T( "List Begin" );
        break;
    case RTLE:
        valueStr = _T( "List End" );
        break;
    case RTNIL:
        valueStr = _T( "NIL" );
        break;
    case RTT:
        valueStr = _T( "T" );
        break;
    default:
        valueStr = _T( "*Unknown*" );
        break;
    }
}

/******************************************************************
**
**  duplicateResbufNode
**      duplicate only a single node on the list.
**
**  **jma
**
****************************/

resbuf*
duplicateResbufNode( resbuf* rb )
{
    resbuf* newNode;
    int dataType;

    newNode = acutNewRb( rb->restype );  // standard out of mem handler??
    ASSERT( newNode != NULL );
    if ( newNode == NULL )
        return( NULL );

    dataType = dxfCodeToDataType( rb->restype );

    if ( dataType == RTSHORT )
        newNode->resval.rint = rb->resval.rint;
    else if ( dataType == RTREAL )
        newNode->resval.rreal = rb->resval.rreal;
    else if ( dataType == RTLONG )
        newNode->resval.rlong = rb->resval.rlong;
    else if ( ( dataType == RTSTR ) || ( dataType == -4 ) )
        newNode->resval.rstring = _tcsdup( rb->resval.rstring );
    else if ( ( dataType == RT3DPOINT ) || ( dataType == RTPOINT ) )
    {
        copyAdsPt( newNode->resval.rpoint, rb->resval.rpoint );
    }
    else if ( ( dataType == RTENAME ) || ( dataType == -1 ) || ( dataType == -2 ) || ( dataType == RTPICKS ) )
    {
        copyEname( newNode->resval.rlname, rb->resval.rlname );
    }
    else if ( dataType == -3 )
        ;       // no value
    else if ( dataType == 1004 )
    {
        // Unicode: Leaving as char type because it is a buffer
        newNode->resval.rbinary.buf = static_cast<char*>( malloc( rb->resval.rbinary.clen ) );
        memcpy( newNode->resval.rbinary.buf, rb->resval.rbinary.buf,
                rb->resval.rbinary.clen );
    }
    else
    {
        ASSERT( 0 );  // shouldn't happen
        return NULL;
    }
    return newNode;
}

/******************************************************************
**
**  duplicateResbufChain
**      duplicate the entire chain.
**
**  **jma
**
****************************/

resbuf*
duplicateResbufChain( resbuf* rb )
{
    resbuf* newNode;             // tmp dup node
    resbuf* lastNode = NULL;            // speed ptr to end of list
    resbuf* duplicate = NULL;    // returned duplicate list

    resbuf* tmp = rb;
    while ( tmp )
    {
        if ( ( newNode = duplicateResbufNode( tmp ) ) != NULL )
        {
            if ( duplicate == NULL )
            {
                duplicate = newNode;
                lastNode = newNode;
            }
            else
            {
                lastNode->rbnext = newNode;
                lastNode = lastNode->rbnext;
            }
        }
        tmp = tmp->rbnext;
    }
    return duplicate;
}

/******************************************************************
**
**  duplicateResbufChain
**      duplicate an range in the chain.
**
**  **jma
**
****************************/

resbuf*
duplicateResbufChain( resbuf* startPtr, resbuf* endPtr )
{
    ASSERT( startPtr != NULL );
    ASSERT( endPtr != NULL );

    // safety bail-out
    if ( ( startPtr == NULL ) || ( endPtr == NULL ) )
        return NULL;

    resbuf* newNode;        // tmp dup node
    resbuf* lastNode = NULL;        // speed ptr to end of list
    resbuf* duplicate = NULL;    // returned duplicate list

    resbuf* tmp = startPtr;
    while ( tmp && ( tmp != endPtr->rbnext ) )
    {
        if ( ( newNode = duplicateResbufNode( tmp ) ) != NULL )
        {
            if ( duplicate == NULL )
            {
                duplicate = newNode;
                lastNode = newNode;
            }
            else
            {
                lastNode->rbnext = newNode;
                lastNode = lastNode->rbnext;
            }
        }
        tmp = tmp->rbnext;
    }
    return duplicate;
}

/*********************************************************************
**
**  ptArrayToResbuf:
**      convert an array of 3d points to a resbuf chain
**  of 3d points.  Used to pass to selection set functions.
**
**  **jma
**
******************************************/

resbuf*
ptArrayToResbuf( const AcGePoint3dArray& ptArray )
{
    resbuf* ptList = NULL;        // overall list
    resbuf* lastRb = NULL;        // place holder to end of list
    resbuf* rb;
    int len = ptArray.length();
    for ( int i = 0; i < len; i++ )
    {
        if ( ( rb = acutNewRb( RT3DPOINT ) ) == NULL )
        {
            acutRelRb( ptList );
            return NULL;
        }
        copyAdsPt( rb->resval.rpoint, asDblArray( ptArray.at( i ) ) );
        if ( ptList == NULL )
        {
            ptList = rb;
            lastRb = rb;
        }
        else
        {
            lastRb->rbnext = rb;
            lastRb = rb;
        }
    }
    return ptList;
}

/*********************************************************************
**
**  tailOfResbufChain:
**      find the end of the resbuf chain.
**
**  **jma
**
******************************************/

resbuf*
tailOfResbufChain( resbuf* const rb )
{
    resbuf* tmp = rb;
    if ( tmp != NULL )
    {
        while ( tmp->rbnext != NULL )
            tmp = tmp->rbnext;
    }
    return tmp;
}

/*********************************************************************
**
**  printResbufChain:
**
**  **jma
**
******************************************/

void
printResbufChain( resbuf* const rb )
{
    CString dxfCodeStr, valueStr, tmpStr;
    resbuf* tmp = rb;
    while ( tmp )
    {
        dxfToStr( tmp, dxfCodeStr, valueStr );
        tmpStr.Format( _T( "\n(%s . %s)" ), dxfCodeStr, valueStr );
        acutPrintf( static_cast<LPCTSTR>( tmpStr ) );
        tmp = tmp->rbnext;
    }
}
