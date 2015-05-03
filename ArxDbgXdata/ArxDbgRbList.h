#ifndef ARXDBGRBLIST_H
#define ARXDBGRBLIST_H

#include "dlimexp.h"

/******************************************************************
**
**	CLASS ArxDbgRbList
**		Thin wrapper for resbuf chains.  This should be used
**	by other classes and wrapped "thicker" whenever possible.
**	Pay attentiob to orphan/adopt semantics since there is some
**	direct access to the underlying representation.
**
**	**jma
**
****************************/

class DBG_XDATA_DLLIMPEXP ArxDbgRbList {

public:
                ArxDbgRbList();                     // constructs empty list
                ArxDbgRbList(resbuf* adoptedList);  // adopts resbuf chain
                ArxDbgRbList(const ArxDbgRbList&);	// copy constructor (does deep clone)
    virtual		~ArxDbgRbList();

        // assignment
    ArxDbgRbList&        operator=(const ArxDbgRbList& list);    // assign a copy

        // operations on an ArxDbgRbList
    int			length();
    bool        isEmpty()    { return m_head == NULL; }

    void        addHead(resbuf* newNode);
    void        addTail(resbuf* newNode);
    void        append(ArxDbgRbList& appendList);    // destructive to appended list

    resbuf*		detach(resbuf* nodeToDel);    // detach but don't delete
    resbuf*     detach(resbuf* startNode, resbuf* endNode);
    resbuf*     detachHead()    { return detach(m_head); }
    resbuf*     detachTail()    { return detach(m_tail); }

    void        remove(resbuf* nodeToDel);    // detach and delete
    void        removeHead()    { remove(m_head); }
    void        removeTail()    { remove(m_tail); }
    void        removeAll();

        // direct operations on resbuf chain, use only if absolutely necessary
        // these are considered "semi-private"
    void        adoptData(resbuf* adoptedList);	// object takes resposiblity for resbuf*
    resbuf*     orphanData();                   // object gives up responsibility for resbuf*
    resbuf*     data() { return m_head; }       // return pointer to data to pass to ADS

private:
    resbuf*		m_head;        // head of resbuf chain
    resbuf*     m_tail;        // tail of resbuf chain

    bool        addToEmptyList(resbuf* newElement);    // helper function
};


#endif    // ARXDBGRBLIST_H
