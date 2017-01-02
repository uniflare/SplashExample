#pragma once

// Don't want debug messages
#ifndef HelperDevDebug
#define SplashExampleDebugMsg(msg)
#define SplashExampleDebugMsgV(msg, ...)
#else

#include "CrySystem\ISystem.h"

#include <sstream>
#include <iomanip>
#include <array>

///////////////////////////////////////////////////////////////////////////
//! Debugging defines
#define SplashExampleDebugMsg(msg) CryWarning(EValidatorModule::VALIDATOR_MODULE_GAME,EValidatorSeverity::VALIDATOR_COMMENT, "DEBUG ::CSplashExamplePlugin::" msg)
#define SplashExampleDebugMsgV(msg, ...) CryWarning(EValidatorModule::VALIDATOR_MODULE_GAME,EValidatorSeverity::VALIDATOR_COMMENT, "DEBUG ::CSplashExamplePlugin::" msg, ##__VA_ARGS__)


///////////////////////////////////////////////////////////////////////////
//! Assortment of static converters to make data more readable
struct CFVDEBUGUTIL
{
	//! Converts an instance method pointer to 64bit hex
	//! Alias of overload of TO_HEX64()
	template<typename R, typename T, typename... Args>
	static string FUNCPOINTER_TO_HEXADDR(R(T::*func)(Args...))
	{
		return TO_HEX64(func);
	}

	//! Converts uintptr_t to 64bit hex
	//! Alias of overload of TO_HEX64()
	static string POINTER_TO_HEXADDR(const uintptr_t ptr)
	{
		return TO_HEX64(ptr);
	}

	//! Converts an instance method pointer to 64bit hex
	template<typename R, typename T, typename... Args>
	static string TO_HEX64(R(T::*func)(Args...))
	{
		union PtrUnion
		{
			R(T::*f)(Args...);
			uintptr_t ptr;
		};
		PtrUnion u;
		u.f = func;
		return TO_HEX64(u.ptr);
	}

	//! Converts an int to 64bit hex
	static string TO_HEX64(const int i32)
	{
		int64 i64 = i32;
		return TO_HEX64(i64);
	}

	//! Converts a volatile int to 64bit hex
	static string TO_HEX64(const volatile int& i32)
	{
		int64 i64 = i32;
		return TO_HEX64(i64);
	}

	//! Converts a uintptr_t to 64bit hex
	static string TO_HEX64(const uintptr_t ptr)
	{
		int64 i64 = ptr;
		return TO_HEX64(i64);
	}

	//! Converts an int64 to 64bit hex
	static string TO_HEX64(const int64 i64Num)
	{
		union PtrUnion
		{
			const int64 f;
			unsigned char buf[sizeof(int64)];
		};
		PtrUnion u{ i64Num };
		std::reverse(std::begin(u.buf), std::end(u.buf));
		// Pretty print
		std::ostringstream os;
		os << "0x" << std::hex << std::setfill('0') << std::uppercase;
		for (auto c : u.buf)
			os << std::setw(2) << (unsigned)c;
		return string(os.str().c_str());
	}

	static const char * EVENT_TO_STRING(const ESystemEvent event)
	{
		switch (event)
		{
			case ESYSTEM_EVENT_MOVE:										{return "ESYSTEM_EVENT_MOVE";}
			case ESYSTEM_EVENT_RESIZE:								        {return "ESYSTEM_EVENT_RESIZE";}
			case ESYSTEM_EVENT_ACTIVATE:							        {return "ESYSTEM_EVENT_ACTIVATE";}
			case ESYSTEM_EVENT_POS_CHANGED:							        {return "ESYSTEM_EVENT_POS_CHANGED";}
			case ESYSTEM_EVENT_STYLE_CHANGED:						        {return "ESYSTEM_EVENT_STYLE_CHANGED";}
			case ESYSTEM_EVENT_LEVEL_LOAD_START_PRELOADINGSCREEN:			{return "ESYSTEM_EVENT_LEVEL_LOAD_START_PRELOADINGSCREEN";}
			case ESYSTEM_EVENT_LEVEL_LOAD_RESUME_GAME:				        {return "ESYSTEM_EVENT_LEVEL_LOAD_RESUME_GAME";}
			case ESYSTEM_EVENT_LEVEL_LOAD_PREPARE:					        {return "ESYSTEM_EVENT_LEVEL_LOAD_PREPARE";}
			case ESYSTEM_EVENT_LEVEL_LOAD_START_LOADINGSCREEN:		        {return "ESYSTEM_EVENT_LEVEL_LOAD_START_LOADINGSCREEN";}
			case ESYSTEM_EVENT_LEVEL_LOAD_LOADINGSCREEN_ACTIVE:				{return "ESYSTEM_EVENT_LEVEL_LOAD_LOADINGSCREEN_ACTIVE";}
			case ESYSTEM_EVENT_LEVEL_LOAD_START:					        {return "ESYSTEM_EVENT_LEVEL_LOAD_START";}
			case ESYSTEM_EVENT_LEVEL_LOAD_END:						        {return "ESYSTEM_EVENT_LEVEL_LOAD_END";}
			case ESYSTEM_EVENT_LEVEL_LOAD_ERROR:					        {return "ESYSTEM_EVENT_LEVEL_LOAD_ERROR";}
			case ESYSTEM_EVENT_LEVEL_NOT_READY:						        {return "ESYSTEM_EVENT_LEVEL_NOT_READY";}
			case ESYSTEM_EVENT_LEVEL_PRECACHE_START:				        {return "ESYSTEM_EVENT_LEVEL_PRECACHE_START";}
			case ESYSTEM_EVENT_LEVEL_PRECACHE_FIRST_FRAME:			        {return "ESYSTEM_EVENT_LEVEL_PRECACHE_FIRST_FRAME";}
			case ESYSTEM_EVENT_LEVEL_GAMEPLAY_START:				        {return "ESYSTEM_EVENT_LEVEL_GAMEPLAY_START";}
			case ESYSTEM_EVENT_LEVEL_UNLOAD:						        {return "ESYSTEM_EVENT_LEVEL_UNLOAD";}
			case ESYSTEM_EVENT_LEVEL_POST_UNLOAD:					        {return "ESYSTEM_EVENT_LEVEL_POST_UNLOAD";}
			case ESYSTEM_EVENT_GAME_POST_INIT:						        {return "ESYSTEM_EVENT_GAME_POST_INIT";}
			case ESYSTEM_EVENT_GAME_POST_INIT_DONE:					        {return "ESYSTEM_EVENT_GAME_POST_INIT_DONE";}
			case ESYSTEM_EVENT_FULL_SHUTDOWN:						        {return "ESYSTEM_EVENT_FULL_SHUTDOWN";}
			case ESYSTEM_EVENT_FAST_SHUTDOWN:						        {return "ESYSTEM_EVENT_FAST_SHUTDOWN";}
			case ESYSTEM_EVENT_LANGUAGE_CHANGE:						        {return "ESYSTEM_EVENT_LANGUAGE_CHANGE";}
			case ESYSTEM_EVENT_TOGGLE_FULLSCREEN:					        {return "ESYSTEM_EVENT_TOGGLE_FULLSCREEN";}
			case ESYSTEM_EVENT_SHARE_SHADER_COMBINATIONS:			        {return "ESYSTEM_EVENT_SHARE_SHADER_COMBINATIONS";}
			case ESYSTEM_EVENT_3D_POST_RENDERING_START:				        {return "ESYSTEM_EVENT_3D_POST_RENDERING_START";}
			case ESYSTEM_EVENT_3D_POST_RENDERING_END:				        {return "ESYSTEM_EVENT_3D_POST_RENDERING_END";}
			case ESYSTEM_EVENT_LEVEL_PRECACHE_END:					        {return "ESYSTEM_EVENT_LEVEL_PRECACHE_END";}
			case ESYSTEM_EVENT_GAME_MODE_SWITCH_START:				        {return "ESYSTEM_EVENT_GAME_MODE_SWITCH_START";}
			case ESYSTEM_EVENT_GAME_MODE_SWITCH_END:				        {return "ESYSTEM_EVENT_GAME_MODE_SWITCH_END";}
			case ESYSTEM_EVENT_VIDEO:								        {return "ESYSTEM_EVENT_VIDEO";}
			case ESYSTEM_EVENT_GAME_PAUSED:							        {return "ESYSTEM_EVENT_GAME_PAUSED";}
			case ESYSTEM_EVENT_GAME_RESUMED:						        {return "ESYSTEM_EVENT_GAME_RESUMED";}
			case ESYSTEM_EVENT_TIME_OF_DAY_SET:						        {return "ESYSTEM_EVENT_TIME_OF_DAY_SET";}
			case ESYSTEM_EVENT_EDITOR_ON_INIT:						        {return "ESYSTEM_EVENT_EDITOR_ON_INIT";}
			case ESYSTEM_EVENT_FRONTEND_INITIALISED:				        {return "ESYSTEM_EVENT_FRONTEND_INITIALISED";}
			case ESYSTEM_EVENT_EDITOR_GAME_MODE_CHANGED:			        {return "ESYSTEM_EVENT_EDITOR_GAME_MODE_CHANGED";}
			case ESYSTEM_EVENT_EDITOR_SIMULATION_MODE_CHANGED:		        {return "ESYSTEM_EVENT_EDITOR_SIMULATION_MODE_CHANGED";}
			case ESYSTEM_EVENT_ENVIRONMENT_SETTINGS_CHANGED:		        {return "ESYSTEM_EVENT_ENVIRONMENT_SETTINGS_CHANGED";}
			case ESYSTEM_EVENT_FRONTEND_RELOADED:					        {return "ESYSTEM_EVENT_FRONTEND_RELOADED";}
			case ESYSTEM_EVENT_SW_FORCE_LOAD_START:					        {return "ESYSTEM_EVENT_SW_FORCE_LOAD_START";}
			case ESYSTEM_EVENT_SW_FORCE_LOAD_END:					        {return "ESYSTEM_EVENT_SW_FORCE_LOAD_END";}
			case ESYSTEM_EVENT_SW_SHIFT_WORLD:						        {return "ESYSTEM_EVENT_SW_SHIFT_WORLD";}
			case ESYSTEM_EVENT_STREAMING_INSTALL_ERROR:				        {return "ESYSTEM_EVENT_STREAMING_INSTALL_ERROR";}
			case ESYSTEM_EVENT_ONLINE_SERVICES_INITIALISED:			        {return "ESYSTEM_EVENT_ONLINE_SERVICES_INITIALISED";}
			case ESYSTEM_EVENT_AUDIO_IMPLEMENTATION_LOADED:			        {return "ESYSTEM_EVENT_AUDIO_IMPLEMENTATION_LOADED";}
			case ESYSTEM_EVENT_URI:									        {return "ESYSTEM_EVENT_URI";}
			case ESYSTEM_EVENT_USER:								        {return "ESYSTEM_EVENT_USER";}
			case ESYSTEM_EVENT_BEAM_PLAYER_TO_CAMERA_POS:			        {return "ESYSTEM_EVENT_BEAM_PLAYER_TO_CAMERA_POS";}
			case ESYSTEM_EVENT_CRYSYSTEM_INIT_DONE:					        {return "ESYSTEM_EVENT_CRYSYSTEM_INIT_DONE";}
			case ESYSTEM_EVENT_PRE_RENDERER_INIT:					        {return "ESYSTEM_EVENT_PRE_RENDERER_INIT";}
			case ESYSTEM_EVENT_GAMEWINDOW_ACTIVATE:					        {return "ESYSTEM_EVENT_GAMEWINDOW_ACTIVATE";}
			case ESYSTEM_EVENT_CVAR_REGISTERED:						        {return "ESYSTEM_EVENT_CVAR_REGISTERED";}
			case ESYSTEM_EVENT_CVAR_UNREGISTERED:					        {return "ESYSTEM_EVENT_CVAR_UNREGISTERED";}
			case ESYSTEM_EVENT_REGISTER_FLOWNODES:					        {return "ESYSTEM_EVENT_REGISTER_FLOWNODES";}
			case ESYSTEM_EVENT_GAME_FRAMEWORK_INIT_DONE:			        {return "ESYSTEM_EVENT_GAME_FRAMEWORK_INIT_DONE";}			
			default: { return "UNKNOWN EVENT"; }
		};
	}
};
#endif