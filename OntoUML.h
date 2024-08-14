// OntoUML.h : Declaration of the COntoUML

#pragma once
#include "resource.h"       // main symbols


#include "ontoumleaplugin_i.h"

// Menu items
#define MI_ONTOUML			L"-&OntoUML"
#define MI_VALIDATE_MODEL	L"Validate &Model"
#define MI_VALIDATE_DIAGRAM	L"Validate &Diagram"

#define TAB_ONTOUML			L"OntoUML"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;

#include <memory>
#include <iostream>
#include <vector>

template<typename T>
class com_aware_allocator
{
public:
	using value_type = T;

	com_aware_allocator() = default;

	template<typename U>
	constexpr com_aware_allocator(const com_aware_allocator<U>&) noexcept {}

	T* allocate(std::size_t n) {
		std::cout << "Allocating " << n * sizeof(T) << " bytes" << std::endl;
		if (n > std::allocator_traits<com_aware_allocator>::max_size(*this)) {
			throw std::bad_alloc();
		}
		return static_cast<T*>(::operator new(n * sizeof(T)));
	}

	void deallocate(T* p, std::size_t) noexcept {
		std::cout << "Deallocating memory" << std::endl;
		::operator delete(p);
	}

	template<typename T2, typename... Args>
	void construct(T2* p, Args&&... args) {
		std::cout << "Constructing element" << std::endl;
		new(p) T2(std::forward<Args>(args)...);
		(*p)->AddRef();
	}

	template<typename T2>
	void destroy(T2* p) noexcept {
		std::cout << "Destroying element" << std::endl;
		(*p)->Release();
	}

	friend bool operator==(const com_aware_allocator&, const com_aware_allocator&) { return true; }
	friend bool operator!=(const com_aware_allocator&, const com_aware_allocator&) { return false; }
};

// COntoUML
class ATL_NO_VTABLE COntoUML :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<COntoUML, &CLSID_OntoUML>,
	public IDispatchImpl<IOntoUML, &IID_IOntoUML, &LIBID_ontoumleapluginLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
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

	template <class T>
	inline static HRESULT copyToVector(IDualCollection* pCollection, std::vector<T*, com_aware_allocator<T*>>& vobjCollection, std::function<bool(T *)> filter = [](T *p) -> bool { return true; })
	{
		if (!pCollection)
		{
			return E_INVALIDARG;
		}

		short sCount = 0;
		auto hr = pCollection->get_Count(&sCount);

		for (short i = 0; SUCCEEDED(hr) && i < sCount; i++)
		{
			CComPtr<IDispatch> spObject;
			hr = pCollection->GetAt(i, &spObject);

			CComQIPtr<T> spT = spObject;

			if (SUCCEEDED(hr) && spT && filter(spT))
			{
				vobjCollection.push_back(spT);
			}
		}

		return hr;
	};

private:
	//typedef std::vector < IDualDiagram*, com_aware_allocator<IDualDiagram*> > DiagramVector;
	static std::map<std::wstring, std::vector<std::wstring>> s_defaultRestrictedToValues;
	//std::map<long, std::tuple<DiagramVector::iterator, DiagramVector>> m_diagramsPerElementId; // for later use
	CComPtr<IDualDiagram> m_spCurrentDiagram;
	std::map<long, web::json::value> m_validationResultsPerOutputId;
	std::map<long, long> m_outputIdToElementId;
	// CComPtr<IValidationResultControl> m_spValidationResultControl; // Not in use right now, need to fix issue with initialization

public:

	STDMETHOD(EA_Connect(IDualRepository* pRepo, BSTR* pRetVal));
	STDMETHOD(EA_GetMenuItems(IDualRepository* pRepo, BSTR bstrMenuLocation, BSTR bstrMenuName, VARIANT* pRetVal));
	STDMETHOD(EA_GetMenuState(IDualRepository* pRepo, BSTR bstrMenuLocation, BSTR bstrMenuName, BSTR bstrItemName, VARIANT_BOOL* pIsEnabled, VARIANT_BOOL* pIsChecked));
	STDMETHOD(EA_MenuClick(IDualRepository* pRepo, BSTR bstrMenuLocation, BSTR bstrMenuName, BSTR bstrItemName));
	STDMETHOD(EA_OnOutputItemClicked(IDualRepository* pRepo, BSTR bstrTabName, BSTR bstrLineText, int ID));
	STDMETHOD(EA_OnOutputItemDoubleClicked(IDualRepository* pRepo, BSTR bstrTabName, BSTR bstrLineText, int ID));
	HRESULT GetDiagramToValidate(IDualRepository* pRepo, ATL::CComPtr<IDualDiagram>& spDiagram);
	HRESULT GetModelPackage(IDualRepository* pRepo, ATL::CComPtr<IDualPackage>& spPackage);
	HRESULT GetConnectorIdInModelScope(IDispatch* pobjConnector, long& lConnectorID);
	HRESULT GetElementIdInModelScope(IDispatch* pobjElement, long& lElementID);
	HRESULT GetConnectorCollectionInModelScope(const std::function<HRESULT(std::vector<IDispatch*, com_aware_allocator<IDispatch*>>&)>& getElementCollection, std::vector<IDispatch*, com_aware_allocator<IDispatch*>>& vobjCollection);
	HRESULT GetElementCollectionInModelScope(IDualPackage* pRootPackage, std::vector<IDispatch*, com_aware_allocator<IDispatch*>>& vobjCollection);
	HRESULT GetAllPackages(IDualPackage* pPackage, std::vector<IDualPackage*, com_aware_allocator<IDualPackage*>>& vobjPackages);
	HRESULT GetConnectorIdInDiagramScope(IDispatch* pobjDiagram, long& lConnectorID);
	HRESULT GetElementIdInDiagramScope(IDispatch* pobjDiagram, long& lElementID);
	HRESULT GetConnectorCollectionInDiagramScope(const ATL::CComPtr<IDualDiagram>& spDiagram, std::vector<IDispatch*, com_aware_allocator<IDispatch*>>& vobjCollection);
	HRESULT GetElementCollectionInDiagramScope(const ATL::CComPtr<IDualDiagram>& spDiagram, std::vector<IDispatch*, com_aware_allocator<IDispatch*>>& vobjCollection);
	HRESULT FillAssociationProperty(web::json::value& prop, std::wstring id, IDualConnector* pConnector, std::function<HRESULT(IDualConnector*, IDualConnectorEnd**)> endSelector, std::function<HRESULT(IDualConnector*, LONG*)> idSelector);
	HRESULT FillGeneralizationProperty(web::json::value& prop, IDualConnector* pConnector, std::function<HRESULT(IDualConnector*, LONG*)> idSelector);
	HRESULT CreateOntoUMLTab(IDualRepository* pRepo);
	HRESULT DeselectEverything(CComPtr<IDualDiagram>& spDiagram);
	HRESULT DeselectEverything(IDualRepository *pRepo);
	HRESULT SelectObject(IDualRepository *pRepo, CComPtr<IDualDiagram>& spDiagram, LONG id);
};

OBJECT_ENTRY_AUTO(__uuidof(OntoUML), COntoUML)
