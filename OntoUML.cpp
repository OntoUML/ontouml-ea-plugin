// OntoUML.cpp : Implementation of COntoUML

#include "pch.h"

#include "OntoUML.h"

std::map<std::wstring, std::vector<std::wstring>> COntoUML::s_defaultRestrictedToValues =
{
	{ U("kind"), {U("functional-complex")} },
	{ U("collective"), {U("collective")} },
	{ U("quantity"), {U("quantity")} },
	{ U("relator"), {U("relator")} },
	{ U("quality"), {U("quality")} },
	{ U("mode"), {U("intrinsic-mode")} },
	{ U("event"), {U("event")} },
	{ U("situation"), {U("situation")} },
	{ U("abstract"), {U("abstract")} },
	{ U("datatype"), {U("abstract")} },
	{ U("enumeration"), {U("abstract")} },
	{ U("type"), {U("type")} },
	{ U("subkind"), {U("functional-complex")} },
	{ U("role"), {U("functional-complex")} },
	{ U("roleMixin"), {U("functional-complex")} },
	{ U("phase"), {U("functional-complex")} },
	{ U("phaseMixin"), {U("functional-complex")} },
	{ U("category"), {U("functional-complex")} },
	{ U("mixin"), {U("functional-complex")} },
	{ U("historicalRole"), {U("functional-complex")} },
	{ U("historicalRoleMixin"), {U("functional-complex")} },
};

// COntoUML
STDMETHODIMP COntoUML::EA_Connect(IDualRepository* pRepo, BSTR* pRetVal)
{
	if (!pRetVal)
	{
		return E_POINTER;
	}

	// Normal AddIn (not workflow or MDG)
	auto hr = CComBSTR(L"").CopyTo(pRetVal);
	if (FAILED(hr))
	{
		return hr;
	}

	if (!pRepo)
	{
		return E_INVALIDARG;
	}
	CreateOntoUMLTab(pRepo);

	//////////
	// Todo: fix custom UI. Probably move out of EA_Connect into one of the other events
	//////////
	//IUnknown *pControl = 0;
	//CComBSTR bstrTitle = L"OntoUML";
	//CComBSTR bstrProgId = L"ontouml-ea-plugin.validation-result";
	//hr = pRepo->AddTab(bstrTitle, bstrProgId, &pControl);

	//if (SUCCEEDED(hr))
	//{
	//	hr = pControl->QueryInterface(&m_spValidationResultControl);
	//	pControl->Release();
	//}

	return hr;
}

STDMETHODIMP COntoUML::EA_OnOutputItemClicked(IDualRepository* pRepo, BSTR bstrTabName, BSTR bstrLineText, int ID)
{
	if (!pRepo)
	{
		return E_INVALIDARG;
	}

	if (wcscmp(TAB_ONTOUML, bstrTabName))
	{
		return S_FALSE;
	}

	ID = m_outputIdToElementId[ID];

	// If there's a value in m_spCurrentDiagram, use that
	if (m_spCurrentDiagram)
	{
		DeselectEverything(pRepo);

		return SelectObject(pRepo, m_spCurrentDiagram, ID);
	}
	else
	{
		// Otherwise, search the whole model
		CComPtr<IDualPackage> spRoot;
		auto hr = GetModelPackage(pRepo, spRoot);
		if (FAILED(hr))
		{
			return hr;
		}

		std::vector<IDualPackage*, com_aware_allocator<IDualPackage*>> vobjPackages;
		if (FAILED(hr = GetAllPackages(spRoot, vobjPackages)))
		{
			return hr;
		}

		for (auto pPackage : vobjPackages)
		{
			CComPtr<IDualCollection> spDiagrams;
			if (FAILED(hr = pPackage->get_Diagrams(&spDiagrams)))
			{
				return hr;
			}

			short sCount = 0;
			if (FAILED(hr = spDiagrams->get_Count(&sCount)))
			{
				return hr;
			}

			VARIANT_BOOL found = VARIANT_FALSE;
			for (short i = 0; i < sCount && !found; i++)
			{
				CComPtr<IDispatch> spDispDiagram;
				if (FAILED(hr = spDiagrams->GetAt(i, &spDispDiagram)))
				{
					return hr;
				}

				CComPtr<IDualDiagram> spDiagram;
				if (FAILED(hr = spDispDiagram->QueryInterface(&spDiagram)))
				{
					return hr;
				}

				if (FAILED(hr = spDiagram->FindElementInDiagram(ID, &found)))
				{
					return hr;
				}

				if (found)
				{
					// Weird, but I see no alternative. To make sure we're not selected multiple things
					// we deselected everything first and then select again
					hr = DeselectEverything(spDiagram);
				}

				if (SUCCEEDED(hr))
				{
					hr = SelectObject(pRepo, spDiagram, ID);
				}
			}
		}

		return hr;
	}
}

HRESULT COntoUML::SelectObject(IDualRepository *pRepo, CComPtr<IDualDiagram>& spDiagram, LONG lID)
{
	VARIANT_BOOL bFound = VARIANT_FALSE;
	auto hr = spDiagram->FindElementInDiagram(lID, &bFound);

	if (!bFound)
	{
		CComPtr<IDualConnector> spConnector;
		hr = pRepo->GetConnectorByID(lID, &spConnector);

		if (SUCCEEDED(hr))
		{
			hr = spDiagram->put_SelectedConnector(spConnector);
		}
	}

	return hr;
}

HRESULT COntoUML::DeselectEverything(IDualRepository* pRepo)
{
	CComPtr<IDualEASelection> spSelection;
	auto hr = pRepo->get_CurrentSelection(&spSelection);

	CComPtr<IDualCollection> spSelectedElements;
	if (SUCCEEDED(hr))
	{
		hr = spSelection->get_ElementSet(&spSelectedElements);
	}

	short sCount = 0;
	if (SUCCEEDED(hr))
	{
		hr = spSelectedElements->get_Count(&sCount);
	}

	for (short i = 0; SUCCEEDED(hr) && i < sCount; i++)
	{
		hr = spSelectedElements->Delete(i);
	}

	return hr;
}

HRESULT COntoUML::DeselectEverything(CComPtr<IDualDiagram>& spDiagram)
{
	CComPtr<IDualCollection> spSelectedObjects;
	auto hr = spDiagram->get_SelectedObjects(&spSelectedObjects);
	short sCount = 0;
	if (SUCCEEDED(hr))
	{
		hr = spSelectedObjects->get_Count(&sCount);
	}

	for (short i = 0; SUCCEEDED(hr) && i < sCount; i++)
	{
		hr = spSelectedObjects->Delete(i);
	}

	return hr;
}

STDMETHODIMP COntoUML::EA_OnOutputItemDoubleClicked(IDualRepository* pRepo, BSTR bstrTabName, BSTR bstrLineText, int ID)
{
	if (!pRepo)
	{
		return E_INVALIDARG;
	}

	if (wcscmp(TAB_ONTOUML, bstrTabName))
	{
		return S_FALSE;
	}

	auto result = m_validationResultsPerOutputId[ID];

	auto stringized = result.serialize();

	MessageBox(NULL, stringized.c_str(), L"Validation result", MB_ICONEXCLAMATION);

	return S_OK;
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
	std::function<HRESULT(std::vector<IDispatch*, com_aware_allocator<IDispatch*>>&)> getElementCollection;
	std::function<HRESULT(std::vector<IDispatch*, com_aware_allocator<IDispatch*>>&)> getConnectorCollection;
	std::function<HRESULT(IDualPackage* pPackage, std::vector<IDualPackage*, com_aware_allocator<IDualPackage*>>& vobjPackages)> getAllPackages;
	std::function<HRESULT(IDispatch*, long&)> getElementID;
	std::function<HRESULT(IDispatch*, long&)> getConnectorID;

	auto getIDsFromCollection = [](std::function<HRESULT(std::vector<IDispatch*, com_aware_allocator<IDispatch*>>&)> getCollection, std::function<HRESULT(IDispatch*, long&)> convertToID, std::vector<long>& vIds) -> HRESULT
		{
			if (!getCollection || !convertToID)
			{
				return E_INVALIDARG;
			}

			com_aware_allocator<IDispatch*> saDispatch;
			std::vector<IDispatch*, com_aware_allocator<IDispatch*>> vobjCollection(saDispatch);
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

	auto hr = CreateOntoUMLTab(pRepo);

	m_outputIdToElementId.clear();
	m_validationResultsPerOutputId.clear();

	if (m_spCurrentDiagram)
	{
		m_spCurrentDiagram.Release();
	}

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
		getAllPackages = [this, &getAllPackages](IDualPackage* pPackage, std::vector<IDualPackage*, com_aware_allocator<IDualPackage*>>& vobjPackages)->HRESULT
			{
				return GetAllPackages(pPackage, vobjPackages);
			};

		getElementCollection = [this, &pRootPackage, &getAllPackages](std::vector<IDispatch*, com_aware_allocator<IDispatch*>>& vobjCollection) -> HRESULT
			{
				return GetElementCollectionInModelScope(pRootPackage, vobjCollection);
			};

		getConnectorCollection = [this, &getElementCollection](std::vector<IDispatch*, com_aware_allocator<IDispatch*>>& vobjCollection) -> HRESULT
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
		hr = GetDiagramToValidate(pRepo, m_spCurrentDiagram);
		if (FAILED(hr) || !m_spCurrentDiagram)
		{
			return S_FALSE;
		}

		// Then set all lambdas to point at the correct behavior for the diagram scope
		getElementCollection = [this](std::vector<IDispatch*, com_aware_allocator<IDispatch*>>& vobjCollection) -> HRESULT
			{
				return GetElementCollectionInDiagramScope(m_spCurrentDiagram, vobjCollection);
			};

		getConnectorCollection = [this](std::vector<IDispatch*, com_aware_allocator<IDispatch*>>& vobjCollection) -> HRESULT
			{
				return GetConnectorCollectionInDiagramScope(m_spCurrentDiagram, vobjCollection);
			};

		getElementID = [this](IDispatch* pobjDiagram, long& lElementID) -> HRESULT
			{
				return GetElementIdInDiagramScope(pobjDiagram, lElementID);
			};

		getConnectorID = [this](IDispatch* pobjDiagram, long& lConnectorID) -> long
			{
				return GetConnectorIdInDiagramScope(pobjDiagram, lConnectorID);
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

	// Fixup a map to connect all element ids to the all diagrams they appear in

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
			CComBSTR bstrType;
			if (FAILED(hr = spElement->get_Type(&bstrType)))
			{
				return hr;
			}

			if (wcscmp(U("Class"), bstrType))
			{
				continue;
			}

			auto element = web::json::value::object();
			CComBSTR bstrName;
			CComBSTR bstrNotes;
			CComBSTR bstrStereotype;

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

			CComQIPtr <IDualTaggedValue> spTaggedValue;
			CComPtr<IDispatch> spDispTaggedValue;
			std::vector<IDualTaggedValue*, com_aware_allocator<IDualTaggedValue*>> vTaggedValues;
			std::function<bool(IDualTaggedValue* p)> filter = [](IDualTaggedValue* p) -> bool
				{
					CComBSTR bstrName;
					p->get_Name(&bstrName);
					return !wcscmp(bstrName, U("restrictedTo"));
				};
			copyToVector(spTaggedValues, vTaggedValues, filter); // vTaggedValues contains only the restrictedTo tagged values

			std::vector<std::wstring> vRestrictedTo;
			if (vTaggedValues.size() > 0)
			{
				std::transform(vTaggedValues.begin(), vTaggedValues.end(), std::back_inserter(vRestrictedTo), [](IDualTaggedValue* pTag) -> std::wstring
					{
						CComBSTR bstrValue;
						pTag->get_Value(&bstrValue);
						return bstrValue.operator wchar_t* ();
					});
			}
			else
			{
				vRestrictedTo = s_defaultRestrictedToValues[bstrStereotype.operator wchar_t* ()];

				// Todo: update element to include restrictedTo tagged values
			}

			auto restrictedTo = web::json::value::array();
			for (auto pwszRestrictedTo : vRestrictedTo)
			{
				restrictedTo[restrictedTo.size()] = web::json::value::string(pwszRestrictedTo);
			}

			element[U("restrictedTo")] = restrictedTo;
			element[U("id")] = web::json::value::string(std::to_wstring(elt));
			element[U("name")] = web::json::value::string(bstrName.operator wchar_t* ());
			element[U("description")] = web::json::value::string(bstrNotes.operator wchar_t* ());
			element[U("type")] = web::json::value::string(U("Class"));
			element[U("propertyAssignments")] = web::json::value();
			element[U("stereotype")] = web::json::value::string(bstrStereotype.operator wchar_t* ());
			element[U("isAbstract")] = web::json::value::boolean(false);
			element[U("isDerived")] = web::json::value::boolean(false);
			element[U("properties")] = web::json::value();
			element[U("isExtensional")] = web::json::value::boolean(false);
			element[U("isPowertype")] = web::json::value();
			element[U("literals")] = web::json::value();

			content[content.size()] = element;
		}
	}

	for (auto rel : vConnectorIDs)
	{
		CComPtr<IDualConnector> spConnector;
		if (SUCCEEDED(hr = pRepo->GetConnectorByID(rel, &spConnector)))
		{
			CComBSTR bstrType;
			if (FAILED(hr = spConnector->get_Type(&bstrType)))
			{
				return hr;
			}

			// We only care about aggegations, associations and generalizations
			if (wcscmp(U("Aggregation"), bstrType) &&
				wcscmp(U("Association"), bstrType) &&
				wcscmp(U("Generalization"), bstrType))
			{
				continue;
			}

			auto connector = web::json::value::object();
			CComBSTR bstrName;
			CComBSTR bstrNotes;
			CComBSTR bstrStereotype;

			if (FAILED(hr = spConnector->get_Name(&bstrName)))
			{
				return hr;
			}

			if (FAILED(hr = spConnector->get_Notes(&bstrNotes)))
			{
				return hr;
			}

			if (FAILED(hr = spConnector->get_Stereotype(&bstrStereotype)))
			{
				return hr;
			}

			connector[U("id")] = web::json::value::string(std::to_wstring(rel));
			connector[U("name")] = web::json::value::string(bstrName.operator wchar_t* ());
			connector[U("description")] = web::json::value::string(bstrNotes.operator wchar_t* ());
			connector[U("propertyAssignments")] = web::json::value();

			if (!wcscmp(U("Aggregation"), bstrType) ||
				!wcscmp(U("Association"), bstrType))
			{
				connector[U("type")] = web::json::value::string(U("Relation"));
				connector[U("stereotype")] = web::json::value::string(bstrStereotype.operator wchar_t* ());
				connector[U("isAbstract")] = web::json::value::boolean(false);
				connector[U("isDerived")] = web::json::value::boolean(false);

				auto properties = web::json::value::array(2);
				auto client = web::json::value::object();
				auto supplier = web::json::value::object();

				if (FAILED(hr = FillAssociationProperty(client, std::to_wstring(rel) + U("_client"), spConnector, &IDualConnector::get_ClientEnd, &IDualConnector::get_ClientID)))
				{
					return hr;
				}

				if (FAILED(hr = FillAssociationProperty(supplier, std::to_wstring(rel) + U("_supplier"), spConnector, &IDualConnector::get_SupplierEnd, &IDualConnector::get_SupplierID)))
				{
					return hr;
				}

				properties[0] = client;
				properties[1] = supplier;

				connector[U("properties")] = properties;
			}
			else
			{
				connector[U("type")] = web::json::value::string(U("Generalization"));

				auto general = web::json::value::object();
				auto specific = web::json::value::object();

				if (FAILED(hr = FillGeneralizationProperty(specific, spConnector, &IDualConnector::get_ClientID)))
				{
					return hr;
				}

				if (FAILED(hr = FillGeneralizationProperty(general, spConnector, &IDualConnector::get_SupplierID)))
				{
					return hr;
				}

				connector[U("specific")] = specific;
				connector[U("general")] = general;
			}

			content[content.size()] = connector;
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

	std::vector<web::json::value> vErrors;
	client.request(request).then([&vErrors](web::http::http_response response)
		{
			if (response.status_code() == web::http::status_codes::OK)
			{
				//////////
				// Be extremely careful here!!! We are on a background thread and we are not allowed to touch the EA API
				// (without properly marshaling interface pointers)
				//////////

				auto body = response.extract_json().get();
				auto results = body[U("result")];
				for (auto& result : results.as_array())
				{
					vErrors.push_back(result);
				}
			}
		}).wait();

	auto outputNr = 42;
	for (auto& err : vErrors)
	{
		auto description = err[U("description")];
		auto elementId = _wtoi(err[U("data")][U("source")][U("id")].as_string().c_str());

		m_validationResultsPerOutputId[outputNr] = err;
		m_outputIdToElementId[outputNr] = elementId;

		hr = pRepo->WriteOutput(CComBSTR(TAB_ONTOUML), CComBSTR(description.as_string().c_str()), outputNr);

		outputNr++;
	}

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

HRESULT COntoUML::GetConnectorIdInDiagramScope(IDispatch* pobjDiagram, long& lConnectorID)
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

HRESULT COntoUML::FillAssociationProperty(web::json::value& prop, std::wstring id, IDualConnector* pConnector, std::function<HRESULT(IDualConnector*, IDualConnectorEnd**)> endSelector, std::function<HRESULT(IDualConnector*, LONG*)> idSelector)
{
	if (!pConnector)
	{
		return E_INVALIDARG;
	}

	prop[U("id")] = web::json::value::string(id);
	prop[U("name")] = web::json::value();
	prop[U("description")] = web::json::value();
	prop[U("type")] = web::json::value::string(U("Property"));
	prop[U("propertyAssignments")] = web::json::value();
	prop[U("stereotype")] = web::json::value();
	prop[U("isDerived")] = web::json::value::boolean(false);
	prop[U("isReadOnly")] = web::json::value::boolean(false);
	prop[U("isOrdered")] = web::json::value::boolean(false);
	prop[U("cardinality")] = web::json::value::boolean(false);

	CComPtr<IDualConnectorEnd> spEnd;
	auto hr = endSelector(pConnector, &spEnd);

	CComBSTR bstrCardinality;
	if (SUCCEEDED(hr))
	{
		hr = spEnd->get_Cardinality(&bstrCardinality);

		if (SUCCEEDED(hr))
		{
			prop[U("cardinality")] = web::json::value::string(bstrCardinality.operator wchar_t* ());
		}
	}

	if (SUCCEEDED(hr))
	{
		LONG lID = 0;
		hr = idSelector(pConnector, &lID);

		if (SUCCEEDED(hr))
		{
			auto propertyType = web::json::value::object();

			propertyType[U("id")] = web::json::value::string(std::to_wstring(lID));
			propertyType[U("type")] = web::json::value::string(U("Class"));

			prop[U("propertyType")] = propertyType;

		}
	}

	return hr;
}

HRESULT COntoUML::FillGeneralizationProperty(web::json::value& prop, IDualConnector* pConnector, std::function<HRESULT(IDualConnector*, LONG*)> idSelector)
{
	if (!pConnector)
	{
		return E_INVALIDARG;
	}

	LONG lID = 0;
	auto hr = S_OK;
	if (FAILED(hr = idSelector(pConnector, &lID)))
	{
		return hr;
	}

	prop[U("id")] = web::json::value::string(std::to_wstring(lID));
	prop[U("type")] = web::json::value::string(U("Class"));

	return hr;
}

HRESULT COntoUML::CreateOntoUMLTab(IDualRepository* pRepo)
{

	CComBSTR bstrOntoUml(TAB_ONTOUML);
	auto hr = pRepo->CreateOutputTab(bstrOntoUml);
	if (SUCCEEDED(hr))
	{
		hr = pRepo->ClearOutput(bstrOntoUml);
	}
	if (SUCCEEDED(hr))
	{
		hr = pRepo->EnsureOutputVisible(bstrOntoUml);
	}

	return hr;
}
