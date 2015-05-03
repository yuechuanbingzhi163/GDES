#pragma once

// global functions applicable to raw resbuf usage but also used as helper
// functions for ArxDbgRbList class
extern int			dxfCodeToDataType(int resType);
extern void        dxfToStr(const resbuf* rb, CString& dxfCodeStr, CString& valueStr);
// Unicode: Leaving as char type because it is a buffer
extern LPCTSTR     bytesToHexStr(char* buffer, int len, CString& hexStr);
extern resbuf*     duplicateResbufNode(resbuf* rb);
extern resbuf*     duplicateResbufChain(resbuf* rb);
extern resbuf*     duplicateResbufChain(resbuf* startPtr, resbuf* endPtr);
extern resbuf*     ptArrayToResbuf(const AcGePoint3dArray& ptArray);
extern resbuf*     tailOfResbufChain(resbuf* const rb);
extern void        printResbufChain(resbuf* const rb);