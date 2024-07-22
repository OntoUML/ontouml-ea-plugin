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

STDMETHODIMP COntoUML::EA_MenuClick(IDualRepository* pRepo, BSTR bstrMenuLocation, BSTR bstrMenuName, BSTR bstrItemName)
{
	std::vector<long> vElementIDs;
	std::vector<long> vConnectorIDs;
	std::function<HRESULT(std::vector<IDispatch *, com_aware_allocator<IDispatch*>>&)> getElementCollection;
	std::function<HRESULT(std::vector<IDispatch *, com_aware_allocator<IDispatch*>>&)> getConnectorCollection;
	std::function<HRESULT(IDualPackage* pPackage, std::vector<IDualPackage*, com_aware_allocator<IDualPackage*>>& vobjPackages)> getAllPackages;
	std::function<HRESULT(IDispatch*, long&)> getElementID;
	std::function<HRESULT(IDispatch*, long&)> getConnectorID;

	auto getIDsFromCollection = [](std::function<HRESULT(std::vector<IDispatch *, com_aware_allocator<IDispatch *>>&)> getCollection, std::function<HRESULT(IDispatch*, long&)> convertToID, std::vector<long>& vIds) -> HRESULT
		{
			if (!getCollection || !convertToID)
			{
				return E_INVALIDARG;
			}

			com_aware_allocator<IDispatch*> saDispatch;
			std::vector<IDispatch *, com_aware_allocator<IDispatch *>> vobjCollection(saDispatch);
			auto hr = getCollection(vobjCollection);

			for (auto i = vobjCollection.begin(); SUCCEEDED(hr) && i != vobjCollection.end(); i++)
			{
				CComPtr<IDispatch> spObject = *i;

				long lID = 0;
				if (SUCCEEDED(hr))
				{
					hr = convertToID(spObject, lID);
				}

				if (SUCCEEDED(hr))
				{
					vIds.push_back(lID);
				}
			}

			return hr;
		};

	if (!pRepo)
	{
		return E_INVALIDARG;
	}

	auto hr = S_OK;

	if (0 == wcscmp(bstrItemName, MI_VALIDATE_MODEL))
	{
		// Validate the model
		CComPtr<IDispatch> spObject;
		ObjectType ot;
		if (FAILED(hr = pRepo->GetContextItemType(&ot)))
		{
			return hr;
		}

		if (ObjectType::otNone != ot)
		{
			if (FAILED(hr = pRepo->GetContextObject(&spObject)))
			{
				return hr;
			}
		}
		else
		{
			if (FAILED(hr = pRepo->GetTreeSelectedObject(&spObject)))
			{
				return hr;
			}

			if (FAILED(hr = pRepo->GetTreeSelectedItemType(&ot)))
			{
				return hr;
			}
		}

		if (!spObject)
		{
			return S_FALSE;
		}

		long lPackageID = 0;
		long lParentID = 0;
		CComPtr<IDualPackage> spPackage;
		if (ObjectType::otDiagram == ot)
		{
			CComPtr<IDualDiagram> spDiagram;
			if (FAILED(hr = spObject->QueryInterface(&spDiagram)))
			{
				return hr;
			}

			if (FAILED(hr = spDiagram->get_PackageID(&lPackageID)))
			{
				return hr;
			}

			if (FAILED(hr = pRepo->GetPackageByID(lPackageID, &spPackage)))
			{
				return hr;
			}
		}
		else
		{
			if (FAILED(hr = spObject->QueryInterface(&spPackage)))
			{
				return hr;
			}
		}

		if (FAILED(hr = spPackage->get_ParentID(&lParentID)))
		{
			return hr;
		}

		while (lParentID > 0)
		{
			lPackageID = lParentID;
			spPackage.Release();

			if (FAILED(hr = pRepo->GetPackageByID(lPackageID, &spPackage)))
			{
				return hr;
			}

			if (FAILED(hr = spPackage->get_ParentID(&lParentID)))
			{
				return hr;
			}
		}

		if (!spPackage)
		{
			return S_FALSE;
		}
		IDualPackage* pRootPackage = spPackage.Detach();

		getAllPackages = [&getAllPackages](IDualPackage *pPackage, std::vector<IDualPackage *, com_aware_allocator<IDualPackage *>>& vobjPackages)->HRESULT
			{
				if (!pPackage)
				{
					return S_FALSE;
				}

				vobjPackages.push_back(pPackage);

				CComPtr<IDualCollection> spCollection;
				auto hr = pPackage->get_Packages(&spCollection);
				short sCount = 0;
				if (SUCCEEDED(hr))
				{
					hr = spCollection->get_Count(&sCount);
				}

				for (short i = 0; SUCCEEDED(hr) && i < sCount; i++)
				{
					CComPtr<IDispatch> spObject;
					hr = spCollection->GetAt(i, &spObject);

					CComQIPtr<IDualPackage> spPkg = spObject;
					if (SUCCEEDED(hr) && spPkg)
					{
						hr = getAllPackages(spPkg, vobjPackages);
					}
				}

				return hr;
			};

		getElementCollection = [&pRootPackage, &getAllPackages](std::vector<IDispatch *, com_aware_allocator<IDispatch *>>& vobjCollection) -> HRESULT
			{
				com_aware_allocator<IDualPackage*> saPackage;
				std::vector<IDualPackage *, com_aware_allocator<IDualPackage *>> vobjPackages(saPackage);
				auto hr = getAllPackages(pRootPackage, vobjPackages);

				for (auto i = vobjPackages.begin(); SUCCEEDED(hr) && i != vobjPackages.end(); i++)
				{
					CComPtr<IDualCollection> spCollection;
					hr = (*i)->get_Elements(&spCollection);

					if (SUCCEEDED(hr))
					{
						hr = copyToVector(spCollection, vobjCollection);
					}
				}

				return hr;
			};

		getConnectorCollection = [&pRootPackage, &getAllPackages](std::vector<IDispatch *, com_aware_allocator<IDispatch *>>& vobjCollection) -> HRESULT
			{
				com_aware_allocator<IDualPackage*> saPackage;
				std::vector<IDualPackage*, com_aware_allocator<IDualPackage*>> vobjPackages(saPackage);
				auto hr = getAllPackages(pRootPackage, vobjPackages);

				for (auto i = vobjPackages.begin(); SUCCEEDED(hr) && i != vobjPackages.end(); i++)
				{
					CComPtr<IDualPackage> spPkg = *i;
					CComPtr<IDualCollection> spCollection;
					hr = spPkg->get_Connectors(&spCollection);

					if (SUCCEEDED(hr))
					{
						hr = copyToVector(spCollection, vobjCollection);
					}
				}

				return hr;
			};

		getElementID = [](IDispatch* pobjElement, long& lElementID) -> HRESULT
			{
				CComPtr<IDualElement> spElement;
				auto hr = pobjElement->QueryInterface(&spElement);

				if (SUCCEEDED(hr))
				{
					hr = spElement->get_ElementID(&lElementID);
				}

				return hr;
			};

		getConnectorID = [](IDispatch* pobjConnector, long& lConnectorID) -> HRESULT
			{
				CComPtr<IDualConnector> spConnector;
				auto hr = pobjConnector->QueryInterface(&spConnector);

				if (SUCCEEDED(hr))
				{
					hr = spConnector->get_ConnectorID(&lConnectorID);
				}

				return hr;
			};
	}
	else if (0 == wcscmp(bstrItemName, MI_VALIDATE_DIAGRAM))
	{
		// Validate the diagram
		CComPtr<IDualDiagram> spDiagram;
		if (FAILED(hr = pRepo->GetCurrentDiagram(&spDiagram)))
		{
			return hr;
		}

		// If the diagram is not open, it is either the current context item, or selected in the tree
		if (!spDiagram)
		{
			ObjectType ot;
			if (FAILED(hr = pRepo->GetContextItemType(&ot)))
			{
				return hr;
			}

			CComPtr<IDispatch> spDispDiagram;
			if (ObjectType::otDiagram == ot)
			{
				if (FAILED(hr = pRepo->GetContextObject(&spDispDiagram)))
				{
					return hr;
				}
			}
			else
			{
				if (FAILED(hr = pRepo->GetTreeSelectedObject(&spDispDiagram)))
				{
					return hr;
				}
			}

			if (FAILED(hr = spDispDiagram->QueryInterface(&spDiagram)))
			{
				return hr;
			}
		}

		getElementCollection = [&spDiagram](std::vector<IDispatch *, com_aware_allocator<IDispatch *>>& vobjCollection) -> HRESULT
			{
				CComPtr<IDualCollection> spCollection;
				auto hr = spDiagram->get_DiagramObjects(&spCollection);

				if (SUCCEEDED(hr))
				{
					hr = copyToVector(spCollection, vobjCollection);
				}

				return hr;
			};

		getConnectorCollection = [&spDiagram](std::vector<IDispatch *, com_aware_allocator<IDispatch *>>& vobjCollection) -> HRESULT
			{
				CComPtr<IDualCollection> spCollection;
				auto hr = spDiagram->get_DiagramLinks(&spCollection);

				if (SUCCEEDED(hr))
				{
					hr = copyToVector(spCollection, vobjCollection);
				}

				return hr;
			};

		getElementID = [](IDispatch* pobjDiagram, long& lElementID) -> HRESULT
			{
				CComPtr<IDualDiagramObject> spDiagramObject;
				auto hr = pobjDiagram->QueryInterface(&spDiagramObject);

				if (SUCCEEDED(hr))
				{
					hr = spDiagramObject->get_ElementID(&lElementID);
				}

				return hr;
			};

		getConnectorID = [](IDispatch* pobjDiagram, long& lConnectorID) -> long
			{
				CComPtr<IDualDiagramLink> spDiagramObject;
				auto hr = pobjDiagram->QueryInterface(&spDiagramObject);

				if (SUCCEEDED(hr))
				{
					hr = spDiagramObject->get_ConnectorID(&lConnectorID);
				}

				return hr;
			};
	}

	if (SUCCEEDED(hr))
	{
		hr = getIDsFromCollection(getElementCollection, getElementID, vElementIDs);
	}

	if (SUCCEEDED(hr))
	{
		hr = getIDsFromCollection(getConnectorCollection, getConnectorID, vConnectorIDs);
	}

	//////////
	// Todo: Validate the model or diagram using the ids in vElementIDs and vConnectorIDs
	//////////

	return S_OK;
}