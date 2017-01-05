// 05/01/2017 by Uniflare.

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
	REGISTER_CVAR2("splash_show", &m_sCVars.m_ShowSplashOverlay, 1, VF_CHEAT, "Whether to show the splash screen (0=no)");
	REGISTER_CVAR2("splash_minimumPlaybackTime", &m_sCVars.m_MinPlaybackSec, 10.f, VF_CHEAT, "Minimum playback time in seconds (float min 0.f)");
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