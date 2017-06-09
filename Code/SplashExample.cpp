// 05/01/2017 Copyright by Uniflare. All rights reserved.
// See bundled license.txt for more information.

#include "StdAfx.h"
#include "SplashExample.h"

#include <CryRenderer/IRenderer.h>
#include <CryInput/IInput.h>
#include <CryInput/IHardwareMouse.h>
#include <CryGame/IGameFramework.h>
#include <CryFlowGraph\IFlowSystem.h>
#include <IPlayerProfiles.h>


///////////////////////////////////////////////////////////////////////////
//! Default Constructor
CSplashExample::CSplashExample() :
	m_sCVars()
	, m_bInitialized(false)
	, m_bSystemListenerRegistered(false)
	, m_bProfileListenerRegistered(false)
	, m_bLoadedProfileAttributes(false)
	, m_bIgnoreNextProfileSave(false)
	, m_pSplashTexture(nullptr)
	, m_pInitialSplashTexture(nullptr)
	, m_pProfileManager(nullptr)
	, m_pCurrentProfile(nullptr)
	, m_pDefaultProfile(nullptr)
{
}

///////////////////////////////////////////////////////////////////////////
//! Destructor
CSplashExample::~CSplashExample()
{
	SafeReleaseSplashTextures();
	UnregisterProfileListener(this);

	if (m_bSystemListenerRegistered)
	{
		gEnv->pSystem->GetISystemEventDispatcher()->RemoveListener(this);
		CRY_LOG_DEBUG("SplashExamplePlugin: Unregistered system listener");
	}

	// flatten flags
	m_bSystemListenerRegistered = m_bProfileListenerRegistered = m_bInitialized = false;
}

///////////////////////////////////////////////////////////////////////////
//! Called from ICryPlugin interface
bool CSplashExample::Initialize(SSystemGlobalEnvironment & env, const SSystemInitParams & initParams)
{
	m_sCVars.Init();

	// Try to initialize splash example
	TryInitialize();

	return true;
}

///////////////////////////////////////////////////////////////////////////
//! Releases textures if possible, ensures nulled pointers.
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
//! If plugin construction is ever moved upwards in system init, this will still be compatible.
//! Also useful if any system/required components failed to init yet for any reason.
void CSplashExample::TryInitialize()
{
	CRY_LOG_CALL("SplashExamplePlugin: CSplashExample::TryConstruct()");

	// Try to register our system listener, if we havn't managed to before now for some reason.
	m_bSystemListenerRegistered = (!m_bSystemListenerRegistered)? gEnv->pSystem->GetISystemEventDispatcher()->RegisterListener(this) : m_bSystemListenerRegistered;

	// Only init once (Listener is required to function properly)
	if (!m_bInitialized && m_bSystemListenerRegistered)
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

		// Attempt to load our intial splash texture from CVar
		if (!m_pInitialSplashTexture && m_sCVars.m_iInitialSplashEnable > 0)
		{
			string InitialSplashTexturePath = m_sCVars.m_pInitialSplashTexturePath->GetString();

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
		if (!m_pSplashTexture && m_sCVars.m_iMainSplashEnable > 0)
		{
			string SplashTexturePath = m_sCVars.m_pSplashTexturePath->GetString();
			if (pCryPak->IsFileExist(SplashTexturePath))
			{
				m_pSplashTexture = pRenderer->EF_LoadTexture(SplashTexturePath, FT_DONT_STREAM | FT_NOMIPS);
				if(!m_pSplashTexture)
					CRY_LOG_ERROR("SplashExamplePlugin: Unable to load initial splash texture. Path '%s'", SplashTexturePath);
			}
			else
				CRY_LOG_ERROR("SplashExamplePlugin: Splash texture not found. Path: '%s'", SplashTexturePath);
		}

		if (m_pSplashTexture)
		{
			pConsole->GetCVar("sys_rendersplashscreen")->Set(0); // Disable built-in splash screen routine
		}

		m_bInitialized = true;
		CRY_LOG_DEBUG("SplashExamplePlugin: Constructed splash example.");
	}
}

///////////////////////////////////////////////////////////////////////////
//! Handles the initial splash display.
void CSplashExample::DisplayInitialSplash(bool stallThread)
{
	CRY_LOG_CALL("SplashExamplePlugin: CSplashExample::DisplayInitialSplash()");

	if (m_pInitialSplashTexture)
	{
		// Set viewport to our initial splash texture size if not already.
		gEnv->pConsole->GetCVar("r_width")->Set(m_pInitialSplashTexture->GetWidth());
		gEnv->pConsole->GetCVar("r_height")->Set(m_pInitialSplashTexture->GetHeight());
		gEnv->pConsole->GetCVar("r_fullscreen")->Set(0); // Force window mode (Note this may bring us our of fullscreen if not disabled in cfg).
		gEnv->pConsole->GetCVar("r_fullscreenwindow")->Set(1); // Remove window border (Note same problem if not enabled in cfg).

		gEnv->pConsole->GetCVar("sys_rendersplashscreen")->Set(0); // Disable built-in splash screen routine

		// force window update
		gEnv->pRenderer->FlushRTCommands(true, false, false);

		if (!stallThread)
		{
			if (m_pInitialSplashTexture)
			{
				// Hide cursor immediately
				gEnv->pSystem->GetIHardwareMouse()->Hide(true);

				// Push a single frame with our texture to renderer
				gEnv->pRenderer->BeginFrame();
				Draw2DImage(m_pInitialSplashTexture, true);
				gEnv->pRenderer->EndFrame();
			}
		}
		else 
		{
				CRY_LOG_DEBUG("SplashExamplePlugin: Stalling thread for initial splash");
				if (!DrawAndStall(gEnv->pTimer->GetAsyncCurTime(), m_sCVars.m_fInitialSplashPlaybackTime, m_pInitialSplashTexture, true))
					return;
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//! Handles the main splash display.
void CSplashExample::DisplayMainSplash(const bool stallThread)
{
	CRY_LOG_CALL("SplashExamplePlugin: CSplashExample::DisplayMainSplash()");

	const SWindowProperties sWindowProperties = GetWindowProperties();

	// Propagate changes
	static bool bSetCVars = false;
	if (!bSetCVars)
	{
		gEnv->pConsole->GetCVar("r_fullscreen")->Set(sWindowProperties.sWindowMode.bFullscreen);
		CRY_LOG_DEBUG("SplashExamplePlugin: Set Fullscreen CVar to profile setting ('%s')", CryStringUtils::toString(sWindowProperties.sWindowMode.bFullscreen));
		gEnv->pConsole->GetCVar("r_fullscreenwindow")->Set(sWindowProperties.sWindowMode.bHideWindowBorder);
		CRY_LOG_DEBUG("SplashExamplePlugin: Set FullscreenWindow CVar to profile setting ('%s')", CryStringUtils::toString(sWindowProperties.sWindowMode.bHideWindowBorder));
		SetScreenResolution(sWindowProperties.sScreenResolution);
		CRY_LOG_DEBUG("SplashExamplePlugin: Got splash resolution from profile setting ('%s')", sWindowProperties.sScreenResolution.sResolution);
		bSetCVars = true;
	}

	// This will update the window using any changed cvars
	gEnv->pRenderer->FlushRTCommands(false, true, true);

	if (m_pSplashTexture)
	{
		// Hide cursor
		gEnv->pSystem->GetIHardwareMouse()->Hide(true);

		if (!stallThread)
		{
			gEnv->pRenderer->BeginFrame();
			Draw2DImage(m_pSplashTexture);
			gEnv->pRenderer->EndFrame();
		}
		else
		{
			// Set the start time for render, note this is not an accurate stamp 
			// for when the image is actually rendered to the screen so we can 
			// apply an additional offset here.
			float StartTime = gEnv->pTimer->GetAsyncCurTime() + m_sCVars.m_fStartTimeOffset;

			// Get the minimum playback time
			float LengthTime = m_sCVars.m_fSplashPlaybackTime;

			CRY_LOG_DEBUG("SplashExamplePlugin: Stalling thread for splash, (start=%s,Length=%s)", CryStringUtils::toString(StartTime), CryStringUtils::toString(LengthTime));

			// Stall the engine if we havn't shown our splash for enough time!
			DrawAndStall(StartTime, LengthTime, m_pSplashTexture, false);

			// After Stall...

			// Show cursor
			gEnv->pSystem->GetIHardwareMouse()->Hide(false);

			// Make sure we dont use this tex again!
			if (m_pSplashTexture) m_pSplashTexture->Release();
			m_pSplashTexture = nullptr;

			CRY_LOG_DEBUG("SplashExamplePlugin: Finished main splash screen draw cycle.");
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

			//gEnv->pRenderer->SetViewport(0, 0, RenderWidth, RenderHeight);

			// Force fullscreen render
			/*
			float fScaledW = RenderWidth / (float(RenderWidth) / 800.0f);
			float fScaledH = RenderHeight / (float(RenderHeight) / 600.0f);
			*/
			float fScaledW = 800.f;
			float fScaledH = 600.f;

			// make sure it's rendered in full screen mode when triple buffering is enabled as well
			for (size_t n = 0; n < 3; n++)
			{
			//	gEnv->pRenderer->SetCullMode(R_CULL_NONE);
				gEnv->pRenderer->SetState(GS_NODEPTHTEST);
			//	gEnv->pRenderer->Draw2dImageStretchMode(true);
			//	gEnv->pRenderer->Draw2dImage(PosX, PosY, fScaledW, fScaledH, pTex->GetTextureID(), 0.0f, 1.0f, 1.0f, 0.0f);
				gEnv->pRenderer->Draw2dImage(0, 0, 800, 600, pTex->GetTextureID(), 0.0f, 1.0f, 1.0f, 0.0f);
			//	gEnv->pRenderer->Draw2dImageStretchMode(false);
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
	CRY_LOG_CALL("SplashExamplePlugin: CSplashExample::DrawAndStall()");

	if (pTex)
	{
		CSimpleThreadBackOff backoff;
		while (gEnv->pTimer->GetAsyncCurTime() - LengthTime <= StartTime) {

			// Make sure we don't stall on app exit
			if (IsShuttingDown())
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

///////////////////////////////////////////////////////////////////////////
//! Gets the user-defined, or default-defined window properties from PlayerProfiles.
const CSplashExample::SWindowProperties CSplashExample::GetWindowProperties()
{
	CRY_LOG_CALL("SplashExamplePlugin: CSplashExample::GetWindowProperties()");

	assert(m_pDefaultProfile != nullptr);

	// Revert to defaults from lib/config
	auto pProfile = (m_pCurrentProfile) ? m_pCurrentProfile : m_pDefaultProfile;

	// Get profile attributes or use predefined defaults
	int paResolution = -1;
	if (!pProfile->GetAttribute("Resolution", paResolution, true))
		paResolution = WINDOW_DEFAULT_RESOLUTION_IDX;

	bool paFullscreen;
	if (!pProfile->GetAttribute("Fullscreen", paFullscreen, true))
		paFullscreen = WINDOW_DEFAULT_FULLSCREEN;

	bool paFullscreenWindow;
	if (!pProfile->GetAttribute("FullscreenWindow", paFullscreenWindow, true))
		paFullscreenWindow = WINDOW_DEFAULT_HIDE_BORDER;

	return SWindowProperties(
		GetScreenResolution(paResolution),
		SWindowMode(paFullscreen, paFullscreenWindow)
	);
}

//////////////////////////////////////////////////////////////////////////
//! Gets the requested screen resolution from GetScreenResolutions()
const CSplashExample::SScreenResolution CSplashExample::GetScreenResolution(const int idx)
{
	CRY_LOG_CALL("SplashExamplePlugin: CSplashExample::GetScreenResolution()");

	static auto ScreenResolutions = CSplashExample::GetScreenResolutions();

	// Didn't get any resolution data for some reason
	if (ScreenResolutions.size() <= 0)
	{
		CRY_LOG_ERROR("SplashExamplePlugin: Could not get available screen resolutions for display.");
		return SScreenResolution();
	}

	// If we don't have a resolution this high, get the highest available
	if (idx >= ScreenResolutions.size() || idx < 0)
	{
		CRY_LOG_ALWAYS("SplashExamplePlugin: idx is not supported, defaulting to highest available. ('%s')", ScreenResolutions[ScreenResolutions.size() - 1].sResolution);
		return ScreenResolutions[ScreenResolutions.size() - 1];
	}

	return ScreenResolutions[idx];
}

//////////////////////////////////////////////////////////////////////////
//! Generates a list of supported screen resolutions
//! From GameSDK Sample
const std::vector<CSplashExample::SScreenResolution> CSplashExample::GetScreenResolutions()
{
	CRY_LOG_CALL("SplashExamplePlugin: CSplashExample::GetScreenResolutions()");

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
	CRY_LOG_CALL("SplashExamplePlugin: CSplashExample::SetScreenResolution()");

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
//! Gets the Player Profile Manager interface
IPlayerProfileManager * CSplashExample::GetIPlayerProfileManager()
{
	CRY_LOG_CALL("SplashExamplePlugin: CSplashExample::GetIPlayerProfileManager()");

	assert(gEnv->pGameFramework != nullptr);

	// Make sure we have what we need
	if (!gEnv->pGameFramework)
		return nullptr;

	return gEnv->pGameFramework->GetIPlayerProfileManager();
}

//////////////////////////////////////////////////////////////////////////
//! Gets the default player profile
IPlayerProfile * CSplashExample::GetDefaultPlayerProfile(IPlayerProfileManager * pProfileManager)
{
	CRY_LOG_CALL("SplashExamplePlugin: CSplashExample::GetDefaultPlayerProfile()");

	// Get the current user profile (or default if none)
	return (pProfileManager) ? pProfileManager->GetDefaultProfile() : nullptr;
}

//////////////////////////////////////////////////////////////////////////
//! Gets the currently used player profile
IPlayerProfile * CSplashExample::GetCurrentPlayerProfile(IPlayerProfileManager * pProfileManager)
{
	CRY_LOG_CALL("SplashExamplePlugin: CSplashExample::GetCurrentPlayerProfile()");

	// Get the current user profile (or default if none)
	return (pProfileManager) ? pProfileManager->GetCurrentProfile(pProfileManager->GetCurrentUser()) : nullptr;
}

///////////////////////////////////////////////////////////////////////////
//! Sets the splash flag attribute & manually saves the profile
bool CSplashExample::TryLoadProfileAttributes()
{
	CRY_LOG_CALL("SplashExamplePlugin: CSplashExample::TryLoadProfileAttributes()");

	// Attempt to get profile manager if we havn't already
	m_pProfileManager = (!m_pProfileManager)? GetIPlayerProfileManager() : m_pProfileManager;
	if (m_pProfileManager != nullptr)
	{
		// Register the profile listener
		RegisterProfileListener(this);

		// Attempt to get the default profile if we havn't already
		m_pDefaultProfile = (!m_pDefaultProfile) ? GetDefaultPlayerProfile() : m_pDefaultProfile;
		if (m_pDefaultProfile != nullptr)
		{
			// Attempt to get the current profile if we havn't already
			m_pCurrentProfile = (!m_pCurrentProfile)? GetCurrentPlayerProfile() : m_pCurrentProfile;
			if (m_pCurrentProfile != nullptr)
			{
				// Use the SplashFlag in the player profile to check if this is the very first game launch
				bool bSplashFlag = true;
				if (!m_pCurrentProfile->GetAttribute("SplashFlag_FirstRun", bSplashFlag, false))
				{
					int res = 0;
					// If profile attribute Resolution is <= 0 (or not set).
					if (!m_pCurrentProfile->GetAttribute("Resolution", res, false) || res <= 0)
					{
						// First launch of game, copy defaults (libs/config) to the current profile.
						if (!CopyProfileAttributes(/* To */ m_pCurrentProfile, /* From */ m_pDefaultProfile))
						{
							CRY_LOG_ERROR("SplashExamplePlugin: Could not copy default profile attributes to '%s'.).", m_pCurrentProfile->GetName());
						}
					}
					else
					{
						// If the resolution is set higher than the bare minimum supported then
						// assume this profile was created outside the scope of this plugin.
						// 
						// Add the splashflag to this profile to prevent overwriting the window preoperties later.
						if (!SaveSplashFlagToProfile())
						{
							CRY_LOG_ERROR("SplashExamplePlugin: Could not save splash flag to profile '%s'.", m_pCurrentProfile->GetName());
						}
					}
				}

				m_bLoadedProfileAttributes = true;
			}
			else
			{
				// Could not get current profile, use defaults
				return false;
			}
		}
		else
		{
			// Could not get default profile
			return false;
		}
	}
	else
	{
		// Could not get profile manager
		return false;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////
//! Copies profile attributes from one profile to the other
bool CSplashExample::CopyProfileAttributes(IPlayerProfile * pTo, IPlayerProfile * pFrom)
{
	CRY_LOG_CALL("SplashExamplePlugin: CSplashExample::CopyProfileAttributes()");

	assert(pTo != nullptr);
	assert(pFrom != nullptr);

	if (pTo == pFrom)
		return true;

	if (pTo->IsDefault())
		return false;

	auto pAttrEnumerator = pFrom->CreateAttributeEnumerator();
	IAttributeEnumerator::SAttributeDescription desc;
	TFlowInputData value;
	bool ok = true;
	while (pAttrEnumerator->Next(desc))
	{
		pFrom->GetAttribute(desc.name, value);
		if (!pTo->SetAttribute(desc.name, value))
		{
			CRY_LOG_ERROR("SplashExamplePlugin: Could not set default attribute to current profile.");
			ok = false;
		}
	}
	return ok;
}

///////////////////////////////////////////////////////////////////////////
//! Sets the splash flag attribute on the current profile
bool CSplashExample::SetProfileAttribute(const char * attributeName, const TFlowInputData attributeValue)
{
	CRY_LOG_CALL("SplashExamplePlugin: CSplashExample::SetProfileAttribute()");
	return m_pCurrentProfile->SetAttribute(attributeName, attributeValue);
}
template <class T>
bool CSplashExample::SetProfileAttribute(const char * attributeName, const T& attributeValue)
{
	CRY_LOG_CALL("SplashExamplePlugin: CSplashExample::SetProfileAttribute<T>()");
	return m_pCurrentProfile->SetAttribute(attributeName, attributeValue);
}

///////////////////////////////////////////////////////////////////////////
//! Sets the splash flag attribute & manually saves the profile
bool CSplashExample::SaveSplashFlagToProfile()
{
	CRY_LOG_CALL("SplashExamplePlugin: CSplashExample::SaveSplashFlagToProfile()");

	if (!SetProfileAttribute("SplashFlag_FirstRun", 0))
		return false;

	EPOResult res;
	if (!SavePlayerProfile(res, EProfileReasons::ePR_Options))
		return false;

	return true;
}

///////////////////////////////////////////////////////////////////////////
//! Manually saves the current profile
bool CSplashExample::SavePlayerProfile(EPOResult &result, EProfileReasons reason)
{
	CRY_LOG_CALL("SplashExamplePlugin: CSplashExample::SavePlayerProfile()");

	assert(m_pProfileManager != nullptr);
	assert(m_pCurrentProfile != nullptr);

	m_bIgnoreNextProfileSave = true;

	if (m_pProfileManager->SaveProfile(m_pCurrentProfile->GetName(), result, reason))
	{
		return true;
	}
	else
	{
		m_bIgnoreNextProfileSave = false;
		return false;
	}
}

///////////////////////////////////////////////////////////////////////////
//! Registers the player profile listener if not registered
bool CSplashExample::RegisterProfileListener(IPlayerProfileListener * pListener)
{
	CRY_LOG_CALL("SplashExamplePlugin: CSplashExample::RegisterProfileListener()");
	assert(pListener);

	if (!m_bProfileListenerRegistered)
	{
		auto pMan = GetIPlayerProfileManager();
		if (pMan)
		{
			pMan->AddListener(pListener, false);
			m_bProfileListenerRegistered = true;
			CRY_LOG_DEBUG("SplashExamplePlugin: Registered Profile Manager Listener.");
		}
		else
		{
			CRY_LOG_DEBUG("SplashExamplePlugin: Profile Manager not available, could not register listener.");
		}
	}

	return m_bProfileListenerRegistered;
}

///////////////////////////////////////////////////////////////////////////
//! De-Registers the player profile listener
bool CSplashExample::UnregisterProfileListener(IPlayerProfileListener * pListener)
{
	CRY_LOG_CALL("SplashExamplePlugin: CSplashExample::UnregisterProfileListener()");
	assert(pListener);

	if (m_bProfileListenerRegistered)
	{
		auto pMan = GetIPlayerProfileManager();
		if (pMan)
		{
			pMan->RemoveListener(pListener);
			m_bProfileListenerRegistered = false;
			CRY_LOG_DEBUG("SplashExamplePlugin: Unregistered Profile Manager Listener.");
		}
		else
		{
			CRY_LOG_ERROR("SplashExamplePlugin: Profile manager destructed before removing listener.");
		}
	}

	return !m_bProfileListenerRegistered;
}

///////////////////////////////////////////////////////////////////////////
//! Registers the system event listener if not registered
bool CSplashExample::RegisterSystemListener(ISystemEventListener * pListener)
{
	CRY_LOG_CALL("SplashExamplePlugin: CSplashExample::RegisterSystemListener()");
	assert(pListener);

	if (!m_bSystemListenerRegistered)
	{
		if (gEnv->pSystem)
		{
			auto pDispatcher = gEnv->pSystem->GetISystemEventDispatcher();
			if (pDispatcher)
			{
				pDispatcher->RegisterListener(pListener);
				m_bSystemListenerRegistered = true;
				CRY_LOG_DEBUG("SplashExamplePlugin: Registered System Event Listener.");
			}
			else
			{
				CRY_LOG_DEBUG("SplashExamplePlugin: System Event Dispatcher not available, could not register listener.");
			}
		}
		else
		{
			CRY_LOG_DEBUG("SplashExamplePlugin: Profile Manager not available, could not register listener.");
		}
	}

	return m_bSystemListenerRegistered;
}

///////////////////////////////////////////////////////////////////////////
//! De-Registers the system event listener
bool CSplashExample::UnregisterSystemListener(ISystemEventListener * pListener)
{
	CRY_LOG_CALL("SplashExamplePlugin: CSplashExample::UnregisterSystemListener()");
	assert(pListener);

	if (m_bSystemListenerRegistered)
	{
		if (gEnv->pSystem)
		{
			auto pDispatcher = gEnv->pSystem->GetISystemEventDispatcher();
			if (pDispatcher)
			{
				pDispatcher->RemoveListener(pListener);
				m_bSystemListenerRegistered = false;
				CRY_LOG_DEBUG("SplashExamplePlugin: Unregistered System Event Listener.");
			}
			else
			{
				CRY_LOG_ERROR("SplashExamplePlugin: System Event Dispatcher destructed before removing listener.");
			}
		}
		else
		{
			CRY_LOG_ERROR("SplashExamplePlugin: System destructed before removing listener.");
		}
	}

	return !m_bSystemListenerRegistered;
}

///////////////////////////////////////////////////////////////////////////
//! Listener: 
void CSplashExample::SaveToProfile(IPlayerProfile* pProfile, bool online, unsigned int /*EProfileReasons*/ reason)
{
	CRY_LOG_CALL("SplashExamplePlugin: CSplashExample::SaveToProfile()");

	if (m_bIgnoreNextProfileSave)
	{
		m_bIgnoreNextProfileSave = false;
		return;
	}

	// Todo: Make sure we have required profile attributes?
	return;
}

///////////////////////////////////////////////////////////////////////////
//! Listener: Ensures loaded profiles have the splash flag.
void CSplashExample::LoadFromProfile(IPlayerProfile* pProfile, bool online, unsigned int /*EProfileReasons*/ reason)
{
	CRY_LOG_CALL("SplashExamplePlugin: CSplashExample::LoadFromProfile()");

	if (!pProfile->IsDefault())
	{
		if (m_pCurrentProfile == nullptr)
			m_pCurrentProfile = pProfile;
		else if (m_pCurrentProfile != pProfile)
			SaveSplashFlagToProfile();
	}
}

///////////////////////////////////////////////////////////////////////////
//! Listener: All splash operations are dispatched from this method
void CSplashExample::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
	// Skip these
	if (event == ESYSTEM_EVENT_CVAR_REGISTERED || event == ESYSTEM_EVENT_CVAR_UNREGISTERED)
		return;

	CRY_LOG_CALL("SplashExamplePlugin: Debug OnSystemEvent. event = %s", SYSEVENT_CODE_TO_TEXT(event));

	// Unlink listeners if we notice a shutdown (CE Registry Destruction Bug)
	if (IsShuttingDown()) return;

	// Construct at the earliest opportunity
	// * This is a safe-guard if the plugins object construction
	// * is moved before other systems have initialized.
	// * (Currently in 5.3.0, Construction, Initialization, and 
	// * ESYSTEM_EVENT_PRE_RENDERER_INIT are called at the same 
	// * time in CSystem::Init)

	switch (event)
	{
	// First stage, override startscreen, hide cursor and draw our gui splash (splash_a)
	case ESYSTEM_EVENT_PRE_RENDERER_INIT:
	{
		if (!m_bInitialized) return; // Only respond to events if we are ready to

		// Initial splash
		DisplayInitialSplash();

		break;
	}

	// Second stage, stall with our initial splash for the specified time
	case ESYSTEM_EVENT_GAME_POST_INIT:
	{
		// Stall if needed
		DisplayInitialSplash(true);

		break;
	}

	// Third stage, Render main splash image, and delay further loading for the specified time.
	case ESYSTEM_EVENT_GAME_POST_INIT_DONE:
	{
		// Load profile attributes or defaults
		if (!m_bLoadedProfileAttributes)
		TryLoadProfileAttributes();

		// Stall if needed
		DisplayMainSplash(true);
	}
	}
}

bool CSplashExample::IsShuttingDown()
{
	if (gEnv->pSystem->IsQuitting())
	{
		// cleanup immmediately
		UnregisterProfileListener(this);
		UnregisterSystemListener(this);
		return true;
	}
	return false;
}