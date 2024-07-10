// OntoUML.cpp : Implementation of COntoUML

#include "pch.h"

#include "OntoUML.h"


// COntoUML
STDMETHODIMP COntoUML::EA_Connect(IDualRepository* pRepo, BSTR* pRetVal)
{
	if (!pRetVal)
	{
		return E_POINTER;
	}

	// Normal AddIn (not workflow or MDG)
	auto hr = CComBSTR(L"").CopyTo(pRetVal);

	if (!pRepo)
	{
		return E_INVALIDARG;
	}

	this->m_spRepo = pRepo; // Assignment override increases refcount for us

	return hr;
}

STDMETHODIMP COntoUML::EA_GetMenuItems(IDualRepository* pRepo, BSTR bstrMenuLocation, BSTR bstrMenuName, VARIANT* pRetVal)
{
	if (!pRetVal)
	{
		return E_POINTER;
	}

	if (!pRepo)
	{
		return E_INVALIDARG;
	}

	auto hr = S_OK;
	if (0 == wcscmp(bstrMenuName, L""))
	{
		hr = CComBSTR(MI_ONTOUML).CopyTo(pRetVal);
	}
	else if (0 == wcscmp(bstrMenuName, MI_ONTOUML))
	{
		CComSafeArray<BSTR> rgsfaItems;
		BSTR rgbstrItems[] = { CComBSTR(MI_VALIDATE_MODEL).Detach(), CComBSTR(MI_VALIDATE_DIAGRAM).Detach() };
		auto hr = rgsfaItems.Add(2, rgbstrItems, 0);

		if (SUCCEEDED(hr))
		{
			hr = CComVariant(rgsfaItems).Detach(pRetVal);
		}
	}

	return hr;
}

STDMETHODIMP COntoUML::EA_GetMenuState(IDualRepository* pRepo, BSTR bstrMenuLocation, BSTR bstrMenuName, BSTR bstrItemName, VARIANT_BOOL* pIsEnabled, VARIANT_BOOL* pIsChecked)
{
	if (!pIsChecked || !pIsEnabled)
	{
		return E_POINTER;
	}

	*pIsChecked = VARIANT_FALSE;
	*pIsEnabled = VARIANT_FALSE;

	if (!pRepo)
	{
		return E_INVALIDARG;
	}

	ObjectType ot;
	auto hr = pRepo->GetContextItemType(&ot);

	if (SUCCEEDED(hr) && ObjectType::otNone == ot)
	{
		hr = pRepo->GetTreeSelectedItemType(&ot);
	}

	if (FAILED(hr))
	{
		return hr;
	}

	switch (ot)
	{
	case ObjectType::otDiagram:

		*pIsEnabled = VARIANT_TRUE;
		break;

	case ObjectType::otModel:
	case ObjectType::otPackage:

		if (0 == wcscmp(bstrItemName, MI_VALIDATE_MODEL))
		{
			*pIsEnabled = VARIANT_TRUE;
		}
		break;
	}

	return hr;
}
