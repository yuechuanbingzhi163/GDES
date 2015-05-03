#include "StdAfx.h"

#if defined(_DEBUG) && !defined(AC_FULL_DEBUG)
#error _DEBUG should not be defined except in internal Adesk debug builds
#endif

#include "ArxDbgRbList.h"
//#include "ArxDbgUtils.h"
#include "RawResbufHelper.h"

/******************************************************************
**
**  ArxDbgRbList::addToEmptyList
**      Helper function for other add functions to handle
**  the special case of adding to an empty list.
**
**  **jma
**
****************************/

bool
ArxDbgRbList::addToEmptyList( resbuf* newNode )
{
    ASSERT( newNode != NULL );

    if ( m_head == NULL )
    {
        m_head = newNode;
        m_tail = newNode;
        return true;
    }
    else
        return false;
}

/****************************************************************************
**
**  ArxDbgRbList::ArxDbgRbList
**
**  **jma
**
*************************************/

ArxDbgRbList::ArxDbgRbList()
    :   m_head( NULL ),
        m_tail( NULL )
{
}

/****************************************************************************
**
**  ArxDbgRbList::ArxDbgRbList
**
**  **jma
**
*************************************/

ArxDbgRbList::ArxDbgRbList( resbuf* adoptedList )
{
    m_head = adoptedList;
    m_tail = tailOfResbufChain( m_head );
}

/****************************************************************************
**
**  ArxDbgRbList::ArxDbgRbList
**
**  **jma
**
*************************************/

ArxDbgRbList::ArxDbgRbList( const ArxDbgRbList& copyFrom )
{
    m_head = duplicateResbufChain( copyFrom.m_head );
    m_tail = tailOfResbufChain( m_head );
}

/****************************************************************************
**
**  ArxDbgRbList::~ArxDbgRbList
**
**  **jma
**
*************************************/

ArxDbgRbList::~ArxDbgRbList()
{
    if ( m_head )
        acutRelRb( m_head );
}

/****************************************************************************
**
**  ArxDbgRbList::operator=
**
**  **jma
**
*************************************/

ArxDbgRbList&
ArxDbgRbList::operator=( const ArxDbgRbList& copyFrom )
{
    // check for self-assignment
    if ( m_head == copyFrom.m_head )
        return *this;

    removeAll();
    m_head = duplicateResbufChain( copyFrom.m_head );
    m_tail = tailOfResbufChain( m_head );
    return *this;
}

/******************************************************************
**
**  ArxDbgRbList::addHead
**
**  **jma
**
****************************/

void
ArxDbgRbList::addHead( resbuf* newNode )
{
    ASSERT( newNode != NULL );

    if ( addToEmptyList( newNode ) == false )
    {
        newNode->rbnext = m_head;
        m_head = newNode;
    }
}

/******************************************************************
**
**  ArxDbgRbList::addTail
**
**  **jma
**
****************************/

void
ArxDbgRbList::addTail( resbuf* newNode )
{
    ASSERT( newNode != NULL );

    if ( addToEmptyList( newNode ) == false )
    {
        m_tail->rbnext = newNode;
        m_tail = newNode;
    }
}

/******************************************************************
**
**  ArxDbgRbList::append
**      append two rblists together, destroying the handle to
**  the second list.
**
**  **jma
**
****************************/

void
ArxDbgRbList::append( ArxDbgRbList& appendList )
{
    // NULL-OP if second list is NULL
    if ( appendList.m_head == NULL )
        return;

    // EASY-OP if first list is NULL, switch head ptr
    if ( m_head == NULL )
        m_head = appendList.m_head;
    else
        m_tail->rbnext = appendList.m_head;

    m_tail = appendList.m_tail;

    // The list has been handed over to list 1, so
    // reset member vars in list 2 to reflect the change.
    appendList.m_head = NULL;
    appendList.m_tail = NULL;
}

/******************************************************************
**
**  ArxDbgRbList::length
**
**  **jma
**
****************************/

int
ArxDbgRbList::length()
{
    int len = 0;
    resbuf* rb = m_head;
    while ( rb )
    {
        len++;
        rb = rb->rbnext;
    }
    return len;
}

/******************************************************************
**
**  ArxDbgRbList::adoptData
**
**  **jma
**
****************************/

void
ArxDbgRbList::adoptData( resbuf* adoptedList )
{
    removeAll();    // remove any existing list
    m_head = adoptedList;
    m_tail = tailOfResbufChain( m_head );
}

/******************************************************************
**
**  ArxDbgRbList::orphanData
**
**  **jma
**
****************************/

resbuf*
ArxDbgRbList::orphanData()
{
    resbuf* rb = m_head;
    m_head = NULL;
    m_tail = NULL;
    return rb;
}

/******************************************************************
**
**  ArxDbgRbList::detach
**
**  **jma
**
****************************/

resbuf*
ArxDbgRbList::detach( resbuf* nodeToDel )
{
    ASSERT( ( m_head != NULL ) && ( nodeToDel != NULL ) );

    // trap for safety
    if ( ( m_head == NULL ) || ( nodeToDel == NULL ) )
        return NULL;

    // see if it is first on the list
    if ( m_head == nodeToDel )
    {
        m_head = m_head->rbnext;
        nodeToDel->rbnext = NULL;
        if ( m_head == NULL )
            m_tail = NULL;    // removed only element

        return nodeToDel;
    }
    // node is somewhere in the middle or end of the list
    resbuf* rb = m_head;
    while ( rb->rbnext )
    {
        if ( rb->rbnext == nodeToDel )
        {
            if ( m_tail == nodeToDel )  // don't lose tail ptr
                m_tail = rb;

            rb->rbnext = nodeToDel->rbnext;
            nodeToDel->rbnext = NULL;
            return( nodeToDel );
        }
        rb = rb->rbnext;
    }
    ASSERT( rb != NULL );
    return NULL;
}

/******************************************************************
**
**  ArxDbgRbList::detach
**
**  **jma
**
****************************/

resbuf*
ArxDbgRbList::detach( resbuf* startPtr, resbuf* endPtr )
{
    ASSERT( m_head != NULL );
    ASSERT( startPtr != NULL );
    ASSERT( endPtr != NULL );

    // trap safety case
    if ( ( m_head == NULL ) || ( startPtr == NULL ) || ( endPtr == NULL ) )
        return NULL;

    // see if it is first on the list
    if ( m_head == startPtr )
    {
        m_head = endPtr->rbnext;
        endPtr->rbnext = NULL;
        if ( m_head == NULL )
            m_tail = NULL;    // removed only element

        return startPtr;
    }
    // node is somewhere in the middle or end of the list
    resbuf* rb = m_head;
    while ( rb->rbnext )
    {
        if ( rb->rbnext == startPtr )
        {
            if ( m_tail == endPtr )  // don't lose tail ptr
                m_tail = rb;

            rb->rbnext = endPtr->rbnext;
            endPtr->rbnext = NULL;
            return startPtr;
        }
        rb = rb->rbnext;
    }
    ASSERT( rb != NULL );
    return NULL;
}

/******************************************************************
**
**  ArxDbgRbList::remove
**
**  **jma
**
****************************/

void
ArxDbgRbList::remove( resbuf* nodeToDel )
{
    resbuf* rb = detach( nodeToDel );
    if ( rb )
        acutRelRb( rb );
}

/******************************************************************
**
**  ArxDbgRbList::removeAll
**      delete all elements of the list and ensure that the
**  member vars are updated to reflect the new state
**
**  **jma
**
****************************/

void
ArxDbgRbList::removeAll()
{
    if ( m_head )
        acutRelRb( m_head );

    m_head = NULL;
    m_tail = NULL;
}
