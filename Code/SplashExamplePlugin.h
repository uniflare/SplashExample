// 05/01/2017 Copyright by Uniflare. All rights reserved.
// See bundled license.txt for more information.

#pragma once
#include "SplashExampleCVars.h"
#include "SplashExample.h"

#include <CrySystem/ICryPlugin.h>
#include <CryEntitySystem/IEntityClass.h>

class CSplashExample;
class CPlugin_SplashExample 
	: ICryPlugin
{
	// Console variables provided by this object
	SSplashExampleCVars m_sCVars;

	// reference to splash example implementation
	CSplashExample * m_pSplashExample;
public:
	CRYINTERFACE_SIMPLE(ICryPlugin)
	CRYGENERATE_SINGLETONCLASS(CPlugin_SplashExample, "Plugin_SplashExample", 0xF77494CD6D0E44FE, 0xBEFBAA9E2CA27CB5)

	// ICryPlugin
	virtual const char* GetName() const override { return "Splash Example Plugin"; }
	virtual const char* GetCategory() const override { return "Game"; }
	virtual bool Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams) override;
	virtual void OnPluginUpdate(EPluginUpdateType updateType) override {}
	// ~ICryPlugin

	CPlugin_SplashExample();
	virtual ~CPlugin_SplashExample();
};