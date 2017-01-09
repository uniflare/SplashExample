// 05/01/2017 Copyright by Uniflare. All rights reserved.
// See bundled license.txt for more information.

#include "StdAfx.h"
#include "SplashExamplePlugin.h"

///////////////////////////////////////////////////////////////////////////
CPlugin_SplashExample::CPlugin_SplashExample()
	: m_pSplashExample(nullptr)
{
	// Don't bother if we are in the editor
	if (!gEnv->IsEditor())
	{
		// Register our CVars immediately
		RegisterCVars();

		if (gEnv->pConsole->GetCVar("splash_show")->GetIVal() > 0)
			m_pSplashExample = new CSplashExample();
		else
			CryLogAlways("SplashExample: splash_show was 0. Skipping splash example plugin.");
	}
	else
	{
		CryLogAlways("SplashExample: Editor mode detected. Skipping splash example.");
	}
}

///////////////////////////////////////////////////////////////////////////
CPlugin_SplashExample::~CPlugin_SplashExample()
{
	// Don't bother if we are in the editor
	if (!gEnv->IsEditor())
	{
		UnregisterCVars();
		SAFE_DELETE(m_pSplashExample);
	}
}

///////////////////////////////////////////////////////////////////////////
//! Registers CVars associated to this plugin
void CPlugin_SplashExample::RegisterCVars()
{
	REGISTER_CVAR2("splash_show", &m_sCVars.m_EnableSplash, 1, VF_CHEAT, "Whether to enable the splash plugin (0=no)");
	REGISTER_CVAR2("splash_show_initial", &m_sCVars.m_EnableSplashInitial, 1, VF_CHEAT, "Whether to enable the intial splash screen (0=no)");
	REGISTER_CVAR2("splash_minimumPlaybackTimeA", &m_sCVars.m_MinPlaybackSecA, 5.f, VF_CHEAT, "Initial Splash - Minimum playback time in seconds (float min 0.f)");
	REGISTER_CVAR2("splash_minimumPlaybackTime", &m_sCVars.m_MinPlaybackSec, 10.f, VF_CHEAT, "Main Splash - Minimum playback time in seconds (float min 0.f)");
	REGISTER_CVAR2("splash_startTimeOffset", &m_sCVars.m_StartTimeOffset, 0.0f, VF_CHEAT, "Offset to make splash_minimumPlaybackTime more accurate (float)");
	m_sCVars.m_pOverlayTexturePath = REGISTER_STRING("splash_texture", "SplashExample/textures/splash.dds", VF_CHEAT, "Sets the splash overlay texture to load");
	m_sCVars.m_pOverlayTexturePathA = REGISTER_STRING("splash_texture_a", "SplashExample/textures/splash_a.dds", VF_CHEAT, "Sets the initial splash overlay texture to load (before we go fullscreen)");
}

///////////////////////////////////////////////////////////////////////////
//! Unregisters CVars associated to this plugin
void CPlugin_SplashExample::UnregisterCVars()
{
	IConsole* pConsole = gEnv->pConsole;
	if (pConsole)
	{
		pConsole->UnregisterVariable("splash_texture_a");
		pConsole->UnregisterVariable("splash_texture");
		pConsole->UnregisterVariable("splash_startTimeOffset");
		pConsole->UnregisterVariable("splash_minimumPlaybackTime");
		pConsole->UnregisterVariable("splash_minimumPlaybackTimeA");
		pConsole->UnregisterVariable("splash_show_initial");
		pConsole->UnregisterVariable("splash_show");
	}
}

///////////////////////////////////////////////////////////////////////////
//! ICryPlugin override
bool CPlugin_SplashExample::Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams)
{
	if (!gEnv->IsEditor() && gEnv->pConsole->GetCVar("splash_show")->GetIVal() > 0)
		return (m_pSplashExample != nullptr);
	return true;
}

CSingletonFactory<CPlugin_SplashExample> CPlugin_SplashExample::s_factory;