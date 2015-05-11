

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Mon May 11 10:20:23 2015
 */
/* Compiler settings for _SoSmiley.idl:
    Oicf, W1, Zp8, env=Win64 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_ITimerHandler,0x8A0E5678,0x792F,0x439b,0xAE,0xDD,0xE8,0xD4,0xAB,0x60,0x20,0x40);


MIDL_DEFINE_GUID(IID, IID_ISmileySource,0xE9FFF8D9,0x7585,0x42ce,0xB6,0xCE,0x33,0x33,0x62,0x83,0x99,0x4D);


MIDL_DEFINE_GUID(IID, IID_ISmileyHost,0x0F3687B9,0x333F,0x48a4,0x90,0x01,0xC9,0x94,0x45,0x5B,0x43,0x0C);


MIDL_DEFINE_GUID(IID, IID_ISoSmileyCtrl,0x3286141B,0xC87F,0x4052,0xB6,0xA2,0x37,0x63,0x91,0xDC,0xDA,0xF6);


MIDL_DEFINE_GUID(IID, LIBID_SoSmiley,0x2FC6E96F,0xB61D,0x4396,0x8F,0xAE,0x58,0x68,0x91,0xEF,0xF0,0x11);


MIDL_DEFINE_GUID(CLSID, CLSID_CSoSmileyCtrl,0xD29E0BDE,0xCFDA,0x4b93,0x92,0x9A,0x87,0x7A,0xB4,0x55,0x7B,0xD8);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



