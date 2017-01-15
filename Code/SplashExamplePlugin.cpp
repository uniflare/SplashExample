// 05/01/2017 Copyright by Uniflare. All rights reserved.
// See bundled license.txt for more information.

#include "StdAfx.h"
#include "SplashExamplePlugin.h"

///////////////////////////////////////////////////////////////////////////
CPlugin_SplashExample::CPlugin_SplashExample()
	: m_sCVars()
	, m_pSplashExample(nullptr)
{
	// Skip if we are in the editor
	if (gEnv->IsEditor())
	{
		CryLogAlways("SplashExample: Editor mode detected. Skipping splash example construction.");
		return;
	}

	// Make sure we were able to register our cvars with pConsole
	if (m_sCVars.m_bHasRegistered)
	{
		if (m_sCVars.m_iSplashEnable > 0)
			m_pSplashExample = new CSplashExample(&m_sCVars);
		else
			CRY_LOG_DEBUG("SplashExamplePlugin: Splash example has been disabled by cvar 'splash_show'.");
	}
}

///////////////////////////////////////////////////////////////////////////
CPlugin_SplashExample::~CPlugin_SplashExample()
{
	SAFE_DELETE(m_pSplashExample);
	// ~m_sCVars(); (stack)
}

///////////////////////////////////////////////////////////////////////////
//! ICryPlugin override
bool CPlugin_SplashExample::Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams)
{
	if (m_pSplashExample)
		return m_pSplashExample->Initialize(env, initParams);
	return false;
}

CRYREGISTER_SINGLETON_CLASS(CPlugin_SplashExample)