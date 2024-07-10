// OntoUML.h : Declaration of the COntoUML

#pragma once
#include "resource.h"       // main symbols


#include "ontoumleaplugintest_i.h"

// Menu items
#define MI_ONTOUML			L"-&OntoUML"
#define MI_VALIDATE_MODEL	L"Validate &Model"
#define MI_VALIDATE_DIAGRAM	L"Validate &Diagram"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;


// COntoUML

class ATL_NO_VTABLE COntoUML :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<COntoUML, &CLSID_OntoUML>,
	public IDispatchImpl<IOntoUML, &IID_IOntoUML, &LIBID_ontoumleaplugintestLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
private:
	CComPtr<IDualRepository> m_spRepo;

public:
	COntoUML()
	{
	}

DECLARE_REGISTRY_RESOURCEID(106)


BEGIN_COM_MAP(COntoUML)
	COM_INTERFACE_ENTRY(IOntoUML)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

	STDMETHOD(EA_Connect(IDualRepository* pRepo, BSTR* pRetVal));
	STDMETHOD(EA_GetMenuItems(IDualRepository* pRepo, BSTR bstrMenuLocation, BSTR bstrMenuName, VARIANT *pRetVal));
	STDMETHOD(EA_GetMenuState(IDualRepository* pRepo, BSTR bstrMenuLocation, BSTR bstrMenuName, BSTR bstrItemName, VARIANT_BOOL* pIsEnabled, VARIANT_BOOL* pIsChecked));
};

OBJECT_ENTRY_AUTO(__uuidof(OntoUML), COntoUML)
