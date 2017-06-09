// 11/01/2017 Copyright by Uniflare. All rights reserved.
// See bundled license.txt for more information.
#pragma once

#include <CrySystem\ISystem.h>
#include <CrySystem\IConsole.h>

struct CVars
{
	bool m_bHasRegistered;

	int m_iSplashEnable;
	int m_iMainSplashEnable;
	int m_iInitialSplashEnable;

	float m_fSplashPlaybackTime;
	float m_fInitialSplashPlaybackTime;
	float m_fStartTimeOffset;

	ICVar * m_pSplashTexturePath;
	ICVar * m_pInitialSplashTexturePath;

	// Register console variables
	CVars() :
		m_bHasRegistered(false)
		, m_iSplashEnable(false)
		, m_iInitialSplashEnable(false)
		, m_iMainSplashEnable(false)
		, m_fSplashPlaybackTime(0.f)
		, m_fInitialSplashPlaybackTime(0.f)
		, m_fStartTimeOffset(0.f)
		, m_pSplashTexturePath(nullptr)
		, m_pInitialSplashTexturePath(nullptr)

	{
		CRY_LOG_CALL("SplashExamplePlugin: CVars::CVars()");
	}
	
	// Unregister console variables
	~CVars()
	{
		CRY_LOG_CALL("SplashExamplePlugin: CVars::~CVars()");
		assert(gEnv->pConsole);
		if (!gEnv->pConsole) return;
		
		gEnv->pConsole->UnregisterVariable("splash_texture_a");
		gEnv->pConsole->UnregisterVariable("splash_texture");
		gEnv->pConsole->UnregisterVariable("splash_startTimeOffset");
		gEnv->pConsole->UnregisterVariable("splash_minimumPlaybackTime");
		gEnv->pConsole->UnregisterVariable("splash_minimumPlaybackTimeA");
		gEnv->pConsole->UnregisterVariable("splash_show_initial");
		gEnv->pConsole->UnregisterVariable("splash_show_main");
		gEnv->pConsole->UnregisterVariable("splash_show");

		CRY_LOG_DEBUG("SplashExamplePlugin: Unregistered CVars.");
	}

	bool Init()
	{
		assert(gEnv->pConsole);
		if (!gEnv->pConsole) return false;

		REGISTER_CVAR2("splash_show", &m_iSplashEnable, 1, VF_CHEAT, "Whether to enable the splash plugin (0=no)");
		REGISTER_CVAR2("splash_show_main", &m_iMainSplashEnable, 1, VF_CHEAT, "Whether to enable the main (second) splash image (0=no)");
		REGISTER_CVAR2("splash_show_initial", &m_iInitialSplashEnable, 1, VF_CHEAT, "Whether to enable the intial splash screen (0=no)");
		REGISTER_CVAR2("splash_minimumPlaybackTimeA", &m_fInitialSplashPlaybackTime, 5.f, VF_CHEAT, "Initial Splash - Minimum playback time in seconds (float min 0.f)");
		REGISTER_CVAR2("splash_minimumPlaybackTime", &m_fSplashPlaybackTime, 10.f, VF_CHEAT, "Main Splash - Minimum playback time in seconds (float min 0.f)");
		REGISTER_CVAR2("splash_startTimeOffset", &m_fStartTimeOffset, 0.0f, VF_CHEAT, "Offset to make splash_minimumPlaybackTime more accurate (float)");
		m_pSplashTexturePath = REGISTER_STRING("splash_texture", "splashexample/textures/splash.dds", VF_CHEAT, "Sets the splash overlay texture to load");
		m_pInitialSplashTexturePath = REGISTER_STRING("splash_texture_a", "splashexample/textures/splash_a.dds", VF_CHEAT, "Sets the initial splash overlay texture to load (before we go fullscreen)");

		CRY_LOG_DEBUG("SplashExamplePlugin: Registered CVars.");
		return true;
	}
};