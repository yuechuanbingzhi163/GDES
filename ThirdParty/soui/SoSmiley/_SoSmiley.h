

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Sun May 17 10:11:44 2015
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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef ___SoSmiley_h__
#define ___SoSmiley_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ITimerHandler_FWD_DEFINED__
#define __ITimerHandler_FWD_DEFINED__
typedef interface ITimerHandler ITimerHandler;
#endif 	/* __ITimerHandler_FWD_DEFINED__ */


#ifndef __ISmileySource_FWD_DEFINED__
#define __ISmileySource_FWD_DEFINED__
typedef interface ISmileySource ISmileySource;
#endif 	/* __ISmileySource_FWD_DEFINED__ */


#ifndef __ISmileyHost_FWD_DEFINED__
#define __ISmileyHost_FWD_DEFINED__
typedef interface ISmileyHost ISmileyHost;
#endif 	/* __ISmileyHost_FWD_DEFINED__ */


#ifndef __ISoSmileyCtrl_FWD_DEFINED__
#define __ISoSmileyCtrl_FWD_DEFINED__
typedef interface ISoSmileyCtrl ISoSmileyCtrl;
#endif 	/* __ISoSmileyCtrl_FWD_DEFINED__ */


#ifndef __CSoSmileyCtrl_FWD_DEFINED__
#define __CSoSmileyCtrl_FWD_DEFINED__

#ifdef __cplusplus
typedef class CSoSmileyCtrl CSoSmileyCtrl;
#else
typedef struct CSoSmileyCtrl CSoSmileyCtrl;
#endif /* __cplusplus */

#endif 	/* __CSoSmileyCtrl_FWD_DEFINED__ */


/* header files for imported files */
#include "prsht.h"
#include "mshtml.h"
#include "mshtmhst.h"
#include "exdisp.h"
#include "objsafe.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ITimerHandler_INTERFACE_DEFINED__
#define __ITimerHandler_INTERFACE_DEFINED__

/* interface ITimerHandler */
/* [unique][helpstring][nonextensible][uuid][object] */ 


EXTERN_C const IID IID_ITimerHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8A0E5678-792F-439b-AEDD-E8D4AB602040")
    ITimerHandler : public IUnknown
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnTimer( 
            /* [in] */ HDC hdc) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Clear( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetRect( 
            /* [out] */ LPRECT pRect) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITimerHandlerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITimerHandler * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITimerHandler * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITimerHandler * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OnTimer )( 
            ITimerHandler * This,
            /* [in] */ HDC hdc);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Clear )( 
            ITimerHandler * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetRect )( 
            ITimerHandler * This,
            /* [out] */ LPRECT pRect);
        
        END_INTERFACE
    } ITimerHandlerVtbl;

    interface ITimerHandler
    {
        CONST_VTBL struct ITimerHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITimerHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITimerHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITimerHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITimerHandler_OnTimer(This,hdc)	\
    ( (This)->lpVtbl -> OnTimer(This,hdc) ) 

#define ITimerHandler_Clear(This)	\
    ( (This)->lpVtbl -> Clear(This) ) 

#define ITimerHandler_GetRect(This,pRect)	\
    ( (This)->lpVtbl -> GetRect(This,pRect) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITimerHandler_INTERFACE_DEFINED__ */


#ifndef __ISmileySource_INTERFACE_DEFINED__
#define __ISmileySource_INTERFACE_DEFINED__

/* interface ISmileySource */
/* [unique][helpstring][nonextensible][uuid][object] */ 


EXTERN_C const IID IID_ISmileySource;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E9FFF8D9-7585-42ce-B6CE-33336283994D")
    ISmileySource : public IUnknown
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Stream_Load( 
            /* [in] */ LPSTREAM pStm) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Stream_Save( 
            /* [in] */ LPSTREAM pStm) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadFromID( 
            /* [in] */ UINT uID) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadFromFile( 
            /* [in] */ LPCWSTR pszFilePath) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetID( 
            /* [out] */ UINT *pID) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetFile( 
            /* [out] */ BSTR *bstrFile) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetFrameCount( 
            /* [out] */ int *pFrameCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetFrameDelay( 
            /* [in] */ int iFrame,
            /* [out] */ int *pFrameDelay) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetSize( 
            /* [out] */ LPSIZE pSize) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Draw( 
            /* [in] */ HDC hdc,
            /* [in] */ LPCRECT pRect,
            /* [in] */ int iFrame) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISmileySourceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISmileySource * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISmileySource * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISmileySource * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Stream_Load )( 
            ISmileySource * This,
            /* [in] */ LPSTREAM pStm);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Stream_Save )( 
            ISmileySource * This,
            /* [in] */ LPSTREAM pStm);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadFromID )( 
            ISmileySource * This,
            /* [in] */ UINT uID);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadFromFile )( 
            ISmileySource * This,
            /* [in] */ LPCWSTR pszFilePath);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetID )( 
            ISmileySource * This,
            /* [out] */ UINT *pID);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetFile )( 
            ISmileySource * This,
            /* [out] */ BSTR *bstrFile);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetFrameCount )( 
            ISmileySource * This,
            /* [out] */ int *pFrameCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetFrameDelay )( 
            ISmileySource * This,
            /* [in] */ int iFrame,
            /* [out] */ int *pFrameDelay);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSize )( 
            ISmileySource * This,
            /* [out] */ LPSIZE pSize);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Draw )( 
            ISmileySource * This,
            /* [in] */ HDC hdc,
            /* [in] */ LPCRECT pRect,
            /* [in] */ int iFrame);
        
        END_INTERFACE
    } ISmileySourceVtbl;

    interface ISmileySource
    {
        CONST_VTBL struct ISmileySourceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISmileySource_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISmileySource_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISmileySource_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISmileySource_Stream_Load(This,pStm)	\
    ( (This)->lpVtbl -> Stream_Load(This,pStm) ) 

#define ISmileySource_Stream_Save(This,pStm)	\
    ( (This)->lpVtbl -> Stream_Save(This,pStm) ) 

#define ISmileySource_LoadFromID(This,uID)	\
    ( (This)->lpVtbl -> LoadFromID(This,uID) ) 

#define ISmileySource_LoadFromFile(This,pszFilePath)	\
    ( (This)->lpVtbl -> LoadFromFile(This,pszFilePath) ) 

#define ISmileySource_GetID(This,pID)	\
    ( (This)->lpVtbl -> GetID(This,pID) ) 

#define ISmileySource_GetFile(This,bstrFile)	\
    ( (This)->lpVtbl -> GetFile(This,bstrFile) ) 

#define ISmileySource_GetFrameCount(This,pFrameCount)	\
    ( (This)->lpVtbl -> GetFrameCount(This,pFrameCount) ) 

#define ISmileySource_GetFrameDelay(This,iFrame,pFrameDelay)	\
    ( (This)->lpVtbl -> GetFrameDelay(This,iFrame,pFrameDelay) ) 

#define ISmileySource_GetSize(This,pSize)	\
    ( (This)->lpVtbl -> GetSize(This,pSize) ) 

#define ISmileySource_Draw(This,hdc,pRect,iFrame)	\
    ( (This)->lpVtbl -> Draw(This,hdc,pRect,iFrame) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISmileySource_INTERFACE_DEFINED__ */


#ifndef __ISmileyHost_INTERFACE_DEFINED__
#define __ISmileyHost_INTERFACE_DEFINED__

/* interface ISmileyHost */
/* [unique][helpstring][nonextensible][uuid][object] */ 


EXTERN_C const IID IID_ISmileyHost;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0F3687B9-333F-48a4-9001-C994455B430C")
    ISmileyHost : public IUnknown
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SendMessageW( 
            /* [in] */ UINT uMsg,
            /* [in] */ WPARAM wParam,
            /* [in] */ LPARAM lParam,
            /* [out] */ LRESULT *pRet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetHostRect( 
            /* [out] */ LPRECT prcHost) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE InvalidateRect( 
            /* [in] */ LPCRECT pRect) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateSource( 
            /* [out][in] */ ISmileySource **ppSource) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetTimer( 
            /* [in] */ ITimerHandler *pTimerHander,
            /* [in] */ int nInterval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KillTimer( 
            /* [in] */ ITimerHandler *pTimerHander) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnTimer( 
            /* [in] */ int nInterval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ClearTimer( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISmileyHostVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISmileyHost * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISmileyHost * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISmileyHost * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SendMessageW )( 
            ISmileyHost * This,
            /* [in] */ UINT uMsg,
            /* [in] */ WPARAM wParam,
            /* [in] */ LPARAM lParam,
            /* [out] */ LRESULT *pRet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetHostRect )( 
            ISmileyHost * This,
            /* [out] */ LPRECT prcHost);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *InvalidateRect )( 
            ISmileyHost * This,
            /* [in] */ LPCRECT pRect);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateSource )( 
            ISmileyHost * This,
            /* [out][in] */ ISmileySource **ppSource);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetTimer )( 
            ISmileyHost * This,
            /* [in] */ ITimerHandler *pTimerHander,
            /* [in] */ int nInterval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KillTimer )( 
            ISmileyHost * This,
            /* [in] */ ITimerHandler *pTimerHander);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OnTimer )( 
            ISmileyHost * This,
            /* [in] */ int nInterval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ClearTimer )( 
            ISmileyHost * This);
        
        END_INTERFACE
    } ISmileyHostVtbl;

    interface ISmileyHost
    {
        CONST_VTBL struct ISmileyHostVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISmileyHost_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISmileyHost_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISmileyHost_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISmileyHost_SendMessageW(This,uMsg,wParam,lParam,pRet)	\
    ( (This)->lpVtbl -> SendMessageW(This,uMsg,wParam,lParam,pRet) ) 

#define ISmileyHost_GetHostRect(This,prcHost)	\
    ( (This)->lpVtbl -> GetHostRect(This,prcHost) ) 

#define ISmileyHost_InvalidateRect(This,pRect)	\
    ( (This)->lpVtbl -> InvalidateRect(This,pRect) ) 

#define ISmileyHost_CreateSource(This,ppSource)	\
    ( (This)->lpVtbl -> CreateSource(This,ppSource) ) 

#define ISmileyHost_SetTimer(This,pTimerHander,nInterval)	\
    ( (This)->lpVtbl -> SetTimer(This,pTimerHander,nInterval) ) 

#define ISmileyHost_KillTimer(This,pTimerHander)	\
    ( (This)->lpVtbl -> KillTimer(This,pTimerHander) ) 

#define ISmileyHost_OnTimer(This,nInterval)	\
    ( (This)->lpVtbl -> OnTimer(This,nInterval) ) 

#define ISmileyHost_ClearTimer(This)	\
    ( (This)->lpVtbl -> ClearTimer(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISmileyHost_INTERFACE_DEFINED__ */


#ifndef __ISoSmileyCtrl_INTERFACE_DEFINED__
#define __ISoSmileyCtrl_INTERFACE_DEFINED__

/* interface ISoSmileyCtrl */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ISoSmileyCtrl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3286141B-C87F-4052-B6A2-376391DCDAF6")
    ISoSmileyCtrl : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Insert2Richedit( 
            /* [in] */ DWORD_PTR ole) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetSource( 
            /* [in] */ ISmileySource *pSource) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetSource( 
            /* [out] */ ISmileySource **ppSource) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISoSmileyCtrlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISoSmileyCtrl * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISoSmileyCtrl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISoSmileyCtrl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISoSmileyCtrl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISoSmileyCtrl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISoSmileyCtrl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISoSmileyCtrl * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Insert2Richedit )( 
            ISoSmileyCtrl * This,
            /* [in] */ DWORD_PTR ole);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetSource )( 
            ISoSmileyCtrl * This,
            /* [in] */ ISmileySource *pSource);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSource )( 
            ISoSmileyCtrl * This,
            /* [out] */ ISmileySource **ppSource);
        
        END_INTERFACE
    } ISoSmileyCtrlVtbl;

    interface ISoSmileyCtrl
    {
        CONST_VTBL struct ISoSmileyCtrlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISoSmileyCtrl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISoSmileyCtrl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISoSmileyCtrl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISoSmileyCtrl_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISoSmileyCtrl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISoSmileyCtrl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISoSmileyCtrl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISoSmileyCtrl_Insert2Richedit(This,ole)	\
    ( (This)->lpVtbl -> Insert2Richedit(This,ole) ) 

#define ISoSmileyCtrl_SetSource(This,pSource)	\
    ( (This)->lpVtbl -> SetSource(This,pSource) ) 

#define ISoSmileyCtrl_GetSource(This,ppSource)	\
    ( (This)->lpVtbl -> GetSource(This,ppSource) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISoSmileyCtrl_INTERFACE_DEFINED__ */



#ifndef __SoSmiley_LIBRARY_DEFINED__
#define __SoSmiley_LIBRARY_DEFINED__

/* library SoSmiley */
/* [helpstring][uuid][version] */ 


EXTERN_C const IID LIBID_SoSmiley;

EXTERN_C const CLSID CLSID_CSoSmileyCtrl;

#ifdef __cplusplus

class DECLSPEC_UUID("D29E0BDE-CFDA-4b93-929A-877AB4557BD8")
CSoSmileyCtrl;
#endif
#endif /* __SoSmiley_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  HDC_UserSize(     unsigned long *, unsigned long            , HDC * ); 
unsigned char * __RPC_USER  HDC_UserMarshal(  unsigned long *, unsigned char *, HDC * ); 
unsigned char * __RPC_USER  HDC_UserUnmarshal(unsigned long *, unsigned char *, HDC * ); 
void                      __RPC_USER  HDC_UserFree(     unsigned long *, HDC * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


