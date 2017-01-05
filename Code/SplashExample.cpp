// 05/01/2017 by Uniflare.

#include "StdAfx.h"
#include "SplashExample.h"

#include <CryRenderer/IRenderer.h>
#include <CryInput/IInput.h>
#include <CryInput/IHardwareMouse.h>
#include <CryGame/IGameFramework.h>
#include <IPlayerProfiles.h>

///////////////////////////////////////////////////////////////////////////
CSplashExample::CSplashExample()
	: m_bListenerRegistered(false)
	, m_pSplashTextureA(nullptr)
	, m_pSplashTexture(nullptr)
{
	// Attempt to load our textures
	if (gEnv->pCryPak->IsFileExist(gEnv->pConsole->GetCVar("splash_texture_a")->GetString()))
		m_pSplashTextureA = gEnv->pRenderer->EF_LoadTexture(gEnv->pConsole->GetCVar("splash_texture_a")->GetString(), FT_DONT_STREAM | FT_NOMIPS);

	if (gEnv->pCryPak->IsFileExist(gEnv->pConsole->GetCVar("splash_texture")->GetString()))
		m_pSplashTexture = gEnv->pRenderer->EF_LoadTexture(gEnv->pConsole->GetCVar("splash_texture")->GetString(), FT_DONT_STREAM | FT_NOMIPS);

	// Register as a system event listener
	RegisterListener(true);
}

///////////////////////////////////////////////////////////////////////////
CSplashExample::~CSplashExample()
{
	// Release if necessary
	if (m_pSplashTextureA)
		m_pSplashTextureA->Release();

	if (m_pSplashTexture)
		m_pSplashTexture->Release();

	m_pSplashTexture = m_pSplashTextureA = nullptr;

	// Remove us from the event dispatcher system
	RegisterListener(false);
}

///////////////////////////////////////////////////////////////////////////
//! Draws the supplied texture in stretched mode to the main viewport
void CSplashExample::Draw2DImageScaled(const ITexture * tex) const
{
	if (tex)
	{
		const int splashWidth = tex->GetWidth();
		const int splashHeight = tex->GetHeight();

		const int screenWidth = gEnv->pRenderer->GetOverlayWidth();
		const int screenHeight = gEnv->pRenderer->GetOverlayHeight();

		if (splashWidth > 0 && splashHeight > 0 && screenWidth > 0 && screenHeight > 0)
		{
			const float &fScaledW = screenWidth / (float(screenWidth) / 800.0f);
			const float &fScaledH = screenHeight / (float(screenHeight) / 600.0f);

			gEnv->pRenderer->SetViewport(0, 0, screenWidth, screenHeight);

			// make sure it's rendered in full screen mode when triple buffering is enabled as well
			for (size_t n = 0; n < 3; n++)
			{
				gEnv->pRenderer->SetCullMode(R_CULL_NONE);
				gEnv->pRenderer->SetState(GS_BLSRC_SRCALPHA | GS_BLDST_ONEMINUSSRCALPHA | GS_NODEPTHTEST);
				gEnv->pRenderer->Draw2dImageStretchMode(true);
				gEnv->pRenderer->Draw2dImage(0, 0, fScaledW, fScaledH, tex->GetTextureID(), 0.0f, 1.0f, 1.0f, 0.0f);
				gEnv->pRenderer->Draw2dImageStretchMode(false);
			}
		}
	}
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
	auto ScreenResolutions = GetScreenResolutions();

#ifndef _RELEASE
	CRY_ASSERT_MESSAGE(idx < ScreenResolutions.size(), "SplashExamplePlugin: idx is higher than supported, defaulting to highest available.");
	CRY_ASSERT_MESSAGE(idx >= 0, "SplashExamplePlugin: GetScreenResolution() failed. idx was negative.");
	CRY_ASSERT_MESSAGE(ScreenResolutions.size() > 0, "SplashExamplePlugin: Could not get available screen resolutions for display.");
#endif

	if (ScreenResolutions.size() <= 0)
		return SScreenResolution();

	// If we don't have a resolution this high, get the highest available
	if (idx >= ScreenResolutions.size() || idx < 0)
		return ScreenResolutions[ScreenResolutions.size() - 1];

	return ScreenResolutions[idx];
}

//////////////////////////////////////////////////////////////////////////
//! Generates a list of supported screen resolutions
const std::vector<CSplashExample::SScreenResolution> CSplashExample::GetScreenResolutions() const
{
	std::vector<SScreenResolution> s_ScreenResolutions;

#if CRY_PLATFORM_DESKTOP

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

		for (auto &res : s_ScreenResolutions)
			if (res.iWidth == formats[i].m_Width && res.iHeight == formats[i].m_Height)
				bAlreadyExists = true;

		if (bAlreadyExists || formats[i].m_Width < 800)
			continue;

		format.Format("%i X %i", formats[i].m_Width, formats[i].m_Height);

		s_ScreenResolutions.emplace_back(formats[i].m_Width, formats[i].m_Height, formats[i].m_BPP, format.c_str());
	}

	if (formats)
		delete[] formats;

#endif

	return s_ScreenResolutions;
}

///////////////////////////////////////////////////////////////////////////
//! Handles our event listener (makes sure we only set/remove once each way)
void CSplashExample::RegisterListener(const bool bRegister) {
	if (bRegister && !m_bListenerRegistered)
		m_bListenerRegistered = GetISystem()->GetISystemEventDispatcher()->RegisterListener(this);
	else if (!bRegister && m_bListenerRegistered)
		m_bListenerRegistered = !GetISystem()->GetISystemEventDispatcher()->RemoveListener(this);
}

///////////////////////////////////////////////////////////////////////////
//! Sets the r_width and r_height CVars if they are set properly in sResolution
void CSplashExample::SetResolutionCVars(const SScreenResolution sResolution) const
{
	if (sResolution.iWidth > 0 && sResolution.iHeight > 0)
	{
		gEnv->pConsole->GetCVar("r_width")->Set(sResolution.iWidth);
		gEnv->pConsole->GetCVar("r_height")->Set(sResolution.iHeight);
	}
}

///////////////////////////////////////////////////////////////////////////
void CSplashExample::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
	if (event == ESYSTEM_EVENT_CVAR_REGISTERED || event == ESYSTEM_EVENT_CVAR_UNREGISTERED)
		return;

	switch (event)
	{
		// First stage, override startscreen, hide cursor and draw our gui splash (splash_a)
		case ESYSTEM_EVENT_PRE_RENDERER_INIT:
		{
#ifndef _RELEASE
			CRY_ASSERT_MESSAGE(
				m_pSplashTextureA
				&& gEnv->pConsole->GetCVar("sys_rendersplashscreen"),
				"SplashExamplePlugin: Expected defaults for 'splash_texture_a' and 'sys_rendersplashscreen'."
			);
#endif
			// Check if this game has startscreen enabled.
			if (gEnv->pConsole->GetCVar("sys_rendersplashscreen")->GetIVal() == 1)
				gEnv->pConsole->GetCVar("sys_rendersplashscreen")->Set(0); // override

			// Remove cursor immediately
			gEnv->pSystem->GetIHardwareMouse()->Hide(true);

			// Draw our intial splash to window
			if (m_pSplashTextureA) {
				gEnv->pRenderer->BeginFrame();
				Draw2DImageScaled(m_pSplashTextureA);
				gEnv->pRenderer->EndFrame();
				m_pSplashTextureA->Release();
			}
			else
			{
				CryWarning(EValidatorModule::VALIDATOR_MODULE_GAME, EValidatorSeverity::VALIDATOR_ERROR, "SplashExamplePlugin: Missing texture '%s'", gEnv->pConsole->GetCVar("splash_texture_a")->GetString());
			}

			m_pSplashTextureA = nullptr;

			break;
		}

		// Second stage, switch resolution, toggle fullscreen, start our main splash render
		case ESYSTEM_EVENT_GAME_POST_INIT:
		{
#ifndef _RELEASE
			CRY_ASSERT_MESSAGE(
				m_pSplashTexture
				&& gEnv->pConsole->GetCVar("r_height") 
				&& gEnv->pConsole->GetCVar("r_width") 
				&& gEnv->pConsole->GetCVar("r_fullscreen"), 
					"SplashExamplePlugin: Expected defaults for 'r_height', 'r_width', 'r_fullscreen' and 'splash_texture'."
			);
#endif

			// Default to CVar values (system.cfg or game.cfg or user.cfg)
			bool bFullscreen = (gEnv->pConsole->GetCVar("r_fullscreen")->GetIVal() > 0)? true : false;
			SScreenResolution ScreenResolution(gEnv->pConsole->GetCVar("r_width")->GetIVal(), gEnv->pConsole->GetCVar("r_height")->GetIVal(), 32, "DefaultResolution");

			// GAMESDK/PROFILE SUPPORT //
			auto pProfile = GetCurrentPlayerProfile();

			// We need to set these cvars before we FlushRTCommands since they won't be set yet.
			if (auto pProfile = GetCurrentPlayerProfile())
			{
				int userFS = GetAttributeIValue("Fullscreen", pProfile);
				if (userFS != -1)
				{
					CryLogAlways("SplashExamplePlugin: Setting Fullscreen CVar to profile setting ('%s')", CryStringUtils::toString(userFS));
					gEnv->pConsole->ExecuteString("r_fullscreen " + CryStringUtils::toString(userFS));
				}

				int userRes = GetAttributeIValue("Resolution", pProfile);
				auto res = GetScreenResolution(userRes);
				CryLogAlways("SplashExamplePlugin: Settings resolution from profile setting to '%s', index '%s'", res.sResolution, CryStringUtils::toString(userRes));
				SetResolutionCVars(res);
			}
			else
			{
				CryLogAlways("SplashExamplePlugin: No default profile loaded during GAME_POST_INIT. Defaulting to CVars.");
			}
			// ~GAMESDK/PROFILE SUPPORT //

			//This will set the window to fullscreen for us
			gEnv->pRenderer->FlushRTCommands(false, true, true);

			// Render the splash image (Drawing here helps with playback time accuracy)
			if (m_pSplashTexture)
			{
				gEnv->pRenderer->BeginFrame();
				Draw2DImageScaled(m_pSplashTexture); // Our overlay texture (can have alpha which is why we need background color)
				gEnv->pRenderer->EndFrame();
			}
			else
			{
				CryWarning(EValidatorModule::VALIDATOR_MODULE_GAME, EValidatorSeverity::VALIDATOR_ERROR, "SplashExamplePlugin: Missing texture '%s'", gEnv->pConsole->GetCVar("splash_texture")->GetString());
			}

			break;
		}

		// Third stage, Keep rendering splash image, and delay further loading until minimum playback time is achieved.
		case ESYSTEM_EVENT_GAME_POST_INIT_DONE:
		{
#ifndef _RELEASE
			CRY_ASSERT_MESSAGE(
				m_pSplashTexture
				&& gEnv->pConsole->GetCVar("splash_startTimeOffset")
				&& gEnv->pConsole->GetCVar("splash_minimumPlaybackTime"),
				"SplashExamplePlugin: Expected defaults for 'splash_startTimeOffset', 'splash_minimumPlaybackTime' and 'splash_texture'."
			);
#endif

			// Set the start time for render, note this is not an accurate stamp 
			// for when the image is actually rendered to the screen so we apply 
			// an additional offset.
			auto PlaybackStartTime = gEnv->pTimer->GetAsyncCurTime() + gEnv->pConsole->GetCVar("splash_startTimeOffset")->GetFVal();

			// Get the minimum playback time
			auto MinPlaybackSec = gEnv->pConsole->GetCVar("splash_minimumPlaybackTime")->GetFVal();

			if (m_pSplashTexture)
			{
				// Stall the engine if we havn't shown our splash for enough time!
				CryLogAlways("SplashExamplePlugin: Stalling thread for splash");

				CSimpleThreadBackOff backoff;
				while (gEnv->pTimer->GetAsyncCurTime() - MinPlaybackSec <= PlaybackStartTime) {

					// Make sure we don't stall on app exit
					if (gEnv->pSystem->IsQuitting())
						break;

					// Make sure windows doesn't think our application has crashed
					if (gEnv->pSystem->PumpWindowMessage(false) == -1) break;

					// Update input events so we don't get any windows cursors during our splash
					gEnv->pInput->Update(true);
					gEnv->pSystem->GetIHardwareMouse()->Update();

					// Render the splash image
					gEnv->pRenderer->BeginFrame();
					Draw2DImageScaled(m_pSplashTexture); // Our overlay texture (can have alpha which is why we need background color)
					gEnv->pConsole->Draw(); // Allow drawing of console while we stall
					gEnv->pRenderer->EndFrame();

					// Give the system some breathing space
					backoff.backoff();
					continue;
				};

				// Release if necessary
				if (m_pSplashTexture)
					m_pSplashTexture->Release();
			}
			else
			{
				CryWarning(EValidatorModule::VALIDATOR_MODULE_GAME, EValidatorSeverity::VALIDATOR_ERROR, "SplashExamplePlugin: Missing texture '%s'", gEnv->pConsole->GetCVar("splash_texture")->GetString());
			}

			// Re-enable cursor
			gEnv->pSystem->GetIHardwareMouse()->Hide(false);

			// We're done, de-register our system event handler
			RegisterListener(false);

			m_pSplashTexture = nullptr;
		}
	}
}



































/*

typedef  void(CSplashExample::*TimedCallbackFunc)(int64 i64Id);
//! Simple thread object used to execute supplied callback function at specified time
struct CThreadTimedCallback : public IThread
{
	//! Static volatile int for signalling
	volatile int * m_vLifeTime;

	//! An identifier given to this thread
	int64 m_i64Id;

	//! Callback Method Context
	CSplashExample * m_pSplashExample;

	//! Callback Method Pointer
	TimedCallbackFunc m_pCallback;

	//! Async time the timer should tigger
	float m_fAsyncTimeUntil;

	~CThreadTimedCallback()
	{
		// set the shared int to a number telling the caller this instance is gone
		CryInterlockedExchange(m_vLifeTime, 999);
	}

	CThreadTimedCallback(int64 id, CSplashExample * pSplashExample, TimedCallbackFunc pCallback, float fAsyncTimeUntil, volatile int * vLifeTime)
		: m_i64Id(id)
		, m_pSplashExample(pSplashExample)
		, m_pCallback(pCallback)
		, m_vLifeTime(vLifeTime)
		, m_fAsyncTimeUntil(fAsyncTimeUntil)
	{
		SplashExampleDebugMsgV("CThreadTimedCallback::ThreadTimedCallback(\n\
	id = %s\n\
	pSplashExample = %s\n\
	pCallback = %s\n\
	)"
			, CFVDEBUGUTIL::TO_HEX64(id)
			, CFVDEBUGUTIL::TO_HEX64((uintptr_t)pSplashExample)
			, CFVDEBUGUTIL::TO_HEX64(pCallback)
		);
	}

	virtual void ThreadEntry() override
	{
		SplashExampleDebugMsgV("CThreadTimedCallback::ThreadEntry()::BEGIN (%s)", CFVDEBUGUTIL::TO_HEX64(m_i64Id));

		if (!m_pSplashExample)
			return;

		static_assert(sizeof(int) == sizeof(LONG), "Unsecured cast. Int is not same size as LONG.");
		CSimpleThreadBackOff threadBackoff;

		while (true)
		{
			if (CryInterlockedExchange((volatile LONG*)m_vLifeTime, 1) >= 1)
				break;

			if (gEnv->pTimer->GetAsyncCurTime() >= m_fAsyncTimeUntil)
			{
				CALL_MEMBER_FN(*m_pSplashExample, m_pCallback)(m_i64Id);
				break;
			}
			threadBackoff.backoff();
		}

		SplashExampleDebugMsgV("CThreadTimedCallback::ThreadEntry()::Exit (%s)", CFVDEBUGUTIL::TO_HEX64(m_i64Id));
	}
};

///////////////////////////////////////////////////////////////////////////
bool CSplashExample::Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams)
{
	SplashExampleDebugMsgV("CSplashExample::Initialize(env = %s, initParams = %s)"
		, CFVDEBUGUTIL::POINTER_TO_HEXADDR((uintptr_t)&env)
		, CFVDEBUGUTIL::POINTER_TO_HEXADDR((uintptr_t)&initParams));

	// Have to delay registering a framework listener until framework is fully initialized
	m_pFrameworkListener = new CFrameworkListener(this);

	return (m_pFrameworkListener != nullptr);
}

///////////////////////////////////////////////////////////////////////////
void CSplashExample::BeginPlayback()
{
	SplashExampleDebugMsg("CSplashExample::SplashExampleStart()");
	// Busy (Queue requests here?)
	if (m_bPlayingSplash)
		return;

	// RenderListener will begin rendering immediately
	if (m_pFrameworkListener != nullptr)
	{
		m_pFrameworkListener->Start();
		m_fTimePlaybackStart = gEnv->pTimer->GetAsyncCurTime();
		m_bStopRequested = false;
		m_bPlayingSplash = true;
	}
}

///////////////////////////////////////////////////////////////////////////
bool CSplashExample::CanStopVideo()
{
	// Only for main thread (don't want to stall)
	if(CryGetCurrentThreadId() == gEnv->mMainThreadId)
		SplashExampleDebugMsg("CSplashExample::CanStopVideo()");

	return (gEnv->pTimer->GetAsyncCurTime() - m_fMinimumPlaybackTime > m_fTimePlaybackStart);
}

///////////////////////////////////////////////////////////////////////////
void CSplashExample::StopPlayback(bool force)
{
	SplashExampleDebugMsg("CSplashExample::SplashExampleEnd()");

	// Make sure we play for our minimum length of time
	if (CanStopVideo() || force)
	{
		// Ok we can skip
		if (m_bPlayingSplash)
			m_bPlayingSplash = false;

		m_pFrameworkListener->Stop();

		// Unpause the game if we paused it
		if (m_bPausedForPlayback)
			gEnv->pGameFramework->PauseGame(false, true);

		m_bStopRequested = false;
	}
	else
	{
		if (m_bStopRequested)
			return;

		static int64 i64NextId = 0;
		++i64NextId;

		volatile int * vp = new volatile int(0);

		// Let's set up a timer so we don't get bothered again
		auto pThread = new CThreadTimedCallback(i64NextId, m_pThis, &CSplashExample::CallbackStopVideo, m_fTimePlaybackStart + m_fMinimumPlaybackTime, vp);

		// Add to thread list
		m_tThreadList[pThread] = vp;
		
		if (gEnv->pThreadManager->SpawnThread(pThread, "CThreadTimedCallback"))
		{
			m_vCallbackList.push_back(pThread);
		}

		m_bStopRequested = true;
	}
}

void CSplashExample::SetOverlayTexture(const char * path)
{
	if (m_pOverlayTexture)
		m_pOverlayTexture->Release();

	auto pTexture = gEnv->pRenderer->EF_LoadTexture(path, FT_DONT_RELEASE | FT_DONT_STREAM | FT_FILESINGLE);

	if (pTexture)
		m_pOverlayTexture = pTexture;
}

void CSplashExample::CallbackStopVideo(int64 id)
{
	StopPlayback(true);
}

///////////////////////////////////////////////////////////////////////////
void CSplashExample::OnPostUpdate(float fDeltaTime)
{
	if (m_bPlayingSplash)
	{
		// Get Renderer Resolution
		const int &viewWidth = gEnv->pRenderer->GetWidth();
		const int &viewHeight = gEnv->pRenderer->GetHeight();

		// Scale to internal dimensions (800x600?)
		const float &fScaledW = viewWidth / (float(viewWidth) / 800.0f);
		const float &fScaledH = viewHeight / (float(viewHeight) / 600.0f);

		// Draw image directly
		gEnv->pRenderer->Draw2dImage(0, 0, fScaledW, fScaledH, m_pOverlayTexture->GetTextureID(), 0, 1, 1, 0, 0);
	}
}

///////////////////////////////////////////////////////////////////////////
void CSplashExample::SystemEventHandler(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
	SplashExampleDebugMsgV("CSplashExample::SystemEventHandler(event = %s, wparam = %s, lparam = %s)"
		, CFVDEBUGUTIL::EVENT_TO_STRING(event)
		, CFVDEBUGUTIL::POINTER_TO_HEXADDR(wparam)
		, CFVDEBUGUTIL::POINTER_TO_HEXADDR(lparam)
	);

	switch (event)
	{
	case ESYSTEM_EVENT_REGISTER_FLOWNODES:
	{
		if (m_bShowSplash)
			BeginPlayback();
		break;
	}
	// Earliest point we should allow skipping any remaining intro video
	case ESYSTEM_EVENT_GAME_FRAMEWORK_INIT_DONE:
	{

		if (m_bShowSplash)
			StopPlayback();
		break;
	}
	case ESYSTEM_EVENT_GAME_RESUMED:
	{
		OnGameResumed();
		break;
	}
	case ESYSTEM_EVENT_GAME_PAUSED:
	{
		OnGamePaused();
		break;
	}
	case ESYSTEM_EVENT_FAST_SHUTDOWN:
	case ESYSTEM_EVENT_FULL_SHUTDOWN:
	{
		Shutdown(true);
		break;
	}
	}
}

///////////////////////////////////////////////////////////////////////////
void CSplashExample::OnGamePaused()
{
	SplashExampleDebugMsg("CSplashExample::OnGamePaused()");
}

///////////////////////////////////////////////////////////////////////////
void CSplashExample::OnGameResumed()
{
	SplashExampleDebugMsg("CSplashExample::OnGameResumed()");

	m_bPausedForPlayback = false;

	// We're not finished here
	if (!CanStopVideo())
	{
		gEnv->pGameFramework->PauseGame(true, false);
		m_bPausedForPlayback = true;
	}
}

///////////////////////////////////////////////////////////////////////////
void CSplashExample::Shutdown(bool force)
{
	SplashExampleDebugMsgV("CSplashExample::Shutdown(force = %s)",
		CryStringUtils::toString(force));

	DestroyThreads();
	DestroyListeners();
}

///////////////////////////////////////////////////////////////////////////
void CSplashExample::DestroyListeners()
{
	SplashExampleDebugMsg("CSplashExample::DestroyListeners()");

	// Destroy dat ting, safely!
	SAFE_DELETE(m_pFrameworkListener)
		m_bPlayingSplash = false;

	SAFE_DELETE(m_pSystemListener)

	for (auto v : m_vCallbackList)
		SAFE_DELETE(v)
}

///////////////////////////////////////////////////////////////////////////
void CSplashExample::DestroyThreads()
{
	SplashExampleDebugMsg("CSplashExample::DestroyThreads()");

	// Signal any executing threads to stop imediately
	SendShutdownSignalTS();

	for (auto&t : m_tThreadList)
	{
		if (CryInterlockedExchange((volatile LONG*)&t.second, 999) == 999)
		{
			m_tThreadList.erase(t.first);
		}
		else
		{
			GetISystem()->GetIThreadManager()->JoinThread(t.first, EJoinMode::eJM_TryJoin);
			m_tThreadList.erase(t.first);
		}
	}
}

///////////////////////////////////////////////////////////////////////////
void CSplashExample::SendShutdownSignalTS()
{
	SplashExampleDebugMsg("CSplashExample::SendShutdownSignalTS()");

	// Signal all timer threads
	for (auto& t : m_tThreadList)
		CryInterlockedCompareExchange((volatile LONG*)&t.second, 1, 0);

}*/