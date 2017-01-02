#pragma once

#include <CrySystem/ICryPlugin.h>

#include <CryGame/IGameFramework.h>

#include <CryEntitySystem/IEntityClass.h>

#include <Interface\ISplashExamplePlugin.h>
#include <System/SplashExample.h>

class CSplashExample;

class CPlugin_SplashExample 
	: public ISplashExamplePlugin
{
	CRYINTERFACE_BEGIN()
		CRYINTERFACE_ADD(ISplashExamplePlugin)
		CRYINTERFACE_ADD(ICryPlugin)
		CRYINTERFACE_END()

	CRYGENERATE_SINGLETONCLASS(CPlugin_SplashExample, "Plugin_Sample", 0xF77494CD6D0E44FE, 0xBEFBAA9E2CA27CB5)
public:
	CPlugin_SplashExample();
	virtual ~CPlugin_SplashExample();
	
	//! Retrieve name of plugin.
	virtual const char* GetName() const override { return "Plugin_Sample"; }

	//! Retrieve category for the plugin.
	virtual const char* GetCategory() const override { return "Game"; }

	//! This is called to initialize the new plugin.
	virtual bool Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams) override;

	virtual void OnPluginUpdate(EPluginUpdateType updateType) override {}

	virtual ISplashExample*				GetISplashExample() const;
	static  CPlugin_SplashExample*		GetSplashExamplePlugin() { return m_pThis; }

protected:
	static CPlugin_SplashExample* m_pThis;

private:
	//void Register();
	//void Unregister();

	CSplashExample* m_pSplashExample;
};