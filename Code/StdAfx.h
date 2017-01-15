// 05/01/2017 Copyright by Uniflare. All rights reserved.
// See bundled license.txt for more information.

#pragma once

#ifndef _DEBUG
#define _RELEASE 1
#endif

#include <CryCore/Project/CryModuleDefs.h>
#define eCryModule eCryM_EnginePlugin
#define GAME_API   DLL_EXPORT

#define CRY_LOG_RELEASE(...) CryLogAlways(##__VA_ARGS__)
#define CRY_LOG_ERROR(...) CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_ERROR, ##__VA_ARGS__)

#ifdef _DEBUG
#define CRY_LOG_DEBUG(...) CryLogAlways(##__VA_ARGS__)
#else
//#define CRY_LOG_DEBUG(...) true
#define CRY_LOG_DEBUG(...) true
#endif

#include <CryCore/Platform/platform.h>