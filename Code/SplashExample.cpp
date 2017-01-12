// 05/01/2017 Copyright by Uniflare. All rights reserved.
// See bundled license.txt for more information.

#include "StdAfx.h"
#include "SplashExample.h"

#include <CryRenderer/IRenderer.h>
#include <CryInput/IInput.h>
#include <CryInput/IHardwareMouse.h>
#include <CryGame/IGameFramework.h>
#include <IPlayerProfiles.h>

// TODO:
// 
// Move construction to construct()
// Store pointers to cvars on init.
// 
// Always register to system immediately - system will always be init'd.
// 

///////////////////////////////////////////////////////////////////////////
CSplashExample::CSplashExample(SSplashExampleCVars * sCVars) :
	m_sCVars(sCVars)
	, m_bConstructed(false)
	, m_bListenerRegistered(false)
	, m_pSplashTexture(nullptr)
	, m_pInitialSplashTexture(nullptr)
{
	ISystem * pSystem = gEnv->pSystem;

	// Register as a system event listener
	if (!pSystem)
	{
		CRY_LOG_ERROR("SplashExamplePlugin: Cannot register system event listener. Skipping Construction.");
		return;
	}

	// Try to initialize splash example
	TryConstruct();
}

///////////////////////////////////////////////////////////////////////////
bool CSplashExample::Initialize(SSystemGlobalEnvironment & env, const SSystemInitParams & initParams)
{
	// Try to initialize splash example
	if(!m_bConstructed) TryConstruct();

	return m_bConstructed;
}

///////////////////////////////////////////////////////////////////////////
void CSplashExample::SafeReleaseSplashTextures()
{
	// Release our textures (-1 ref count)
	if (m_pInitialSplashTexture)
		m_pInitialSplashTexture->Release();

	if (m_pSplashTexture)
		m_pSplashTexture->Release();

	m_pSplashTexture = m_pInitialSplashTexture = nullptr;
}

///////////////////////////////////////////////////////////////////////////
CSplashExample::~CSplashExample()
{
	// Release our textures (-1 ref count)
	SafeReleaseSplashTextures();

	// Remove us from the event dispatcher system
	if(m_bListenerRegistered)
		gEnv->pSystem->GetISystemEventDispatcher()->RemoveListener(this);

	m_bListenerRegistered = m_bConstructed = false;
}

///////////////////////////////////////////////////////////////////////////
//! If plugin construction is ever moved upwards in system init, this will still be compatible.
//! Also useful if any system/required components failed to init yet for any reason.
void CSplashExample::TryConstruct()
{
	// Try to register our system listener, if we havn't managed to before now for some reason.
	m_bListenerRegistered = (!m_bListenerRegistered)? gEnv->pSystem->GetISystemEventDispatcher()->RegisterListener(this) : m_bListenerRegistered;

	// Only init once (Listener is required to function properly)
	if (!m_bConstructed && m_bListenerRegistered)
	{
		ICryPak * pCryPak = gEnv->pCryPak;
		IConsole * pConsole = gEnv->pConsole;
		IRenderer * pRenderer = gEnv->pRenderer;
		ITimer * pTimer = gEnv->pTimer;

		// Only construct if the system is in a state we can use
		if (!pConsole)
		{
			CRY_LOG_DEBUG("SplashExamplePlugin: pConsole has not initialized yet. Skipping construction.");
			return;
		}

		if(!pTimer)
		{
			CRY_LOG_DEBUG("SplashExamplePlugin: pTimer has not initialized yet. Skipping construction.");
			return;
		}

		if (!pCryPak)
		{
			CRY_LOG_DEBUG("SplashExamplePlugin: pCryPak has not initialized yet. Skipping construction.");
			return;
		}

		if (!pRenderer)
		{
			CRY_LOG_DEBUG("SplashExamplePlugin: pRenderer has not initialized yet. Skipping construction.");
			return;
		}

		// We really need our cvars
		if (!m_sCVars)
		{
			CRY_LOG_ERROR("SplashExamplePlugin: Null reference (CVars). Skipping construction.");
			return;
		}
		if (!m_sCVars->m_bHasRegistered)
		{
			CRY_LOG_ERROR("SplashExamplePlugin: CVars have not been registered. Skipping construction.");
			return;
		}

		// Attempt to load our intial splash texture from CVar
		if (!m_pInitialSplashTexture && m_sCVars->m_iInitialSplashEnable > 0)
		{
			string InitialSplashTexturePath = m_sCVars->m_pInitialSplashTexturePath->GetString();

			if (pCryPak->IsFileExist(InitialSplashTexturePath))
			{
				m_pInitialSplashTexture = pRenderer->EF_LoadTexture(InitialSplashTexturePath, FT_DONT_STREAM | FT_NOMIPS);
				if (!m_pInitialSplashTexture)
					CRY_LOG_ERROR("SplashExamplePlugin: Unable to load initial splash texture. Path '%s'", InitialSplashTexturePath);
			}
			else
				CRY_LOG_ERROR("SplashExamplePlugin: Initial splash texture not found. Path: '%s'", InitialSplashTexturePath);
		}

		// Attempt to load our main splash texture from CVar
		if (!m_pSplashTexture && m_sCVars->m_iMainSplashEnable > 0)
		{
			string SplashTexturePath = m_sCVars->m_pSplashTexturePath->GetString();
			if (pCryPak->IsFileExist(SplashTexturePath))
			{
				m_pSplashTexture = pRenderer->EF_LoadTexture(SplashTexturePath, FT_DONT_STREAM | FT_NOMIPS);
				if(!m_pSplashTexture)
					CRY_LOG_ERROR("SplashExamplePlugin: Unable to load initial splash texture. Path '%s'", SplashTexturePath);
			}
			else
				CRY_LOG_ERROR("SplashExamplePlugin: Splash texture not found. Path: '%s'", SplashTexturePath);
		}

		// Get a fallback resolution (from cvars)
		int iWidth = pConsole->GetCVar("r_width")->GetIVal();
		int iHeight = pConsole->GetCVar("r_height")->GetIVal();

		if (iWidth > pRenderer->GetWidth())
			iWidth = pRenderer->GetWidth();

		if (iHeight > pRenderer->GetHeight())
			iHeight = pRenderer->GetHeight();

		m_sFallbackResolution = SScreenResolution(iWidth, iHeight, 32, string(CryStringUtils::toString(iWidth) + "x" + CryStringUtils::toString(iHeight)));

		m_bConstructed = true;

		CRY_LOG_DEBUG("SplashExamplePlugin: Constructed splash example.");

		// Make sure console vars are set correctly for initial splash
		if (m_pInitialSplashTexture)
		{
			// Set viewport to our initial splash texture size.
			pConsole->GetCVar("r_width")->Set(m_pInitialSplashTexture->GetWidth());
			pConsole->GetCVar("r_height")->Set(m_pInitialSplashTexture->GetHeight());
			pConsole->GetCVar("r_fullscreen")->Set(0); // Force window mode (Note this may bring us our of fullscreen if not disabled in cfg).
			pConsole->GetCVar("r_fullscreenwindow")->Set(1); // Remove window border (Note same problem if not enabled in cfg).
			pConsole->GetCVar("sys_rendersplashscreen")->Set(0); // Disable built-in splash screen routine
		}
		else if(m_pSplashTexture)
		{
			pConsole->GetCVar("sys_rendersplashscreen")->Set(0); // Disable built-in splash screen routine
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//! Draws the supplied texture in stretched mode to the main viewport
void CSplashExample::Draw2DImage(const ITexture * pTex, bool bUseTextureSize) const
{
	if (pTex)
	{
		int RenderWidth = gEnv->pRenderer->GetOverlayWidth();
		int RenderHeight = gEnv->pRenderer->GetOverlayHeight();
		int TextureWidth = pTex->GetWidth();
		int TextureHeight = pTex->GetHeight();

		float PosX = 0;
		float PosY = 0;

		if (TextureWidth > 0 && TextureHeight > 0 && RenderWidth > 0 && RenderHeight > 0)
		{
			if (bUseTextureSize)
			{
				// Get Center offsets from window size
				if (TextureWidth < RenderWidth)
					PosX = (float)(int)((RenderWidth - TextureWidth) / 2); // raw cast to int to floor, then float to suppress warnings
				if (TextureHeight < RenderHeight)
					PosY = (float)(int)((RenderHeight - TextureHeight) / 2); // raw cast to int to floor, then float to suppress warnings

				// Don't exceed renderable dimensions
				RenderWidth = min(TextureWidth, RenderWidth);
				RenderHeight = min(TextureHeight, RenderHeight);
			}

			gEnv->pRenderer->SetViewport(0, 0, RenderWidth, RenderHeight);

			float fScaledW = RenderWidth / (float(RenderWidth) / 800.0f);
			float fScaledH = RenderHeight / (float(RenderHeight) / 600.0f);

			// make sure it's rendered in full screen mode when triple buffering is enabled as well
			for (size_t n = 0; n < 3; n++)
			{
				gEnv->pRenderer->SetCullMode(R_CULL_NONE);
				gEnv->pRenderer->SetState(GS_BLSRC_SRCALPHA | GS_BLDST_ONEMINUSSRCALPHA | GS_NODEPTHTEST);
				gEnv->pRenderer->Draw2dImageStretchMode(true);
				gEnv->pRenderer->Draw2dImage(PosX, PosY, fScaledW, fScaledH, pTex->GetTextureID(), 0.0f, 1.0f, 1.0f, 0.0f);
				gEnv->pRenderer->Draw2dImageStretchMode(false);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//! Stalls the current executing thread. Draws stretched texture. Returns false if app requested exit through PumpWindowMessage()
//! float StartTime			Time from getasynccurtime() start of render
//! float LengthTime		Time in seconds, duration of playback
//! ITexture * pTex			Texture to draw
//! bool bUseTextureSize	Use the size of the texture to set the viewport size
//! bool bUpdateInput		Each loop/frame, update input system
//! bool bUpdateMouse		Each loop/frame, update hardware mouse system
//! bool bDrawConsole		Each loop/frame, update console draw state
//! bool bPumpWinMsg		Each loop/frame, handle any pumped window messages
bool CSplashExample::DrawAndStall(float StartTime, float LengthTime, ITexture * pTex, bool bUseTextureSize, bool bUpdateInput, bool bUpdateMouse, bool bDrawConsole, bool bPumpWinMsg)
{
	if (pTex)
	{
		CSimpleThreadBackOff backoff;
		while (gEnv->pTimer->GetAsyncCurTime() - LengthTime <= StartTime) {

			// Make sure we don't stall on app exit
			if (gEnv->pSystem->IsQuitting())
				break;

			// Keep hiding cursor (needed for some reason)
			if(!gEnv->pConsole->GetStatus())
				gEnv->pSystem->GetIHardwareMouse()->Hide(true);
			else
				gEnv->pSystem->GetIHardwareMouse()->Hide(false);

			// Make sure windows doesn't think our application has crashed
			if (bPumpWinMsg) if (gEnv->pSystem->PumpWindowMessage(true) == -1) return false;

			// Update input events so we don't get any windows cursors during our splash
			if (bUpdateInput) gEnv->pInput->Update(true);
			if (bUpdateMouse) gEnv->pSystem->GetIHardwareMouse()->Update();

			// Render the splash image
			gEnv->pRenderer->BeginFrame();
			Draw2DImage(pTex, bUseTextureSize); // Our overlay texture (can have alpha which is why we need background color)
			if (bDrawConsole) gEnv->pConsole->Draw(); // Allow drawing of console while we stall
			gEnv->pRenderer->EndFrame();

			// Give the system some breathing space
			backoff.backoff();
			continue;
		};
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
//! Creates an integer from the player profile attribute requested
const int CSplashExample::GetAttributeIValue(const string attName, const IPlayerProfile * pProfile) const
{
	string attValue;
	if (pProfile->GetAttribute(attName, attValue) && attValue.length() > 0)
		return std::atoi(attValue);
	return -1;
}

//////////////////////////////////////////////////////////////////////////
//! Gets the currently used player profile
const IPlayerProfile * CSplashExample::GetCurrentPlayerProfile() const
{
	// Get the current user profile (or default if none)
	auto * pPlayerManager = gEnv->pGameFramework->GetIPlayerProfileManager();
	auto * pPlayerProfile = pPlayerManager->GetCurrentProfile(pPlayerManager->GetCurrentUser());
	return (!pPlayerProfile) ? pPlayerManager->GetDefaultProfile() : pPlayerProfile;
}

//////////////////////////////////////////////////////////////////////////
//! Gets the requested screen resolution from GetScreenResolutions()
const CSplashExample::SScreenResolution CSplashExample::GetScreenResolution(const int idx) const
{
	static auto ScreenResolutions = GetScreenResolutions();

	// Didn't get any resolution data for some reason
	if (ScreenResolutions.size() <= 0)
	{
		CRY_LOG_ERROR("SplashExamplePlugin: Could not get available screen resolutions for display.");
		return m_sFallbackResolution;
	}

	// If we don't have a resolution this high, get the highest available
	if (idx >= ScreenResolutions.size() || idx < 0)
	{
		CRY_LOG_RELEASE("SplashExamplePlugin: idx is not supported, defaulting to highest available. ('%s')", ScreenResolutions[ScreenResolutions.size() - 1].sResolution);
		return ScreenResolutions[ScreenResolutions.size() - 1];
	}

	return ScreenResolutions[idx];
}

//////////////////////////////////////////////////////////////////////////
//! Generates a list of supported screen resolutions
//! From GameSDK Sample
const std::vector<CSplashExample::SScreenResolution> CSplashExample::GetScreenResolutions() const
{
	static std::vector<SScreenResolution> ScreenResolutions;

#if CRY_PLATFORM_DESKTOP

	if (ScreenResolutions.size() > 0)
		return ScreenResolutions;

	CryFixedStringT<16> format;

	SDispFormat *formats = NULL;
	int numFormats = gEnv->pRenderer->EnumDisplayFormats(NULL);
	if (numFormats)
	{
		formats = new SDispFormat[numFormats];
		gEnv->pRenderer->EnumDisplayFormats(formats);
	}

	for (int i = 0; i < numFormats; ++i)
	{
		bool bAlreadyExists = false;

		for (auto &res : ScreenResolutions)
			if (res.iWidth == formats[i].m_Width && res.iHeight == formats[i].m_Height)
				bAlreadyExists = true;

		if (bAlreadyExists || formats[i].m_Width < 800)
			continue;

		format.Format("%i X %i", formats[i].m_Width, formats[i].m_Height);

		ScreenResolutions.emplace_back(formats[i].m_Width, formats[i].m_Height, formats[i].m_BPP, format.c_str());
	}

	if (formats)
		delete[] formats;

#endif

	return ScreenResolutions;
}

///////////////////////////////////////////////////////////////////////////
//! Sets the r_width and r_height CVars if they are set properly in sResolution
void CSplashExample::SetScreenResolution(const SScreenResolution &res) const
{

	if (res.iWidth > 0 && res.iHeight > 0)
	{
		CRY_LOG_DEBUG("SplashExamplePlugin: Setting resolution to '%s'", res.sResolution);

		gEnv->pConsole->GetCVar("r_width")->Set(res.iWidth);
		gEnv->pConsole->GetCVar("r_height")->Set(res.iHeight);
	}
	else
	{
		CRY_LOG_ERROR("SplashExamplePlugin: Invalid resolution params supplied to SetScreenResolution(). (iWidth=%s,iHeight=%s)", res.iWidth, res.iHeight);
	}
}

///////////////////////////////////////////////////////////////////////////
void CSplashExample::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
	// Construct at the earliest opportunity
	// * This is a safe-guard if the plugins object construction
	// * is moved before other systems have initialized.
	// * (Currently in 5.3.0, Construction, Initialization, and 
	// * ESYSTEM_EVENT_PRE_RENDERER_INIT are called at the same 
	// * time in CSystem::Init)

	if (event == ESYSTEM_EVENT_CVAR_REGISTERED || event == ESYSTEM_EVENT_CVAR_UNREGISTERED)
		return;

	CRY_LOG_DEBUG("SplashExamplePlugin: Debug OnSystemEvent. event = %s", CryStringUtils::toString(event));

	if (!m_bConstructed) TryConstruct();
	if (!m_bConstructed) return; // Only respond to events if we are ready to

	switch (event)
	{
		// First stage, override startscreen, hide cursor and draw our gui splash (splash_a)
	case ESYSTEM_EVENT_PRE_RENDERER_INIT:
	{

		// Initial splash
		if (m_pInitialSplashTexture)
		{
			// Hide cursor immediately
			gEnv->pSystem->GetIHardwareMouse()->Hide(true);

			// Push a single frame with our texture to renderer
			gEnv->pRenderer->BeginFrame();
			Draw2DImage(m_pInitialSplashTexture, true);
			gEnv->pRenderer->EndFrame();
		}

		break;
	}

	// Second stage, switch resolution, toggle fullscreen, start our main splash render
	case ESYSTEM_EVENT_GAME_POST_INIT:
	{
		// Keep showing our initial splash screen
		if (m_pInitialSplashTexture)
		{
			CRY_LOG_DEBUG("SplashExamplePlugin: Stalling thread for initial splash");
			if (!DrawAndStall(gEnv->pTimer->GetAsyncCurTime(), m_sCVars->m_fInitialSplashPlaybackTime, m_pInitialSplashTexture, true)) break;
		}

		// GAMESDK/PROFILE SUPPORT //
		SScreenResolution ScreenResolution = GetScreenResolution(-1); // default native res or fallback if no profile

		// We need to set these cvars before we FlushRTCommands since they won't be set yet.
		if (auto pProfile = GetCurrentPlayerProfile())
		{
			const int &userFS = GetAttributeIValue("Fullscreen", pProfile);
			const int &userFSW = GetAttributeIValue("FullscreenWindow", pProfile);
			const int &userRes = GetAttributeIValue("Resolution", pProfile);

			if (userFS != -1)
			{
				gEnv->pConsole->GetCVar("r_fullscreen")->Set(userFS);
				CRY_LOG_DEBUG("SplashExamplePlugin: Set Fullscreen CVar to profile setting ('%s')", CryStringUtils::toString(userFS));
			}

			if (userFSW != -1)
			{
				gEnv->pConsole->GetCVar("r_fullscreenwindow")->Set(userFSW);
				CRY_LOG_DEBUG("SplashExamplePlugin: Set FullscreenWindow CVar to profile setting ('%s')", CryStringUtils::toString(userFSW));
			}

			if (userRes != -1)
			{
				ScreenResolution = GetScreenResolution(userRes);
				CRY_LOG_DEBUG("SplashExamplePlugin: Got splash resolution from profile setting ('%s')", ScreenResolution.sResolution);
			}
		}
		else
		{
			CryLogAlways("SplashExamplePlugin: No default profile loaded during GAME_POST_INIT. Defaulting to CVars.");
		}
		// ~GAMESDK/PROFILE SUPPORT //

		SetScreenResolution(ScreenResolution);

		// This will update the window using any changed cvars
		gEnv->pRenderer->FlushRTCommands(false, true, true);

		// Render the splash image (Drawing here helps with playback time accuracy)
		if (m_pSplashTexture)
		{
			// Hide cursor again
			gEnv->pSystem->GetIHardwareMouse()->Hide(true);

			gEnv->pRenderer->BeginFrame();
			Draw2DImage(m_pSplashTexture);
			gEnv->pRenderer->EndFrame();
		}

		break;
	}

	// Third stage, Keep rendering splash image, and delay further loading until minimum playback time is achieved.
	case ESYSTEM_EVENT_GAME_POST_INIT_DONE:
	{
		if (m_pSplashTexture)
		{
			// Hide cursor (if we havn't already)
			gEnv->pSystem->GetIHardwareMouse()->Hide(false);

			// Set the start time for render, note this is not an accurate stamp 
			// for when the image is actually rendered to the screen so we can 
			// apply an additional offset here.
			float StartTime = gEnv->pTimer->GetAsyncCurTime() + m_sCVars->m_fStartTimeOffset;

			// Get the minimum playback time
			float LengthTime = m_sCVars->m_fSplashPlaybackTime;

			CRY_LOG_DEBUG("SplashExamplePlugin: Stalling thread for splash, (start=%s,Length=%s)", CryStringUtils::toString(StartTime), CryStringUtils::toString(LengthTime));

			// Stall the engine if we havn't shown our splash for enough time!
			DrawAndStall(StartTime, LengthTime, m_pSplashTexture, false);
		}

		// Show cursor
		gEnv->pSystem->GetIHardwareMouse()->Hide(false);

		// We're done, de-register our system event handler
		if (m_bListenerRegistered)
		{
			gEnv->pSystem->GetISystemEventDispatcher()->RemoveListener(this);
			m_bListenerRegistered = false;

			CRY_LOG_DEBUG("SplashExamplePlugin: Unregistered system listener");
		}

		// Make sure we dont use this tex again!
		if (m_pSplashTexture) m_pSplashTexture->Release();
		m_pSplashTexture = nullptr;

		CRY_LOG_DEBUG("SplashExamplePlugin: End of splash example");
	}
	}
}