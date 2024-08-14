

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Tue Jan 19 04:14:07 2038
 */
/* Compiler settings for ontoumleaplugin.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0628 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __ontoumleaplugin_i_h__
#define __ontoumleaplugin_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DECLSPEC_XFGVIRT
#if defined(_CONTROL_FLOW_GUARD_XFG)
#define DECLSPEC_XFGVIRT(base, func) __declspec(xfg_virtual(base, func))
#else
#define DECLSPEC_XFGVIRT(base, func)
#endif
#endif

/* Forward Declarations */ 

#ifndef __IValidationResultControl_FWD_DEFINED__
#define __IValidationResultControl_FWD_DEFINED__
typedef interface IValidationResultControl IValidationResultControl;

#endif 	/* __IValidationResultControl_FWD_DEFINED__ */


#ifndef __OntoUML_FWD_DEFINED__
#define __OntoUML_FWD_DEFINED__

#ifdef __cplusplus
typedef class OntoUML OntoUML;
#else
typedef struct OntoUML OntoUML;
#endif /* __cplusplus */

#endif 	/* __OntoUML_FWD_DEFINED__ */


#ifndef __IOntoUML_FWD_DEFINED__
#define __IOntoUML_FWD_DEFINED__
typedef interface IOntoUML IOntoUML;

#endif 	/* __IOntoUML_FWD_DEFINED__ */


#ifndef __ValidationResultControl_FWD_DEFINED__
#define __ValidationResultControl_FWD_DEFINED__

#ifdef __cplusplus
typedef class ValidationResultControl ValidationResultControl;
#else
typedef struct ValidationResultControl ValidationResultControl;
#endif /* __cplusplus */

#endif 	/* __ValidationResultControl_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "shobjidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IValidationResultControl_INTERFACE_DEFINED__
#define __IValidationResultControl_INTERFACE_DEFINED__

/* interface IValidationResultControl */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IValidationResultControl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("53fb6b99-3b18-4d0c-aae9-bab17ea2141a")
    IValidationResultControl : public IDispatch
    {
    public:
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_BackColor( 
            /* [in] */ OLE_COLOR clr) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_BackColor( 
            /* [retval][out] */ OLE_COLOR *pclr) = 0;
        
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_FillColor( 
            /* [in] */ OLE_COLOR clr) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_FillColor( 
            /* [retval][out] */ OLE_COLOR *pclr) = 0;
        
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_ForeColor( 
            /* [in] */ OLE_COLOR clr) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_ForeColor( 
            /* [retval][out] */ OLE_COLOR *pclr) = 0;
        
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_Text( 
            /* [in] */ BSTR strText) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_Text( 
            /* [retval][out] */ BSTR *pstrText) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IValidationResultControlVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IValidationResultControl * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IValidationResultControl * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IValidationResultControl * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IValidationResultControl * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IValidationResultControl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IValidationResultControl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IValidationResultControl * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        DECLSPEC_XFGVIRT(IValidationResultControl, put_BackColor)
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_BackColor )( 
            IValidationResultControl * This,
            /* [in] */ OLE_COLOR clr);
        
        DECLSPEC_XFGVIRT(IValidationResultControl, get_BackColor)
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BackColor )( 
            IValidationResultControl * This,
            /* [retval][out] */ OLE_COLOR *pclr);
        
        DECLSPEC_XFGVIRT(IValidationResultControl, put_FillColor)
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_FillColor )( 
            IValidationResultControl * This,
            /* [in] */ OLE_COLOR clr);
        
        DECLSPEC_XFGVIRT(IValidationResultControl, get_FillColor)
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FillColor )( 
            IValidationResultControl * This,
            /* [retval][out] */ OLE_COLOR *pclr);
        
        DECLSPEC_XFGVIRT(IValidationResultControl, put_ForeColor)
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ForeColor )( 
            IValidationResultControl * This,
            /* [in] */ OLE_COLOR clr);
        
        DECLSPEC_XFGVIRT(IValidationResultControl, get_ForeColor)
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ForeColor )( 
            IValidationResultControl * This,
            /* [retval][out] */ OLE_COLOR *pclr);
        
        DECLSPEC_XFGVIRT(IValidationResultControl, put_Text)
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Text )( 
            IValidationResultControl * This,
            /* [in] */ BSTR strText);
        
        DECLSPEC_XFGVIRT(IValidationResultControl, get_Text)
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Text )( 
            IValidationResultControl * This,
            /* [retval][out] */ BSTR *pstrText);
        
        END_INTERFACE
    } IValidationResultControlVtbl;

    interface IValidationResultControl
    {
        CONST_VTBL struct IValidationResultControlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IValidationResultControl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IValidationResultControl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IValidationResultControl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IValidationResultControl_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IValidationResultControl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IValidationResultControl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IValidationResultControl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IValidationResultControl_put_BackColor(This,clr)	\
    ( (This)->lpVtbl -> put_BackColor(This,clr) ) 

#define IValidationResultControl_get_BackColor(This,pclr)	\
    ( (This)->lpVtbl -> get_BackColor(This,pclr) ) 

#define IValidationResultControl_put_FillColor(This,clr)	\
    ( (This)->lpVtbl -> put_FillColor(This,clr) ) 

#define IValidationResultControl_get_FillColor(This,pclr)	\
    ( (This)->lpVtbl -> get_FillColor(This,pclr) ) 

#define IValidationResultControl_put_ForeColor(This,clr)	\
    ( (This)->lpVtbl -> put_ForeColor(This,clr) ) 

#define IValidationResultControl_get_ForeColor(This,pclr)	\
    ( (This)->lpVtbl -> get_ForeColor(This,pclr) ) 

#define IValidationResultControl_put_Text(This,strText)	\
    ( (This)->lpVtbl -> put_Text(This,strText) ) 

#define IValidationResultControl_get_Text(This,pstrText)	\
    ( (This)->lpVtbl -> get_Text(This,pstrText) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IValidationResultControl_INTERFACE_DEFINED__ */



#ifndef __ontoumleapluginLib_LIBRARY_DEFINED__
#define __ontoumleapluginLib_LIBRARY_DEFINED__

/* library ontoumleapluginLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_ontoumleapluginLib;

EXTERN_C const CLSID CLSID_OntoUML;

#ifdef __cplusplus

class DECLSPEC_UUID("bccaf007-4701-4f10-a1a2-28d5a46b0a8c")
OntoUML;
#endif

#ifndef __IOntoUML_INTERFACE_DEFINED__
#define __IOntoUML_INTERFACE_DEFINED__

/* interface IOntoUML */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IOntoUML;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("69969391-e037-417b-b925-e0195e06afcd")
    IOntoUML : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE EA_Connect( 
            /* [in] */ /* external definition not present */ IDualRepository *pRepo,
            /* [retval][out] */ BSTR *pRetVal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EA_GetMenuItems( 
            /* [in] */ /* external definition not present */ IDualRepository *pRepo,
            /* [in] */ BSTR bstrMenuLocation,
            /* [in] */ BSTR bstrMenuName,
            /* [retval][out] */ VARIANT *pRetVal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EA_GetMenuState( 
            /* [in] */ /* external definition not present */ IDualRepository *pRepo,
            /* [in] */ BSTR bstrMenuLocation,
            /* [in] */ BSTR bstrMenuName,
            /* [in] */ BSTR bstrItemName,
            /* [out] */ VARIANT_BOOL *pIsEnabled,
            /* [out] */ VARIANT_BOOL *pIsChecked) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EA_MenuClick( 
            /* [in] */ /* external definition not present */ IDualRepository *pRepo,
            /* [in] */ BSTR bstrMenuLocation,
            /* [in] */ BSTR bstrMenuName,
            /* [in] */ BSTR bstrItemName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EA_OnOutputItemClicked( 
            /* [in] */ /* external definition not present */ IDualRepository *pRepo,
            /* [in] */ BSTR bstrTabName,
            /* [in] */ BSTR bstrLineText,
            /* [in] */ int Id) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EA_OnOutputItemDoubleClicked( 
            /* [in] */ /* external definition not present */ IDualRepository *pRepo,
            /* [in] */ BSTR bstrTabName,
            /* [in] */ BSTR bstrLineText,
            /* [in] */ int Id) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IOntoUMLVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IOntoUML * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IOntoUML * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IOntoUML * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IOntoUML * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IOntoUML * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IOntoUML * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IOntoUML * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        DECLSPEC_XFGVIRT(IOntoUML, EA_Connect)
        HRESULT ( STDMETHODCALLTYPE *EA_Connect )( 
            IOntoUML * This,
            /* [in] */ /* external definition not present */ IDualRepository *pRepo,
            /* [retval][out] */ BSTR *pRetVal);
        
        DECLSPEC_XFGVIRT(IOntoUML, EA_GetMenuItems)
        HRESULT ( STDMETHODCALLTYPE *EA_GetMenuItems )( 
            IOntoUML * This,
            /* [in] */ /* external definition not present */ IDualRepository *pRepo,
            /* [in] */ BSTR bstrMenuLocation,
            /* [in] */ BSTR bstrMenuName,
            /* [retval][out] */ VARIANT *pRetVal);
        
        DECLSPEC_XFGVIRT(IOntoUML, EA_GetMenuState)
        HRESULT ( STDMETHODCALLTYPE *EA_GetMenuState )( 
            IOntoUML * This,
            /* [in] */ /* external definition not present */ IDualRepository *pRepo,
            /* [in] */ BSTR bstrMenuLocation,
            /* [in] */ BSTR bstrMenuName,
            /* [in] */ BSTR bstrItemName,
            /* [out] */ VARIANT_BOOL *pIsEnabled,
            /* [out] */ VARIANT_BOOL *pIsChecked);
        
        DECLSPEC_XFGVIRT(IOntoUML, EA_MenuClick)
        HRESULT ( STDMETHODCALLTYPE *EA_MenuClick )( 
            IOntoUML * This,
            /* [in] */ /* external definition not present */ IDualRepository *pRepo,
            /* [in] */ BSTR bstrMenuLocation,
            /* [in] */ BSTR bstrMenuName,
            /* [in] */ BSTR bstrItemName);
        
        DECLSPEC_XFGVIRT(IOntoUML, EA_OnOutputItemClicked)
        HRESULT ( STDMETHODCALLTYPE *EA_OnOutputItemClicked )( 
            IOntoUML * This,
            /* [in] */ /* external definition not present */ IDualRepository *pRepo,
            /* [in] */ BSTR bstrTabName,
            /* [in] */ BSTR bstrLineText,
            /* [in] */ int Id);
        
        DECLSPEC_XFGVIRT(IOntoUML, EA_OnOutputItemDoubleClicked)
        HRESULT ( STDMETHODCALLTYPE *EA_OnOutputItemDoubleClicked )( 
            IOntoUML * This,
            /* [in] */ /* external definition not present */ IDualRepository *pRepo,
            /* [in] */ BSTR bstrTabName,
            /* [in] */ BSTR bstrLineText,
            /* [in] */ int Id);
        
        END_INTERFACE
    } IOntoUMLVtbl;

    interface IOntoUML
    {
        CONST_VTBL struct IOntoUMLVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOntoUML_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IOntoUML_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IOntoUML_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IOntoUML_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IOntoUML_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IOntoUML_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IOntoUML_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IOntoUML_EA_Connect(This,pRepo,pRetVal)	\
    ( (This)->lpVtbl -> EA_Connect(This,pRepo,pRetVal) ) 

#define IOntoUML_EA_GetMenuItems(This,pRepo,bstrMenuLocation,bstrMenuName,pRetVal)	\
    ( (This)->lpVtbl -> EA_GetMenuItems(This,pRepo,bstrMenuLocation,bstrMenuName,pRetVal) ) 

#define IOntoUML_EA_GetMenuState(This,pRepo,bstrMenuLocation,bstrMenuName,bstrItemName,pIsEnabled,pIsChecked)	\
    ( (This)->lpVtbl -> EA_GetMenuState(This,pRepo,bstrMenuLocation,bstrMenuName,bstrItemName,pIsEnabled,pIsChecked) ) 

#define IOntoUML_EA_MenuClick(This,pRepo,bstrMenuLocation,bstrMenuName,bstrItemName)	\
    ( (This)->lpVtbl -> EA_MenuClick(This,pRepo,bstrMenuLocation,bstrMenuName,bstrItemName) ) 

#define IOntoUML_EA_OnOutputItemClicked(This,pRepo,bstrTabName,bstrLineText,Id)	\
    ( (This)->lpVtbl -> EA_OnOutputItemClicked(This,pRepo,bstrTabName,bstrLineText,Id) ) 

#define IOntoUML_EA_OnOutputItemDoubleClicked(This,pRepo,bstrTabName,bstrLineText,Id)	\
    ( (This)->lpVtbl -> EA_OnOutputItemDoubleClicked(This,pRepo,bstrTabName,bstrLineText,Id) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IOntoUML_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_ValidationResultControl;

#ifdef __cplusplus

class DECLSPEC_UUID("dd659e12-5d8d-4207-9403-3968abec3db1")
ValidationResultControl;
#endif
#endif /* __ontoumleapluginLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  BSTR_UserSize64(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal64(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal64(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree64(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


