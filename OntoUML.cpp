// OntoUML.cpp : Implementation of COntoUML

#include "pch.h"

#include "OntoUML.h"
#include <cpprest/json.h>
#include <cpprest/http_client.h>

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

			std::sort(vIds.begin(), vIds.end());
			vIds.erase(std::unique(vIds.begin(), vIds.end()), vIds.end());

			return hr;
		};

	if (!pRepo)
	{
		return E_INVALIDARG;
	}

	auto hr = S_OK;

	if (0 == wcscmp(bstrItemName, MI_VALIDATE_MODEL))
	{
		//////////
		// Validate the model
		//////////

		// First locate the root package (which contains the whole model)
		CComPtr<IDualPackage> spPackage;
		hr = GetModelPackage(pRepo, spPackage);
		if (FAILED(hr) || !spPackage)
		{
			return S_FALSE;
		}

		IDualPackage* pRootPackage = spPackage.Detach();

		// Then set all lambdas to point at the correct behavior for the model scope
		getAllPackages = [this, &getAllPackages](IDualPackage *pPackage, std::vector<IDualPackage *, com_aware_allocator<IDualPackage *>>& vobjPackages)->HRESULT
			{
				return GetAllPackages(pPackage, vobjPackages);
			};

		getElementCollection = [this, &pRootPackage, &getAllPackages](std::vector<IDispatch *, com_aware_allocator<IDispatch *>>& vobjCollection) -> HRESULT
			{
				return GetElementCollectionInModelScope(pRootPackage, vobjCollection);
			};

		getConnectorCollection = [this, &getElementCollection](std::vector<IDispatch *, com_aware_allocator<IDispatch *>>& vobjCollection) -> HRESULT
			{
				return GetConnectorCollectionInModelScope(getElementCollection, vobjCollection);
			};

		getElementID = [this](IDispatch* pobjElement, long& lElementID) -> HRESULT
			{
				return GetElementIdInModelScope(pobjElement, lElementID);
			};

		getConnectorID = [this](IDispatch* pobjConnector, long& lConnectorID) -> HRESULT
			{
				return GetConnectorIdInModelScope(pobjConnector, lConnectorID);
			};
	}
	else if (0 == wcscmp(bstrItemName, MI_VALIDATE_DIAGRAM))
	{
		//////////
		// Validate the diagram
		//////////

		// First get the diagram to validate
		CComPtr<IDualDiagram> spDiagram;
		hr = GetDiagramToValidate(pRepo, spDiagram);
		if (FAILED(hr) || !spDiagram)
		{
			return S_FALSE;
		}

		// Then set all lambdas to point at the correct behavior for the diagram scope
		getElementCollection = [this, &spDiagram](std::vector<IDispatch *, com_aware_allocator<IDispatch *>>& vobjCollection) -> HRESULT
			{
				return GetElementCollectionInDiagramScope(spDiagram, vobjCollection);
			};

		getConnectorCollection = [this, &spDiagram](std::vector<IDispatch *, com_aware_allocator<IDispatch *>>& vobjCollection) -> HRESULT
			{
				return GetConnectorCollectionInDiagramScope(spDiagram, vobjCollection);
			};

		getElementID = [this](IDispatch* pobjDiagram, long& lElementID) -> HRESULT
			{
				return GetElementIdInDiagramScope(pobjDiagram, lElementID);
			};

		getConnectorID = [this](IDispatch* pobjDiagram, long& lConnectorID) -> long
			{
				return GetConnectIdInDiagramScope(pobjDiagram, lConnectorID);
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

	if (FAILED(hr))
	{
		return hr;
	}
	
	auto project = web::json::value::object();
	project[U("id")] = web::json::value::string(std::to_wstring(42));
	project[U("name")] = web::json::value();
	project[U("description")] = web::json::value();
	project[U("type")] = web::json::value::string(U("Project"));
	auto model = web::json::value::object();
	model[U("id")] = web::json::value::string(std::to_wstring(43));
	model[U("name")] = web::json::value();
	model[U("description")] = web::json::value();
	model[U("type")] = web::json::value::string(U("Package"));
	model[U("propertyAssignments")] = web::json::value();
	auto content = web::json::value::array();
	for (auto elt : vElementIDs)
	{
		CComPtr<IDualElement> spElement;
		if (SUCCEEDED(hr = pRepo->GetElementByID(elt, &spElement)))
		{
			auto element = web::json::value::object();
			CComBSTR bstrName;
			CComBSTR bstrNotes;
			CComBSTR bstrStereotype;
			CComBSTR bstrRestrictedTo(U("functional-complex"));

			if (FAILED(hr = spElement->get_Name(&bstrName)))
			{
				return hr;
			}

			if (FAILED(hr = spElement->get_Notes(&bstrNotes)))
			{
				return hr;
			}

			if (FAILED(hr = spElement->get_Stereotype(&bstrStereotype)))
			{
				return hr;
			}

			CComPtr<IDualCollection> spTaggedValues;
			if (FAILED(hr = spElement->get_TaggedValues(&spTaggedValues)))
			{
				return hr;
			}

			CComBSTR bstrRestrictedToTag(U("restrictedTo"));
			CComQIPtr <IDualTaggedValue> spTaggedValue;
			CComPtr<IDispatch> spDispTaggedValue;
			if (SUCCEEDED(spTaggedValues->GetByName(bstrRestrictedToTag, &spDispTaggedValue)) && spDispTaggedValue)
			{
				spTaggedValue = spDispTaggedValue;
				spTaggedValue->get_Value(&bstrRestrictedTo);
			}

			element[U("id")] = web::json::value::string(std::to_wstring(elt));
			element[U("name")] = web::json::value::string(bstrName.operator wchar_t *());
			element[U("description")] = web::json::value::string(bstrNotes.operator wchar_t *());
			element[U("type")] = web::json::value::string(U("Class"));
			element[U("propertyAssignments")] = web::json::value();
			element[U("stereotype")] = web::json::value::string(bstrStereotype.operator wchar_t *());
			element[U("isAbstract")] = web::json::value::boolean(false);
			element[U("isDerived")] = web::json::value::boolean(false);
			element[U("properties")] = web::json::value();
			element[U("isExtensional")] = web::json::value::boolean(false);
			element[U("isPowertype")] = web::json::value();
			element[U("literals")] = web::json::value();

			auto restrictedTo = web::json::value::array(1);
			restrictedTo[0] = web::json::value::string(bstrRestrictedTo.operator wchar_t *());
			element[U("restrictedTo")] = restrictedTo;

			content[content.size()] = element;
		}
	}

	model[U("contents")] = content;
	project[U("model")] = model;

	auto payload = web::json::value::object();
	payload[U("options")] = web::json::value();
	payload[U("project")] = project;

	auto tst = payload.serialize();

	web::http::client::http_client client(U("http://api.ontouml.org/v1/verify"));
	web::http::http_request request(web::http::methods::POST);
	request.headers().set_content_type(U("application/json"));
	request.set_body(payload);

	client.request(request).then([](web::http::http_response response)
		{
			if (response.status_code() == web::http::status_codes::OK)
			{
				auto body = response.extract_json().get();
				auto tst_body = body.serialize();
			}
		}).wait();

	//////////
	// Todo: Validate the model or diagram using the ids in vElementIDs and vConnectorIDs
	//////////
	//quicktype::OntoumlSchema project;
	//project.set_type(quicktype::OntoUmlElementType::PROJECT);
	//project.set_id(std::to_string(42)); // use id for project desc
	//auto elements = std::vector<quicktype::Element>();
	//
	//for (auto elt : vElementIDs)
	//{
	//	CComPtr<IDualElement> spElement;
	//	if (SUCCEEDED(hr = pRepo->GetElementByID(elt, &spElement)))
	//	{
	//		quicktype::Element element;
	//		element.set_type(quicktype::ElementType::CLASS);


	//		elements.push_back(element);
	//	}
	//}
	//project.set_elements(std::optional(elements));

	//quicktype::json json;
	//quicktype::to_json(json, project);
	//auto dump = json.dump();

	return S_OK;
}

HRESULT COntoUML::GetDiagramToValidate(IDualRepository* pRepo, ATL::CComPtr<IDualDiagram>& spDiagram)
{
	auto hr = S_OK;
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

	return hr;
}

HRESULT COntoUML::GetModelPackage(IDualRepository* pRepo, ATL::CComPtr<IDualPackage>& spPackage)
{
	CComPtr<IDispatch> spObject;
	ObjectType ot;
	auto hr = S_OK;

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

	return hr;
}

HRESULT COntoUML::GetConnectorIdInModelScope(IDispatch* pobjConnector, long& lConnectorID)
{
	CComPtr<IDualConnector> spConnector;
	auto hr = pobjConnector->QueryInterface(&spConnector);

	if (SUCCEEDED(hr))
	{
		hr = spConnector->get_ConnectorID(&lConnectorID);
	}

	return hr;
}

HRESULT COntoUML::GetElementIdInModelScope(IDispatch* pobjElement, long& lElementID)
{
	CComPtr<IDualElement> spElement;
	auto hr = pobjElement->QueryInterface(&spElement);

	if (SUCCEEDED(hr))
	{
		hr = spElement->get_ElementID(&lElementID);
	}

	return hr;
}

HRESULT COntoUML::GetConnectorCollectionInModelScope(const std::function<HRESULT(std::vector<IDispatch*, com_aware_allocator<IDispatch*>>&)>& getElementCollection, std::vector<IDispatch*, com_aware_allocator<IDispatch*>>& vobjCollection)
{
	com_aware_allocator<IDispatch*> saElement;
	std::vector<IDispatch*, com_aware_allocator<IDispatch*>> vobjElements(saElement);
	auto hr = S_OK;

	if (FAILED(hr = getElementCollection(vobjElements)))
	{
		return hr;
	}

	for (auto pElt : vobjElements)
	{
		CComPtr<IDualElement> spElement;
		if (SUCCEEDED(hr = pElt->QueryInterface(&spElement)))
		{
			CComPtr<IDualCollection> spCollection;
			hr = spElement->get_Connectors(&spCollection);

			if (SUCCEEDED(hr))
			{
				hr = copyToVector(spCollection, vobjCollection);
			}
		}
	}

	return hr;
}

HRESULT COntoUML::GetElementCollectionInModelScope(IDualPackage* pRootPackage, std::vector<IDispatch*, com_aware_allocator<IDispatch*>>& vobjCollection)
{
	{
		com_aware_allocator<IDualPackage*> saPackage;
		std::vector<IDualPackage*, com_aware_allocator<IDualPackage*>> vobjPackages(saPackage);
		auto hr = GetAllPackages(pRootPackage, vobjPackages);

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
	}
}

HRESULT COntoUML::GetAllPackages(IDualPackage* pPackage, std::vector<IDualPackage*, com_aware_allocator<IDualPackage*>>& vobjPackages)
{
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
				hr = GetAllPackages(spPkg, vobjPackages);
			}
		}

		return hr;
	}
}

HRESULT COntoUML::GetConnectIdInDiagramScope(IDispatch* pobjDiagram, long& lConnectorID)
{
	CComPtr<IDualDiagramLink> spDiagramObject;
	auto hr = pobjDiagram->QueryInterface(&spDiagramObject);

	if (SUCCEEDED(hr))
	{
		hr = spDiagramObject->get_ConnectorID(&lConnectorID);
	}

	return hr;
}

HRESULT COntoUML::GetElementIdInDiagramScope(IDispatch* pobjDiagram, long& lElementID)
{
	CComPtr<IDualDiagramObject> spDiagramObject;
	auto hr = pobjDiagram->QueryInterface(&spDiagramObject);

	if (SUCCEEDED(hr))
	{
		hr = spDiagramObject->get_ElementID(&lElementID);
	}

	return hr;
}

HRESULT COntoUML::GetConnectorCollectionInDiagramScope(const ATL::CComPtr<IDualDiagram>& spDiagram, std::vector<IDispatch*, com_aware_allocator<IDispatch*>>& vobjCollection)
{
	CComPtr<IDualCollection> spCollection;
	auto hr = spDiagram->get_DiagramLinks(&spCollection);

	if (SUCCEEDED(hr))
	{
		hr = copyToVector(spCollection, vobjCollection);
	}

	return hr;
}

HRESULT COntoUML::GetElementCollectionInDiagramScope(const ATL::CComPtr<IDualDiagram>& spDiagram, std::vector<IDispatch*, com_aware_allocator<IDispatch*>>& vobjCollection)
{
	CComPtr<IDualCollection> spCollection;
	auto hr = spDiagram->get_DiagramObjects(&spCollection);

	if (SUCCEEDED(hr))
	{
		hr = copyToVector(spCollection, vobjCollection);
	}

	return hr;
}
