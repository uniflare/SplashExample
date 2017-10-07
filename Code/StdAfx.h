// 05/01/2017 Copyright by Uniflare. All rights reserved.
// See bundled license.txt for more information.

#pragma once

#include <CryCore/Project/CryModuleDefs.h>
#define eCryModule eCryM_EnginePlugin
#define GAME_API   DLL_EXPORT

// Custom logging defines for readability
#define CRY_LOG_ALWAYS(...) CryLogAlways("[SplashExamplePlugin] " ##__VA_ARGS__)
#define CRY_LOG_ERROR(...) CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_ERROR, "[SplashExamplePlugin] " ##__VA_ARGS__)
#define CRY_LOG_WARNING(...) CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "[SplashExamplePlugin] " ##__VA_ARGS__)
#define CRY_LOG_DEBUG(...) CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_COMMENT, "[SplashExamplePlugin] " ##__VA_ARGS__)
#ifdef _DEBUG
#define CRY_LOG_CALL(...) CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_COMMENT, "[SplashExamplePlugin] " ##__VA_ARGS__)
#else
#define CRY_LOG_CALL(...) 0
#endif

#include <CryCore/Platform/platform.h>