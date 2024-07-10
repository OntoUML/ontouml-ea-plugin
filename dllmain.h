// dllmain.h : Declaration of module class.

class ContoumleaplugintestModule : public ATL::CAtlDllModuleT< ContoumleaplugintestModule >
{
public :
	DECLARE_LIBID(LIBID_ontoumleaplugintestLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ONTOUMLEAPLUGINTEST, "{78d2889a-4a16-4088-9f87-ea270d443635}")
};

extern class ContoumleaplugintestModule _AtlModule;
