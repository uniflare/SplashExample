// Copyright (c) 2017, uniflare, see LICENSE.txt for details

#include "StdAfx.h"
#include "Plugin.h"

///////////////////////////////////////////////////////////////////////////
//! Module constructor
CPlugin_SplashExample::CPlugin_SplashExample()
	: m_sCVars()
	, m_pSplashExample(nullptr)
{
	// Skip if we are in the editor
	if (gEnv->IsEditor())
	{
		CRY_LOG_ALWAYS("Editor mode detected. Skipping splash example.");
		return;
	}
	else if (gEnv->IsDedicated())
	{
		CRY_LOG_ALWAYS("Dedicated mode detected. Skipping splash example.");
		return;
	}

	m_pSplashExample = CryAlignedNew<CSplashExample>();
}

///////////////////////////////////////////////////////////////////////////
//! Module destructor
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
	return true;
}

CRYREGISTER_SINGLETON_CLASS(CPlugin_SplashExample)