// Copyright (c) 2017, uniflare, see LICENSE.txt for details

#pragma once
#include "cvars.h"

#include <CryRenderer/ITexture.h>
#include <IPlayerProfiles.h>

#ifdef _DEBUG
#define POR_RESULT_CODE_TO_TEXT(EResult) \
(EResult == IPlayerProfileManager::EProfileOperationResult::ePOR_Success)? "ePOR_Success" : 	\
(EResult == IPlayerProfileManager::EProfileOperationResult::ePOR_NotInitialized)? "ePOR_NotInitialized" : 	\
(EResult == IPlayerProfileManager::EProfileOperationResult::ePOR_NameInUse)? "ePOR_NameInUse" : 	\
(EResult == IPlayerProfileManager::EProfileOperationResult::ePOR_UserNotLoggedIn)? "ePOR_UserNotLoggedIn" : 	\
(EResult == IPlayerProfileManager::EProfileOperationResult::ePOR_NoSuchProfile)? "ePOR_NoSuchProfile" : 	\
(EResult == IPlayerProfileManager::EProfileOperationResult::ePOR_ProfileInUse)? "ePOR_ProfileInUse" : 	\
(EResult == IPlayerProfileManager::EProfileOperationResult::ePOR_NoActiveProfile)? "ePOR_NoActiveProfile" : 	\
(EResult == IPlayerProfileManager::EProfileOperationResult::ePOR_DefaultProfile)? "ePOR_DefaultProfile" : 	\
(EResult == IPlayerProfileManager::EProfileOperationResult::ePOR_LoadingProfile)? "ePOR_LoadingProfile" : "ePOR_Unknown"

#define SYSEVENT_CODE_TO_TEXT(EEvent) \
EEvent == ESYSTEM_EVENT_CHANGE_FOCUS? "ESYSTEM_EVENT_CHANGE_FOCUS" : \
EEvent == ESYSTEM_EVENT_MOVE? "ESYSTEM_EVENT_MOVE" : \
EEvent == ESYSTEM_EVENT_RESIZE? "ESYSTEM_EVENT_RESIZE" : \
EEvent == ESYSTEM_EVENT_ACTIVATE? "ESYSTEM_EVENT_ACTIVATE" : \
EEvent == ESYSTEM_EVENT_POS_CHANGED? "ESYSTEM_EVENT_POS_CHANGED" : \
EEvent == ESYSTEM_EVENT_STYLE_CHANGED? "ESYSTEM_EVENT_STYLE_CHANGED" : \
EEvent == ESYSTEM_EVENT_LEVEL_LOAD_START_PRELOADINGSCREEN? "ESYSTEM_EVENT_LEVEL_LOAD_START_PRELOADINGSCREEN" : \
EEvent == ESYSTEM_EVENT_LEVEL_LOAD_RESUME_GAME? "ESYSTEM_EVENT_LEVEL_LOAD_RESUME_GAME" : \
EEvent == ESYSTEM_EVENT_LEVEL_LOAD_START? "ESYSTEM_EVENT_LEVEL_LOAD_START" : \
EEvent == ESYSTEM_EVENT_LEVEL_LOAD_PREPARE? "ESYSTEM_EVENT_LEVEL_LOAD_PREPARE" : \
EEvent == ESYSTEM_EVENT_LEVEL_LOAD_START_LOADINGSCREEN? "ESYSTEM_EVENT_LEVEL_LOAD_START_LOADINGSCREEN" : \
EEvent == ESYSTEM_EVENT_LEVEL_LOAD_LOADINGSCREEN_ACTIVE? "ESYSTEM_EVENT_LEVEL_LOAD_LOADINGSCREEN_ACTIVE" : \
EEvent == ESYSTEM_EVENT_LEVEL_LOAD_START? "ESYSTEM_EVENT_LEVEL_LOAD_START" : \
EEvent == ESYSTEM_EVENT_LEVEL_LOAD_END? "ESYSTEM_EVENT_LEVEL_LOAD_END" : \
EEvent == ESYSTEM_EVENT_LEVEL_LOAD_ERROR? "ESYSTEM_EVENT_LEVEL_LOAD_ERROR" : \
EEvent == ESYSTEM_EVENT_LEVEL_NOT_READY? "ESYSTEM_EVENT_LEVEL_NOT_READY" : \
EEvent == ESYSTEM_EVENT_LEVEL_PRECACHE_START? "ESYSTEM_EVENT_LEVEL_PRECACHE_START" : \
EEvent == ESYSTEM_EVENT_LEVEL_PRECACHE_FIRST_FRAME? "ESYSTEM_EVENT_LEVEL_PRECACHE_FIRST_FRAME" : \
EEvent == ESYSTEM_EVENT_LEVEL_GAMEPLAY_START? "ESYSTEM_EVENT_LEVEL_GAMEPLAY_START" : \
EEvent == ESYSTEM_EVENT_LEVEL_UNLOAD? "ESYSTEM_EVENT_LEVEL_UNLOAD" : \
EEvent == ESYSTEM_EVENT_LEVEL_POST_UNLOAD? "ESYSTEM_EVENT_LEVEL_POST_UNLOAD" : \
EEvent == ESYSTEM_EVENT_GAME_POST_INIT? "ESYSTEM_EVENT_GAME_POST_INIT" : \
EEvent == ESYSTEM_EVENT_GAME_POST_INIT_DONE? "ESYSTEM_EVENT_GAME_POST_INIT_DONE" : \
EEvent == ESYSTEM_EVENT_FULL_SHUTDOWN? "ESYSTEM_EVENT_FULL_SHUTDOWN" : \
EEvent == ESYSTEM_EVENT_FAST_SHUTDOWN? "ESYSTEM_EVENT_FAST_SHUTDOWN" : \
EEvent == ESYSTEM_EVENT_LANGUAGE_CHANGE? "ESYSTEM_EVENT_LANGUAGE_CHANGE" : \
EEvent == ESYSTEM_EVENT_TOGGLE_FULLSCREEN? "ESYSTEM_EVENT_TOGGLE_FULLSCREEN" : \
EEvent == ESYSTEM_EVENT_SHARE_SHADER_COMBINATIONS? "ESYSTEM_EVENT_SHARE_SHADER_COMBINATIONS" : \
EEvent == ESYSTEM_EVENT_3D_POST_RENDERING_START? "ESYSTEM_EVENT_3D_POST_RENDERING_START" : \
EEvent == ESYSTEM_EVENT_3D_POST_RENDERING_END? "ESYSTEM_EVENT_3D_POST_RENDERING_END" : \
EEvent == ESYSTEM_EVENT_LEVEL_PRECACHE_END? "ESYSTEM_EVENT_LEVEL_PRECACHE_END" : \
EEvent == ESYSTEM_EVENT_GAME_MODE_SWITCH_START? "ESYSTEM_EVENT_GAME_MODE_SWITCH_START" : \
EEvent == ESYSTEM_EVENT_GAME_MODE_SWITCH_END? "ESYSTEM_EVENT_GAME_MODE_SWITCH_END" : \
EEvent == ESYSTEM_EVENT_VIDEO? "ESYSTEM_EVENT_VIDEO" : \
EEvent == ESYSTEM_EVENT_GAME_PAUSED? "ESYSTEM_EVENT_GAME_PAUSED" : \
EEvent == ESYSTEM_EVENT_GAME_RESUMED? "ESYSTEM_EVENT_GAME_RESUMED" : \
EEvent == ESYSTEM_EVENT_TIME_OF_DAY_SET? "ESYSTEM_EVENT_TIME_OF_DAY_SET" : \
EEvent == ESYSTEM_EVENT_EDITOR_ON_INIT? "ESYSTEM_EVENT_EDITOR_ON_INIT" : \
EEvent == ESYSTEM_EVENT_FRONTEND_INITIALISED? "ESYSTEM_EVENT_FRONTEND_INITIALISED" : \
EEvent == ESYSTEM_EVENT_EDITOR_GAME_MODE_CHANGED? "ESYSTEM_EVENT_EDITOR_GAME_MODE_CHANGED" : \
EEvent == ESYSTEM_EVENT_EDITOR_SIMULATION_MODE_CHANGED? "ESYSTEM_EVENT_EDITOR_SIMULATION_MODE_CHANGED" : \
EEvent == ESYSTEM_EVENT_ENVIRONMENT_SETTINGS_CHANGED? "ESYSTEM_EVENT_ENVIRONMENT_SETTINGS_CHANGED" : \
EEvent == ESYSTEM_EVENT_FRONTEND_RELOADED? "ESYSTEM_EVENT_FRONTEND_RELOADED" : \
EEvent == ESYSTEM_EVENT_SW_FORCE_LOAD_START? "ESYSTEM_EVENT_SW_FORCE_LOAD_START" : \
EEvent == ESYSTEM_EVENT_SW_FORCE_LOAD_END? "ESYSTEM_EVENT_SW_FORCE_LOAD_END" : \
EEvent == ESYSTEM_EVENT_SW_SHIFT_WORLD? "ESYSTEM_EVENT_SW_SHIFT_WORLD" : \
EEvent == ESYSTEM_EVENT_CHANGE_FOCUS? "ESYSTEM_EVENT_CHANGE_FOCUS" : \
EEvent == ESYSTEM_EVENT_STREAMING_INSTALL_ERROR? "ESYSTEM_EVENT_STREAMING_INSTALL_ERROR" : \
EEvent == ESYSTEM_EVENT_ONLINE_SERVICES_INITIALISED? "ESYSTEM_EVENT_ONLINE_SERVICES_INITIALISED" : \
EEvent == ESYSTEM_EVENT_AUDIO_IMPLEMENTATION_LOADED? "ESYSTEM_EVENT_AUDIO_IMPLEMENTATION_LOADED" : \
EEvent == ESYSTEM_EVENT_URI? "ESYSTEM_EVENT_URI" : \
EEvent == ESYSTEM_EVENT_USER? "ESYSTEM_EVENT_USER" : \
EEvent == ESYSTEM_EVENT_BEAM_PLAYER_TO_CAMERA_POS? "ESYSTEM_EVENT_BEAM_PLAYER_TO_CAMERA_POS" : \
EEvent == ESYSTEM_EVENT_CRYSYSTEM_INIT_DONE? "ESYSTEM_EVENT_CRYSYSTEM_INIT_DONE" : \
EEvent == ESYSTEM_EVENT_PRE_RENDERER_INIT? "ESYSTEM_EVENT_PRE_RENDERER_INIT" : \
EEvent == ESYSTEM_EVENT_GAMEWINDOW_ACTIVATE? "ESYSTEM_EVENT_GAMEWINDOW_ACTIVATE" : \
EEvent == ESYSTEM_EVENT_REGISTER_FLOWNODES? "ESYSTEM_EVENT_REGISTER_FLOWNODES" : \
EEvent == ESYSTEM_EVENT_CRYSYSTEM_INIT_DONE? "ESYSTEM_EVENT_CRYSYSTEM_INIT_DONE" : \
EEvent == ESYSTEM_EVENT_GAME_FRAMEWORK_INIT_DONE? "ESYSTEM_EVENT_GAME_FRAMEWORK_INIT_DONE" : "UNKNOWN"
#else
#define POR_RESULT_CODE_TO_TEXT(...) ""
#define SYSEVENT_CODE_TO_TEXT(...) ""
#endif // _DEBUG

// This is due to the default gamesdk not using default profile attributes from configs.
// -1 will tell GetScreenResolution(idx) to give us the native screen res
#define WINDOW_DEFAULT_RESOLUTION_IDX -1
#define WINDOW_DEFAULT_FULLSCREEN 1
#define WINDOW_DEFAULT_HIDE_BORDER 0

typedef IPlayerProfileManager::EProfileOperationResult EPOResult;

// CSplashExample implements ISystemEventListener
//
//! Handles main functionality of plugin Splash Example
class CSplashExample 
	: public ISystemEventListener,
	public IPlayerProfileListener
{

private:

	//! Struct to hold screen resolution details
	struct SScreenResolution
	{
		// Copied from GameSDK - For using profile saved resolution
		int iWidth;
		int iHeight;
		int nDepthPerPixel;
		string sResolution;

		SScreenResolution(unsigned int _iWidth, unsigned int _iHeight, unsigned int _nDepthPerPixel, const char* _sResolution) :
			iWidth(_iWidth)
			, iHeight(_iHeight)
			, nDepthPerPixel(_nDepthPerPixel)
			, sResolution(_sResolution)
		{}

		SScreenResolution() :
			iWidth(-1)
			, iHeight(-1)
			, nDepthPerPixel(-1)
			, sResolution("")
		{}
	};

	struct SWindowMode
	{
		bool bFullscreen;
		bool bHideWindowBorder;

		SWindowMode(const bool &fullscreen, const bool &hideWindowBorder)
		{
			bFullscreen = fullscreen;
			bHideWindowBorder = hideWindowBorder;
		}

		SWindowMode() :
			bFullscreen(1),
			bHideWindowBorder(1)
		{}
	};

	struct SWindowProperties
	{
		SScreenResolution sScreenResolution;
		SWindowMode sWindowMode;

		SWindowProperties(const SScreenResolution &screenResolution, const SWindowMode &windowMode) :
			sScreenResolution(screenResolution),
			sWindowMode(windowMode)
		{}

		SWindowProperties() :
			sScreenResolution(),
			sWindowMode()
		{}
	};

	// Console variables provided by this object
	CVars m_sCVars;

	//! Textures
	ITexture * m_pInitialSplashTexture;
	ITexture * m_pSplashTexture;

	//! Simple flag to make sure we don't initialize more than once
	bool m_bInitialized;

	//! Simple flags to make sure we don't register/unregister our listeners too many times etc
	bool m_bSystemListenerRegistered;
	bool m_bProfileListenerRegistered;
	bool m_bLoadedProfileAttributes;
	bool m_bIgnoreNextProfileSave;
	
	//! Local refs
	IPlayerProfileManager * m_pProfileManager;
	IPlayerProfile * m_pCurrentProfile;
	IPlayerProfile * m_pDefaultProfile;

private:

	//! Handles init of Splash Example. (Can only init once)
	void TryInitialize();

	//! Draws the supplied texture in stretched mode to the main viewport
	void Draw2DImage(const ITexture * tex, bool bUseTextureSize = false) const;

	//! Wrapper to stall the current thread for LengthTime whilst drawing pTex
	bool DrawAndStall(float StartTime, float LengthTime, ITexture * pTex, bool bUseTextureSize = false, bool bUpdateInput = true, bool bUpdateMouse = true, bool bDrawConsole = true, bool bPumpWinMsg = true);

	//! Gets the currently in-use player profile
	IPlayerProfile * GetCurrentPlayerProfile(IPlayerProfileManager * pProfileManager = GetIPlayerProfileManager());

	//! Gets the default player profile
	IPlayerProfile * GetDefaultPlayerProfile(IPlayerProfileManager * pProfileManager = GetIPlayerProfileManager());


	//! Gets the requested screen resolution from GetScreenResolutions()
	static const SScreenResolution GetScreenResolution(const int idx);

	//! Generates a list of supported screen resolutions
	static const std::vector<SScreenResolution> GetScreenResolutions();

	const SWindowProperties GetWindowProperties();

	//! Sets the r_width and r_height CVars if they are set properly in sResolution
	void SetScreenResolution(const SScreenResolution &sResolution) const;

	//! Self explanatory
	void SafeReleaseSplashTextures();

	//! Handles the display of the initial splash window
	void DisplayInitialSplash(bool stallThread = false);

	//! Handles the display of the main splash window
	void DisplayMainSplash(const bool stallThread = false);

	bool TryLoadProfileAttributes();

	bool CopyProfileAttributes(IPlayerProfile * pTo, IPlayerProfile * pFrom);

	static IPlayerProfileManager * GetIPlayerProfileManager();
	bool SaveSplashFlagToProfile();
	bool SetProfileAttribute(const char * attributeName, const TFlowInputData attributeValue);
	template <class T>
	bool SetProfileAttribute(const char * attributeName, const T& attributeValue);
	bool SavePlayerProfile(EPOResult &result, EProfileReasons reason = EProfileReasons::ePR_Options);

	bool RegisterSystemListener(ISystemEventListener * pListener);
	bool UnregisterSystemListener(ISystemEventListener * pListener);
	// ISystemEventListener
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;
	// ~ISystemEventListener

	bool RegisterProfileListener(IPlayerProfileListener * pListener);
	bool UnregisterProfileListener(IPlayerProfileListener * pListener);
	// IPlayerProfileListener
	virtual void SaveToProfile(IPlayerProfile* pProfile, bool online, unsigned int /*EProfileReasons*/ reason) override;
	virtual void LoadFromProfile(IPlayerProfile* pProfile, bool online, unsigned int /*EProfileReasons*/ reason) override;
	// ~IPlayerProfileListener

	bool IsShuttingDown();

public:

	CSplashExample();
	~CSplashExample();

	// ICryPlugin
	bool Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams);
	// ~ICryPlugin
};