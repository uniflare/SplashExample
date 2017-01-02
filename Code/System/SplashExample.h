#pragma once

#include <Interface/ISplashExample.h>
#include <CrySystem/ISystem.h>
#include <CryGame\IGameFramework.h>
#include <CryThreading/IThreadManager.h>
#include <CryRenderer/ITexture.h>

class CPlugin_SplashExample;
class CFrameworkListener;
class CSystemListener;
//struct TimedFWUnregisterThread;
struct CThreadTimedCallback;

// CSplashExmaple implements
//  Plugin public interface struct
//  Game framework listener struct
//  System event listener struct
//
// Handles main functionality of plugin Splash Example
class CSplashExample 
	: public ISplashExample
	, public IGameFrameworkListener
	, public ISystemEventListener
{
	friend class CSplashExampleRegistrator;

	// Console variables provided by this object
	struct SExternalCVars
	{
		// Minimum playback time in seconds (float min 0.f)
		float m_MinPlaybackSec;

		// Wether to show the splash screen (0=no)
		int m_ShowSplashOverlay;

		// Pause the game if playing (0=no)
		int m_PauseGameDuringSplash;

		// Path the the splash overlay texture
		ICVar * m_pOverlayTexturePath;
	};

	friend class CSystemListener;
	friend class CFrameworkListener;
	friend struct TimedFWUnregisterThread;
	friend class CPlugin_SplashExample;
	friend struct CThreadTimedCallback;

	//! Used to provide a caller with an instance using the ISplashExample interface
	CSplashExample * m_pThis;

	//! The loaded texture we will use to render to the screen
	ITexture * m_pOverlayTexture;

	//! Curreny loaded texture path
	string m_OverlayTexturePath;

	//! Boolean to control whether we draw
	bool m_bDrawSplash;

	//! Start time of splash playback
	CTimeValue m_PlaybackStartTime;

public:


private:

	// IGameFrameworkListener
	
	// We use the IGameFrameworListener for the OnPostUpdate calls, these provide the best time to
	// render additional media to the screen without render ordering problems.
	virtual void OnPostUpdate(float fDeltaTime) override;
	virtual void OnSaveGame(ISaveGame* pSaveGame) override {};
	virtual void OnLoadGame(ILoadGame* pLoadGame) override {};
	virtual void OnLevelEnd(const char* nextLevel) override {};
	virtual void OnActionEvent(const SActionEvent& event) override {};

	// ~IGameFrameworkListener


	// ISystemEventListener
	
	// We use the ISystemEventListener to catch certain events regarding the state of the gameframework so
	// we can attach a listener as early as possible, and detach properly.
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;

	// ~ISystemEventListener

	// Todo: Implement and use the IEntityComponent interface

	// IEntityComponent

	// Initialize() is automatically called when an entity component is added to system using IEntity::AddComponent().
	// Happens after the component has been added to the m_components vector internally.
	//
	// We call it manually for simplicity. (When plugin is initalized)
	bool Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams);
	// ~IEntityComponent

public:

	// ISplashExample

	//! public
	//! Signals the plugin to begin playback immediately
	virtual void BeginPlayback() override;

	//! public
	//! Signals the plugin to stop playback immediately
	virtual void StopPlayback(bool force = false) override;

	//! public
	//! Internally uses EF_LoadTexture()
	virtual void SetOverlayTexture(string path) override;

	void UnregisterFrameworkListener();

	//! public
	//! Used to get the instance for the public interface
	virtual ISplashExample * GetISplashExample() override {
		return this;
	};
	// ~ISplashExample

	CSplashExample();
	~CSplashExample();

	const SExternalCVars &GetCVars() const;



	/*
protected:
	void DestroyListeners();
	void DestroyThreads();
	bool Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams);
	void OnGamePaused();
	void OnGameResumed();
	void SendShutdownSignalTS();
	void Shutdown(bool force);

private:
	void CallbackStopVideo(int64 id);
	void OnPostUpdate(float fDeltaTime);
	void SystemEventHandler(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam);
	bool CanStopVideo();
	ITexture * m_pOverlayTexture;

	std::vector<CThreadTimedCallback*> m_vCallbackList;

	CSplashExample * m_pThis;
	CSystemListener * m_pSystemListener;
	CFrameworkListener * m_pFrameworkListener;

	std::unordered_map<IThread *, volatile int *> m_tThreadList;

	float m_fMinimumPlaybackTime;
	float m_fTimePlaybackStart;

	bool m_bShowSplash;
	bool m_bPlayingSplash;
	bool m_bStopRequested;
	bool m_bPausedForPlayback;
	*/
};

// EntityRegistrator
struct IEntityRegistrator
{
	IEntityRegistrator()
	{
		if (g_pFirst == nullptr)
		{
			g_pFirst = this;
			g_pLast = this;
		}
		else
		{
			g_pLast->m_pNext = this;
			g_pLast = g_pLast->m_pNext;
		}
	}

	virtual void Register() = 0;
	virtual void Unregister() = 0;

public:
	IEntityRegistrator *m_pNext;

	static IEntityRegistrator *g_pFirst;
	static IEntityRegistrator *g_pLast;
};
// ~EntityRegistrator