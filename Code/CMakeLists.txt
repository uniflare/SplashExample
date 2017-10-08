cmake_minimum_required (VERSION 3.6.2)
set(CRYENGINE_DIR "C:/Program Files (x86)/Crytek/CRYENGINE Launcher/Crytek/CRYENGINE_5.4")
set(TOOLS_CMAKE_DIR "${CRYENGINE_DIR}/Tools/CMake")

set(PROJECT_BUILD 1)
set(PROJECT_DIR "C:/dev/git/sf/SplashExample")

include("${TOOLS_CMAKE_DIR}/CommonOptions.cmake")

add_subdirectory("${CRYENGINE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/CRYENGINE")

include("${TOOLS_CMAKE_DIR}/Configure.cmake")
start_sources()

sources_platform(ALL)
add_sources("Code_uber.cpp"
    PROJECTS SplashExamplePlugin
    SOURCE_GROUP "Root"
		"Plugin.cpp"
		"SplashExample.cpp"
		"StdAfx.cpp"
		"cvars.h"
		"Plugin.h"
		"SplashExample.h"
		"StdAfx.h"
)

end_sources()

CryEngineModule(SplashExamplePlugin PCH "StdAfx.cpp" SOLUTION_FOLDER "Project")

target_include_directories(${THIS_PROJECT}
PRIVATE 
    "${CRYENGINE_DIR}/Code/CryEngine/CryCommon"
    "${CRYENGINE_DIR}/Code/CryEngine/CryAction"
	"${CRYENGINE_DIR}/Code/CryEngine/CrySchematyc/Core/Interface"
	"${CRYENGINE_DIR}/Code/CryPlugins/CryDefaultEntities/Module"
)

# Set StartUp project in Visual Studio

add_library(GameLauncher STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
set_target_properties(GameLauncher PROPERTIES LINKER_LANGUAGE CXX)
if (WIN32)
    set_visual_studio_debugger_command(GameLauncher "${CRYENGINE_DIR}/bin/win_x64/GameLauncher.exe" "-project \"C:/dev/git/sf/SplashExample/SplashExamplePlugin.cryproject\"")
endif()

add_library(Sandbox STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
set_target_properties(Sandbox PROPERTIES LINKER_LANGUAGE CXX)
if (WIN32)
    set_visual_studio_debugger_command(Sandbox "${CRYENGINE_DIR}/bin/win_x64/Sandbox.exe" "-project \"C:/dev/git/sf/SplashExample/SplashExamplePlugin.cryproject\"")
endif()

add_library(GameServer STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
set_target_properties(GameServer PROPERTIES LINKER_LANGUAGE CXX)
if (WIN32)
    set_visual_studio_debugger_command(GameServer "${CRYENGINE_DIR}/bin/win_x64/Game_Server.exe" "-project \"C:/dev/git/sf/SplashExample/SplashExamplePlugin.cryproject\"")
endif()

set_solution_startup_target(GameLauncher)

if (WIN32)
    set_visual_studio_debugger_command( ${THIS_PROJECT} "${CRYENGINE_DIR}/bin/win_x64/GameLauncher.exe" "-project \"C:/dev/git/sf/SplashExample/SplashExamplePlugin.cryproject\"" )
endif()

#BEGIN-CUSTOM
set (USE_TEST_PLATFORM_PROJECT "FALSE")

set (TestPlatformRoot "C:/dev/git/sf/BLANKGAME/")
set (TestPlatformPath "${TestPlatformRoot}bin/win_x64/")
set (TestPlatformProject "${TestPlatformRoot}Game.cryproject")

# Default to this project, if no test platform project is used
if (NOT USE_TEST_PLATFORM_PROJECT)
	set (TestPlatformProject "${PROJECT_FILE}")
endif()

if(USE_TEST_PLATFORM_PROJECT)
	add_custom_command(TARGET ${THIS_PROJECT} POST_BUILD 
	  COMMAND "${CMAKE_COMMAND}" -E copy 
		 "$<TARGET_FILE:${THIS_PROJECT}>"
		 "${TestPlatformPath}/$<TARGET_FILE_NAME:${THIS_PROJECT}>" 
	  COMMENT "Copying to test platform directory")
endif(USE_TEST_PLATFORM_PROJECT)
#END-CUSTOM