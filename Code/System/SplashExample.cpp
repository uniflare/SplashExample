#include "StdAfx.h"
#include "SplashExample.h"
#include <CryRenderer/IRenderer.h>

// EntityRegistrator

IEntityRegistrator *IEntityRegistrator::g_pFirst = nullptr;
IEntityRegistrator *IEntityRegistrator::g_pLast = nullptr;

class CSplashExampleRegistrator
	: public IEntityRegistrator
	, public CSplashExample::SExternalCVars
{
	virtual void Register() override
	{
		// CSplashExamplePlugin::RegisterEntityWithDefaultComponent<CSplashExample>("SplashExample");

		RegisterCVars();
	}

	virtual void Unregister() override
	{
		UnregisterCVars();
	}

	void RegisterCVars()
	{
		REGISTER_CVAR2("splash_show", &m_ShowSplashOverlay, 1, VF_CHEAT, "Whether to show the splash screen (0=no)");
		REGISTER_CVAR2("splash_pausegame", &m_PauseGameDuringSplash, 1, VF_CHEAT, "Pause the game if playing (0=no)");
		REGISTER_CVAR2("splash_minimumPlaybackTime", &m_MinPlaybackSec, 5.f, VF_CHEAT, "Minimum playback time in seconds (float min 0.f)");
		m_pOverlayTexturePath = REGISTER_STRING("splash_texture", "SplashExample/textures/splash.dds", VF_CHEAT, "Sets the splash overlay texture to load");
		//m_pOverlayTexturePath = REGISTER_STRING("splash_texture", "Libs/ui/textures/startscreen.dds", VF_CHEAT, "Sets the splash overlay texture to load");
	}

	void UnregisterCVars()
	{
		IConsole* pConsole = gEnv->pConsole;
		if (pConsole)
		{
			pConsole->UnregisterVariable("splash_texture");
			pConsole->UnregisterVariable("splash_minimumPlaybackTime");
			pConsole->UnregisterVariable("splash_pausegame");
			pConsole->UnregisterVariable("splash_show");
		}
	}
};
CSplashExampleRegistrator g_splashExampleRegistrator;
// ~EntityRegistrator

// CSplashExample
///////////////////////////////////////////////////////////////////////////
// Constructor
CSplashExample::CSplashExample()
	: m_pOverlayTexture(nullptr)
	, m_bDrawSplash(false)
	, m_PlaybackStartTime(CTimeValue::CTimeValue(int64(0)))
	/*
	, m_bShowSplash(true)
	, m_bPlayingSplash(false)
	, m_fMinimumPlaybackTime(0.f) // in seconds
	, m_bStopRequested(false)
	, m_bPausedForPlayback(false)
	, m_pSystemListener(nullptr)
	, m_pFrameworkListener(nullptr)
	, m_vCallbackList(0)
	, m_tThreadList()*/
{
	/*
	SplashExampleDebugMsgV("CSplashExample::CSplashExample(\n\
	m_bDoIntroMovies = %s\n\
	m_bPlayingVideo = %s\n\
	m_fMinimumPlaybackTime = %s\n\
	m_bStopRequested = %s\n\
	m_bPausedForVideo = %s\n\
	m_bSysEventListener = %s\n)"
		, CryStringUtils::toString(m_bShowSplash)
		, CryStringUtils::toString(m_bPlayingSplash)
		, CryStringUtils::toString(m_fMinimumPlaybackTime)
		, CryStringUtils::toString(m_bStopRequested)
		, CryStringUtils::toString(m_bPausedForPlayback)
		, CFVDEBUGUTIL::POINTER_TO_HEXADDR((uintptr_t)m_pSystemListener)
	);*/

	// Make sure we have a self reference
	m_pThis = this;

	// Register a system event listener (us)
	GetISystem()->GetISystemEventDispatcher()->RegisterListener(m_pThis);
}

const CSplashExample::SExternalCVars &CSplashExample::GetCVars() const
{
	return g_splashExampleRegistrator;
}

///////////////////////////////////////////////////////////////////////////
// Destructor
CSplashExample::~CSplashExample()
{
	SplashExampleDebugMsg("CSplashExample::~CSplashExample()");

	// Remove us from the event dispatcher system
	GetISystem()->GetISystemEventDispatcher()->RemoveListener(m_pThis);

	// Unregister recorded entites
	IEntityRegistrator* pTemp = IEntityRegistrator::g_pFirst;
	while (pTemp != nullptr)
	{
		pTemp->Unregister();
		pTemp = pTemp->m_pNext;
	}
}
// ~CSplashExample

// IEntityComponent
///////////////////////////////////////////////////////////////////////////
bool CSplashExample::Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams)
{
	SplashExampleDebugMsgV("CSplashExample::Initialize(env = %s, initParams = %s)"
		, CFVDEBUGUTIL::POINTER_TO_HEXADDR((uintptr_t)&env)
		, CFVDEBUGUTIL::POINTER_TO_HEXADDR((uintptr_t)&initParams));

	return true;
}
// ~IEntityComponent


void CSplashExample::BeginPlayback()
{

}
void CSplashExample::StopPlayback(bool force)
{

}
void CSplashExample::SetOverlayTexture(string path)
{
	// Todo: Check if texture exists

	if (m_pOverlayTexture)
		m_pOverlayTexture->Release();
	
	auto pTexture = gEnv->pRenderer->EF_LoadTexture(path, FT_DONT_RELEASE | FT_DONT_STREAM | FT_NOMIPS);

	if (pTexture)
		m_pOverlayTexture = pTexture;
}

void CSplashExample::UnregisterFrameworkListener()
{
	static bool unregistered = false;
	if(!unregistered)
		gEnv->pGameFramework->UnregisterListener(m_pThis);
	unregistered = true;
}

// IGameFrameworkListener
void CSplashExample::OnPostUpdate(float fDeltaTime)
{
	// Don't let the system hang
	if (gEnv->pSystem->IsQuitting())
	{
		// Unpause if necessary
		if(gEnv->pGameFramework->IsGamePaused() && GetCVars().m_PauseGameDuringSplash > 0)
			gEnv->pGameFramework->PauseGame(false, true);

		UnregisterFrameworkListener();
	}

	// Check if we should be drawing at all
	if (GetCVars().m_ShowSplashOverlay != 0 && m_bDrawSplash)
	{
		// Make sure we have a valid start time set
		if (m_PlaybackStartTime == CTimeValue::CTimeValue(int64(0)))
		{
			m_PlaybackStartTime = gEnv->pTimer->GetAsyncTime();

			// Make sure gameplay is paused while we show our video
			if(GetCVars().m_PauseGameDuringSplash > 0)
				gEnv->pGameFramework->PauseGame(true, true);
		}

		// Check we havn't elapsed our playback time
		if (gEnv->pTimer->GetAsyncTime() - GetCVars().m_MinPlaybackSec > m_PlaybackStartTime)
		{
			m_bDrawSplash = false;
			m_PlaybackStartTime = CTimeValue::CTimeValue(int64(0));

			// Unpause if necessary
			if (gEnv->pGameFramework->IsGamePaused() && GetCVars().m_PauseGameDuringSplash > 0)
				gEnv->pGameFramework->PauseGame(false, true);

			return;
		}

		// Check CVar every frame
		if (m_OverlayTexturePath != GetCVars().m_pOverlayTexturePath->GetString())
		{
			// Todo: Check file exists

			m_OverlayTexturePath = GetCVars().m_pOverlayTexturePath->GetString();
			SetOverlayTexture(m_OverlayTexturePath);
		}

		if (m_pOverlayTexture)
		{
			const int splashWidth = m_pOverlayTexture->GetWidth();
			const int splashHeight = m_pOverlayTexture->GetHeight();

			const int screenWidth = gEnv->pRenderer->GetOverlayWidth();
			const int screenHeight = gEnv->pRenderer->GetOverlayHeight();

			if (splashWidth > 0 && splashHeight > 0 && screenWidth > 0 && screenHeight > 0)
			{
				const float scaleX = (float)screenWidth / (float)splashWidth;
				const float scaleY = (float)screenHeight / (float)splashHeight;

				const float scale = (scaleY * splashWidth > screenWidth) ? scaleX : scaleY;

				const float w = splashWidth * scale;
				const float h = splashHeight * scale;
				const float x = (screenWidth - w) * 0.5f;
				const float y = (screenHeight - h) * 0.5f;

				const float vx = (800.0f / (float)screenWidth);
				const float vy = (600.0f / (float)screenHeight);



				// Get Renderer Resolution
				const int &viewWidth = gEnv->pRenderer->GetWidth();
				const int &viewHeight = gEnv->pRenderer->GetHeight();

				// Scale to internal dimensions (800x600?)
				const float &fScaledW = viewWidth / (float(viewWidth) / 800.0f);
				const float &fScaledH = viewHeight / (float(viewHeight) / 600.0f);



				gEnv->pRenderer->SetViewport(0, 0, screenWidth, screenHeight);
				// make sure it's rendered in full screen mode when triple buffering is enabled as well
				for (size_t n = 0; n < 3; n++)
				{
					// Begin/End frame don't belong here
					//gEnv->pRenderer->BeginFrame();
					gEnv->pRenderer->SetCullMode(R_CULL_NONE);
					gEnv->pRenderer->SetState(GS_BLSRC_SRCALPHA | GS_BLDST_ONEMINUSSRCALPHA | GS_NODEPTHTEST);
					gEnv->pRenderer->Draw2dImageStretchMode(true);

					//gEnv->pRenderer->Draw2dImage(x * vx, y * vy, w * vx, h * vy, m_pOverlayTexture->GetTextureID(), 0.0f, 1.0f, 1.0f, 0.0f);

					// Draw image directly
					gEnv->pRenderer->Draw2dImage(0, 0, fScaledW, fScaledH, m_pOverlayTexture->GetTextureID(), 0.0f, 1.0f, 1.0f, 0.0f);

					gEnv->pRenderer->Draw2dImageStretchMode(false);
					//gEnv->pRenderer->EndFrame();
				}
			}
			else
			{
				UnregisterFrameworkListener();
				gEnv->pLog->LogWarning("Invalid splash screen texture");
			}
		}

		// Get Renderer Resolution
		const int &viewWidth = gEnv->pRenderer->GetWidth();
		const int &viewHeight = gEnv->pRenderer->GetHeight();

		// Scale to internal dimensions (800x600?)
		const float &fScaledW = viewWidth / (float(viewWidth) / 800.0f);
		const float &fScaledH = viewHeight / (float(viewHeight) / 600.0f);

		// Draw image directly
		//gEnv->pRenderer->Draw2dImage(0, 0, fScaledW, fScaledH, m_pOverlayTexture->GetTextureID(), 0, 1, 1, 0, 0);
	}
	else
	{
		// Just for sanity
		if (m_PlaybackStartTime != CTimeValue::CTimeValue(int64(0)))
		{
			m_PlaybackStartTime = CTimeValue::CTimeValue(int64(0));
		}
	}
}
// ~IGameFrameworkListener

// ISystemEventListener
///////////////////////////////////////////////////////////////////////////
void CSplashExample::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
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
		
		}
		// Earliest point we should interact with the system
		case ESYSTEM_EVENT_GAME_FRAMEWORK_INIT_DONE:
		{
			// Register our entities 
			// (atm this just attaches some CVars)
			IEntityRegistrator* pTemp = IEntityRegistrator::g_pFirst;
			while (pTemp != nullptr)
			{
				pTemp->Register();
				pTemp = pTemp->m_pNext;
			}

			// Get the splash texture path from cvar value
			if (GetCVars().m_pOverlayTexturePath != nullptr)
			{
				m_OverlayTexturePath = GetCVars().m_pOverlayTexturePath->GetString();
				SetOverlayTexture(m_OverlayTexturePath);
			}

			m_bDrawSplash = true;

			// Have to delay registering a framework listener until framework is fully initialized
			gEnv->pGameFramework->RegisterListener(m_pThis, "SplashExampleListener", FRAMEWORKLISTENERPRIORITY_MENU);
		}
		case ESYSTEM_EVENT_GAME_RESUMED:
		{
			break;
		}
		case ESYSTEM_EVENT_GAME_PAUSED:
		{
			break;
		}
		case ESYSTEM_EVENT_FAST_SHUTDOWN:
		case ESYSTEM_EVENT_FULL_SHUTDOWN:
		{
			UnregisterFrameworkListener();
			m_bDrawSplash = false;
			break;
		}
	}
}
// ~ISystemEventListener




































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