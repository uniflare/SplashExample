// 05/01/2017 by Uniflare.

#pragma once
#include "SplashExample.h"

#include <CrySystem/ICryPlugin.h>
#include <CryEntitySystem/IEntityClass.h>

class CSplashExample;
class CPlugin_SplashExample 
	: ICryPlugin
{

	// Console variables provided by this object
	struct SSplashExampleCVars
	{
		int m_EnableSplash;
		int m_EnableSplashInitial;
		int m_StallSystemInit;

		float m_MinPlaybackSecA;
		float m_MinPlaybackSec;
		float m_StartTimeOffset;

		ICVar * m_pOverlayTexturePathA;
		ICVar * m_pOverlayTexturePath;
	} m_sCVars;

	// reference to splash example logic
	CSplashExample * m_pSplashExample;

	//! Registers CVars associated to this plugin
	void RegisterCVars();

	//! Unregisters CVars associated to this plugin
	void UnregisterCVars();
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