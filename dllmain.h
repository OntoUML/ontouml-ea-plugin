// dllmain.h : Declaration of module class.

class ContoumleapluginModule : public ATL::CAtlDllModuleT< ContoumleapluginModule >
{
public :
	DECLARE_LIBID(LIBID_ontoumleapluginLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ONTOUMLEAPLUGIN, "{78d2889a-4a16-4088-9f87-ea270d443635}")
};

extern class ContoumleapluginModule _AtlModule;
