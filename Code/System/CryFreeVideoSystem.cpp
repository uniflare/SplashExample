#include "StdAfx.h"
#include "CryFreeVideoSystem.h"
#include <CryGame/IGameFramework.h>
const char * SystemEventIdToName(ESystemEvent event)
{
	switch (event)
	{

		case ESYSTEM_EVENT_MOVE:
		{
			return "ESYSTEM_EVENT_MOVE";
		}

		case ESYSTEM_EVENT_RESIZE:
		{
			return "ESYSTEM_EVENT_RESIZE";
		}

		case ESYSTEM_EVENT_ACTIVATE:
		{
			return "ESYSTEM_EVENT_ACTIVATE";
		}

		case ESYSTEM_EVENT_POS_CHANGED:
		{
			return "ESYSTEM_EVENT_POS_CHANGED";
		}

		case ESYSTEM_EVENT_STYLE_CHANGED:
		{
			return "ESYSTEM_EVENT_STYLE_CHANGED";
		}

		case ESYSTEM_EVENT_LEVEL_LOAD_START_PRELOADINGSCREEN:
		{
			return "ESYSTEM_EVENT_LEVEL_LOAD_START_PRELOADINGSCREEN";
		}

		case ESYSTEM_EVENT_LEVEL_LOAD_RESUME_GAME:
		{
			return "ESYSTEM_EVENT_LEVEL_LOAD_RESUME_GAME";
		}

		case ESYSTEM_EVENT_LEVEL_LOAD_PREPARE:
		{
			return "ESYSTEM_EVENT_LEVEL_LOAD_PREPARE";
		}

		case ESYSTEM_EVENT_LEVEL_LOAD_START_LOADINGSCREEN:
		{
			return "ESYSTEM_EVENT_LEVEL_LOAD_START_LOADINGSCREEN";
		}

		case ESYSTEM_EVENT_LEVEL_LOAD_LOADINGSCREEN_ACTIVE:
		{
			return "ESYSTEM_EVENT_LEVEL_LOAD_LOADINGSCREEN_ACTIVE";
		}

		case ESYSTEM_EVENT_LEVEL_LOAD_START:
		{
			return "ESYSTEM_EVENT_LEVEL_LOAD_START";
		}

		case ESYSTEM_EVENT_LEVEL_LOAD_END:
		{
			return "ESYSTEM_EVENT_LEVEL_LOAD_END";
		}

		case ESYSTEM_EVENT_LEVEL_LOAD_ERROR:
		{
			return "ESYSTEM_EVENT_LEVEL_LOAD_ERROR";
		}

		case ESYSTEM_EVENT_LEVEL_NOT_READY:
		{
			return "ESYSTEM_EVENT_LEVEL_NOT_READY";
		}

		case ESYSTEM_EVENT_LEVEL_PRECACHE_START:
		{
			return "ESYSTEM_EVENT_LEVEL_PRECACHE_START";
		}

		case ESYSTEM_EVENT_LEVEL_PRECACHE_FIRST_FRAME:
		{
			return "ESYSTEM_EVENT_LEVEL_PRECACHE_FIRST_FRAME";
		}

		case ESYSTEM_EVENT_LEVEL_GAMEPLAY_START:
		{
			return "ESYSTEM_EVENT_LEVEL_GAMEPLAY_START";
		}

		case ESYSTEM_EVENT_LEVEL_UNLOAD:
		{
			return "ESYSTEM_EVENT_LEVEL_UNLOAD";
		}

		case ESYSTEM_EVENT_LEVEL_POST_UNLOAD:
		{
			return "ESYSTEM_EVENT_LEVEL_POST_UNLOAD";
		}

		case ESYSTEM_EVENT_GAME_POST_INIT:
		{
			return "ESYSTEM_EVENT_GAME_POST_INIT";
		}

		case ESYSTEM_EVENT_GAME_POST_INIT_DONE:
		{
			return "ESYSTEM_EVENT_GAME_POST_INIT_DONE";
		}

		case ESYSTEM_EVENT_FULL_SHUTDOWN:
		{
			return "ESYSTEM_EVENT_FULL_SHUTDOWN";
		}

		case ESYSTEM_EVENT_FAST_SHUTDOWN:
		{
			return "ESYSTEM_EVENT_FAST_SHUTDOWN";
		}

		case ESYSTEM_EVENT_LANGUAGE_CHANGE:
		{
			return "ESYSTEM_EVENT_LANGUAGE_CHANGE";
		}

		case ESYSTEM_EVENT_TOGGLE_FULLSCREEN:
		{
			return "ESYSTEM_EVENT_TOGGLE_FULLSCREEN";
		}

		case ESYSTEM_EVENT_SHARE_SHADER_COMBINATIONS:
		{
			return "ESYSTEM_EVENT_SHARE_SHADER_COMBINATIONS";
		}

		case ESYSTEM_EVENT_3D_POST_RENDERING_START:
		{
			return "ESYSTEM_EVENT_3D_POST_RENDERING_START";
		}

		case ESYSTEM_EVENT_3D_POST_RENDERING_END:
		{
			return "ESYSTEM_EVENT_3D_POST_RENDERING_END";
		}

		case ESYSTEM_EVENT_LEVEL_PRECACHE_END:
		{
			return "ESYSTEM_EVENT_LEVEL_PRECACHE_END";
		}

		case ESYSTEM_EVENT_GAME_MODE_SWITCH_START:
		{
			return "ESYSTEM_EVENT_GAME_MODE_SWITCH_START";
		}

		case ESYSTEM_EVENT_GAME_MODE_SWITCH_END:
		{
			return "ESYSTEM_EVENT_GAME_MODE_SWITCH_END";
		}

		case ESYSTEM_EVENT_VIDEO:
		{
			return "ESYSTEM_EVENT_VIDEO";
		}

		case ESYSTEM_EVENT_GAME_PAUSED:
		{
			return "ESYSTEM_EVENT_GAME_PAUSED";
		}

		case ESYSTEM_EVENT_GAME_RESUMED:
		{
			return "ESYSTEM_EVENT_GAME_RESUMED";
		}

		case ESYSTEM_EVENT_TIME_OF_DAY_SET:
		{
			return "ESYSTEM_EVENT_TIME_OF_DAY_SET";
		}

		case ESYSTEM_EVENT_EDITOR_ON_INIT:
		{
			return "ESYSTEM_EVENT_EDITOR_ON_INIT";
		}

		case ESYSTEM_EVENT_FRONTEND_INITIALISED:
		{
			return "ESYSTEM_EVENT_FRONTEND_INITIALISED";
		}

		case ESYSTEM_EVENT_EDITOR_GAME_MODE_CHANGED:
		{
			return "ESYSTEM_EVENT_EDITOR_GAME_MODE_CHANGED";
		}

		case ESYSTEM_EVENT_EDITOR_SIMULATION_MODE_CHANGED:
		{
			return "ESYSTEM_EVENT_EDITOR_SIMULATION_MODE_CHANGED";
		}

		case ESYSTEM_EVENT_ENVIRONMENT_SETTINGS_CHANGED:
		{
			return "ESYSTEM_EVENT_ENVIRONMENT_SETTINGS_CHANGED";
		}

		case ESYSTEM_EVENT_FRONTEND_RELOADED:
		{
			return "ESYSTEM_EVENT_FRONTEND_RELOADED";
		}

		case ESYSTEM_EVENT_SW_FORCE_LOAD_START:
		{
			return "ESYSTEM_EVENT_SW_FORCE_LOAD_START";
		}

		case ESYSTEM_EVENT_SW_FORCE_LOAD_END:
		{
			return "ESYSTEM_EVENT_SW_FORCE_LOAD_END";
		}

		case ESYSTEM_EVENT_SW_SHIFT_WORLD:
		{
			return "ESYSTEM_EVENT_SW_SHIFT_WORLD";
		}

		case ESYSTEM_EVENT_STREAMING_INSTALL_ERROR:
		{
			return "ESYSTEM_EVENT_STREAMING_INSTALL_ERROR";
		}

		case ESYSTEM_EVENT_ONLINE_SERVICES_INITIALISED:
		{
			return "ESYSTEM_EVENT_ONLINE_SERVICES_INITIALISED";
		}

		case ESYSTEM_EVENT_AUDIO_IMPLEMENTATION_LOADED:
		{
			return "ESYSTEM_EVENT_AUDIO_IMPLEMENTATION_LOADED";
		}

		case ESYSTEM_EVENT_URI:
		{
			return "ESYSTEM_EVENT_URI";
		}

		case ESYSTEM_EVENT_USER:
		{
			return "ESYSTEM_EVENT_USER";
		}

		case ESYSTEM_EVENT_BEAM_PLAYER_TO_CAMERA_POS:
		{
			return "ESYSTEM_EVENT_BEAM_PLAYER_TO_CAMERA_POS";
		}

		case ESYSTEM_EVENT_CRYSYSTEM_INIT_DONE:
		{
			return "ESYSTEM_EVENT_CRYSYSTEM_INIT_DONE";
		}

		case ESYSTEM_EVENT_PRE_RENDERER_INIT:
		{
			return "ESYSTEM_EVENT_PRE_RENDERER_INIT";
		}

		case ESYSTEM_EVENT_GAMEWINDOW_ACTIVATE:
		{
			return "ESYSTEM_EVENT_GAMEWINDOW_ACTIVATE";
		}

		case ESYSTEM_EVENT_CVAR_REGISTERED:
		{
			return "ESYSTEM_EVENT_CVAR_REGISTERED";
		}

		case ESYSTEM_EVENT_CVAR_UNREGISTERED:
		{
			return "ESYSTEM_EVENT_CVAR_UNREGISTERED";
		}

		case ESYSTEM_EVENT_REGISTER_FLOWNODES:
		{
			return "ESYSTEM_EVENT_REGISTER_FLOWNODES";
		}

		case ESYSTEM_EVENT_GAME_FRAMEWORK_INIT_DONE:
		{
			return "ESYSTEM_EVENT_GAME_FRAMEWORK_INIT_DONE";
		}

		default:
		{
			return "UNKNOWN EVENT";
		}
	};
};

/*
class RenderListener;

class RenderListener : public IGameFrameworkListener
{
	ITexture * m_pTestTexture;
	bool m_bCanSkip;
public:
	RenderListener() 
		: m_bCanSkip(false)
		, m_pTestTexture(gEnv->pRenderer->EF_LoadTexture("Textures/OVERLAY_TEST1.tif", FT_DONT_RELEASE | FT_DONT_STREAM | FT_FILESINGLE))
	{
		RenderListener::Release();

		CRY_ASSERT_MESSAGE(s_pInstance == NULL, "Some how there's still an instance of RenderListener");
		s_pInstance = this;

		// Register listener for PostUpdate
		gEnv->pGameFramework->RegisterListener(this, "CryFreeVideoRenderer", EFRAMEWORKLISTENERPRIORITY::FRAMEWORKLISTENERPRIORITY_MENU);
	}

	ILINE static RenderListener* GetInstance() { return s_pInstance; }

	static void Release(bool bImmediateDelete = false)
	{
		s_pInstance = NULL;
	}

	void SetCanSkip(bool bCanSkip)
	{
		m_bCanSkip = bCanSkip;
	}

private:

	virtual ~RenderListener() override {

		if (m_pTestTexture)
			m_pTestTexture->ReleaseForce();

		gEnv->pGameFramework->UnregisterListener(this);
	};

	// IGameFrameworkListener
	void OnPostUpdate(float fDeltaTime) override {

		// Get Renderer Resolution
		const int &viewWidth = gEnv->pRenderer->GetWidth();
		const int &viewHeight = gEnv->pRenderer->GetHeight();

		// Scale to internal dimensions (800x600?)
		const float &fScaledW = viewWidth / (float(viewWidth) / 800.0f);
		const float &fScaledH = viewHeight / (float(viewHeight) / 600.0f);

		// Draw image directly
		gEnv->pRenderer->Draw2dImage(0, 0, fScaledW, fScaledH, m_pTestTexture->GetTextureID(), 0, 1, 1, 0, 0);
	};
	void OnLoadGame(ILoadGame* pLoadGame) {}
	void OnSaveGame(ISaveGame* pSaveGame) {}
	void OnLevelEnd(const char* nextLevel) {}
	void OnActionEvent(const SActionEvent& event) {}
	void OnPreRender() {}

	static RenderListener* s_pInstance;
	static RenderListener* s_pFirstNonFreedRenderListener;
};

RenderListener*	RenderListener::s_pInstance = NULL;
*/

CCryFreeVideoSystem::CCryFreeVideoSystem() : m_bDoIntroMovies(true), m_bPlayingVideo(false)
{
	//gEnv->pSystem->GetISystemEventDispatcher()->RegisterListener(this);
}

CCryFreeVideoSystem::~CCryFreeVideoSystem()
{
	//gEnv->pSystem->GetISystemEventDispatcher()->RemoveListener(this);
}
/*
void CCryFreeVideoSystem::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
	CryWarning(EValidatorModule::VALIDATOR_MODULE_GAME, EValidatorSeverity::VALIDATOR_WARNING, "CCryFreeVideoSystem::OnSystemEvent(%s)", SystemEventIdToName(event));
}
*/
void CCryFreeVideoSystem::CryFreeVideoStart()
{

}

void CCryFreeVideoSystem::CryFreeVideoEnd()
{
}

///////////////////////////////////////////////////////////////////////////
void CCryFreeVideoSystem::Shutdown()
{

}

/*

CCryFreeVideoSystem::CCryFreeVideoSystem() : m_bDoIntroMovies(true), m_bPlayingVideo(false)
{
	gEnv->pSystem->GetISystemEventDispatcher()->RegisterListener(this);
}

CCryFreeVideoSystem::~CCryFreeVideoSystem()
{
	gEnv->pSystem->GetISystemEventDispatcher()->RemoveListener(this);
	RenderListener::Release(true);
}

void CCryFreeVideoSystem::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
	CryWarning(EValidatorModule::VALIDATOR_MODULE_GAME, EValidatorSeverity::VALIDATOR_WARNING, "CCryFreeVideoSystem::OnSystemEvent(%s)", SystemEventIdToName(event));
	switch (event)
	{
	case ESYSTEM_EVENT_FAST_SHUTDOWN:
	case ESYSTEM_EVENT_FULL_SHUTDOWN:
	{
		gEnv->pConsole->PrintLine("CryFreeVideo shutting down");
		Shutdown();
		break;
	}

	// Earliest point we can render to view using PostUpdate
	case ESYSTEM_EVENT_REGISTER_FLOWNODES:
	{
		if (m_bDoIntroMovies)
		{
			CryFreeVideoStart();
		}
		break;
	}

	// Earliest point we should allow skipping any remaining video
	case ESYSTEM_EVENT_GAME_FRAMEWORK_INIT_DONE:
	{
		if (m_bDoIntroMovies)
		{
			m_pRenderListener->SetCanSkip(true);
		}
		break;
	}
	}
}

void CCryFreeVideoSystem::CryFreeVideoStart()
{
	if (m_bPlayingVideo)
		return;

	gEnv->pConsole->PrintLine("CryFreeVideoTest Start");

	
	if (m_pRenderListener = new RenderListener())
	{
		m_bPlayingVideo = true;
	}

}

void CCryFreeVideoSystem::CryFreeVideoEnd()
{
	gEnv->pConsole->PrintLine("CryFreeVideoTest End");

	m_bPlayingVideo = false;
}

///////////////////////////////////////////////////////////////////////////
void CCryFreeVideoSystem::Shutdown()
{

}
*/