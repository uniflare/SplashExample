#pragma once

#include <CrySystem/ICryPlugin.h>

struct ISplashExample;

struct ISplashExamplePlugin
	: public ICryPlugin
{
	CRYINTERFACE_DECLARE(ISplashExamplePlugin, 0x890949B33A2F41C6, 0x95EA3CFD1F696917);

	_BEFRIEND_CRYINTERFACE_CAST()
		_BEFRIEND_CRYCOMPOSITE_QUERY()

public:

	virtual ISplashExample*	GetISplashExample() const = 0;
	static  ISplashExamplePlugin*	GetSplashExamplePlugin() { return m_pThis; }

protected:
	static ISplashExamplePlugin* m_pThis;
};

ISplashExamplePlugin* ISplashExamplePlugin::m_pThis = nullptr;

DECLARE_SHARED_POINTERS(ISplashExamplePlugin);