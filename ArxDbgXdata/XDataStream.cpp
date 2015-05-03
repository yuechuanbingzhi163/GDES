#include "StdAfx.h"
#include "XDataStream.h"
#include "ArxDbgXdata.h"

DrawParamReader::DrawParamReader( ArxDbgXdataListIterator* pXDataList ) : m_pXDataList( pXDataList )
{

}

void DrawParamReader::readInt( int& value )
{
    m_pXDataList->getInteger( value );
}

void DrawParamReader::readLong( long& value )
{
    m_pXDataList->getLong( value );
}

void DrawParamReader::readDouble( double& value )
{
    m_pXDataList->getReal( value );
}

void DrawParamReader::readString( CString& value )
{
    m_pXDataList->getString( value );
}

void DrawParamReader::readBoolean( bool& value )
{
    int i;
    m_pXDataList->getInteger( i );
    value = ( i > 0 );
}

void DrawParamReader::readPoint( AcGePoint3d& value )
{
    m_pXDataList->getPoint( value );
}

DrawParamWriter::DrawParamWriter( ArxDbgXdataList* pXDataList ) : m_pXDataList( pXDataList )
{

}

void DrawParamWriter::writeInt( int value )
{
    m_pXDataList->appendInteger( value );
}

void DrawParamWriter::writeLong( long value )
{
    m_pXDataList->appendLong( value );
}

void DrawParamWriter::writeDouble( double value )
{
    m_pXDataList->appendReal( value );
}

void DrawParamWriter::writeString( const CString& value )
{
    m_pXDataList->appendString( value );
}

void DrawParamWriter::writeBoolean( bool value )
{
    m_pXDataList->appendInteger( value ? 1 : 0 );
}

void DrawParamWriter::writePoint( const AcGePoint3d& value )
{
    m_pXDataList->appendPoint( value );
}
