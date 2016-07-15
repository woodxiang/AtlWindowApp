#pragma once
class CRibbonApp
	: public CComObjectRootEx<CComMultiThreadModel>
	, public IUIApplication
{
public:
	BEGIN_COM_MAP(CRibbonApp)
		COM_INTERFACE_ENTRY(IUIApplication)
	END_COM_MAP()


	STDMETHOD(OnViewChanged)(UINT32 nViewID, __in UI_VIEWTYPE typeID, __in IUnknown* pView, UI_VIEWVERB verb, INT32 uReasonCode)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnCreateUICommand)(UINT32 nCmdID, __in UI_COMMANDTYPE typeID, __deref_out IUICommandHandler** ppCommandHandler)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnDestroyUICommand)(UINT32 commandId, __in UI_COMMANDTYPE typeID, __in_opt IUICommandHandler* pCommandHandler)
	{
		return E_NOTIMPL;
	}
};

