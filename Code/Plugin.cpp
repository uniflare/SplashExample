#include "StdAfx.h"
#include "Plugin.h"
#include "Interface\ISplashExample.h"

// Included only once per DLL module.
#include <CryCore/Platform/platform_impl.inl>

CPlugin_SplashExample::CPlugin_SplashExample()
{
	SplashExampleDebugMsg("CPlugin::CPlugin()");

	// Debug, make sure verbosity is high enough to show debug comments
	gEnv->pConsole->GetCVar("log_verbosity")->Set(4);
	gEnv->pConsole->GetCVar("log_WriteToFileVerbosity")->Set(4);
}
CPlugin_SplashExample::~CPlugin_SplashExample()
{
	SplashExampleDebugMsg("CPlugin::~CPlugin()");

	SAFE_DELETE(m_pSplashExample)
}

bool CPlugin_SplashExample::Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams)
{
	SplashExampleDebugMsgV("CPlugin::Initialize(env = %s, initParams = %s)"
		, CFVDEBUGUTIL::POINTER_TO_HEXADDR((uintptr_t)&env)
		, CFVDEBUGUTIL::POINTER_TO_HEXADDR((uintptr_t)&initParams));

	// Create and initialize main plugin interface
	m_pSplashExample = new CSplashExample();
	m_pSplashExample->Initialize(env, initParams);
	return (m_pSplashExample != nullptr);

	return true;
}

///////////////////////////////////////////////////////////////////////////
ISplashExample* CPlugin_SplashExample::GetISplashExample() const
{
	SplashExampleDebugMsg("CPlugin::GetISplashExample()");
	return m_pSplashExample;
}

CSingletonFactory<CPlugin_SplashExample> CPlugin_SplashExample::s_factory;