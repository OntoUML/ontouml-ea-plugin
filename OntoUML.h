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

	template<typename U, typename... Args>
	void construct(U* p, Args&&... args) {
		std::cout << "Constructing element" << std::endl;
		new(p) U(std::forward<Args>(args)...);
		(*p)->AddRef();
	}

	template<typename U>
	void destroy(U* p) noexcept {
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
	public IDispatchImpl<IOntoUML, &IID_IOntoUML, &LIBID_ontoumleaplugintestLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
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
	inline static HRESULT copyToVector(IDualCollection* pCollection, std::vector<T*, com_aware_allocator<T*>>& vobjCollection)
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

			if (SUCCEEDED(hr) && spT)
			{
				vobjCollection.push_back(spT);
			}
		}

		return hr;
	};



public:

	STDMETHOD(EA_Connect(IDualRepository* pRepo, BSTR* pRetVal));
	STDMETHOD(EA_GetMenuItems(IDualRepository* pRepo, BSTR bstrMenuLocation, BSTR bstrMenuName, VARIANT* pRetVal));
	STDMETHOD(EA_GetMenuState(IDualRepository* pRepo, BSTR bstrMenuLocation, BSTR bstrMenuName, BSTR bstrItemName, VARIANT_BOOL* pIsEnabled, VARIANT_BOOL* pIsChecked));
	STDMETHOD(EA_MenuClick(IDualRepository* pRepo, BSTR bstrMenuLocation, BSTR bstrMenuName, BSTR bstrItemName));
};

OBJECT_ENTRY_AUTO(__uuidof(OntoUML), COntoUML)
