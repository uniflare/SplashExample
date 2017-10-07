# Copyright (c) 2017, uniflare, see LICENSE.txt for details
# Make sure you set these variables appropriately! (If using a host project)
#
set (USE_TEST_PLATFORM_PROJECT "FALSE")

set (TestPlatformRoot "C:/dev/git/sf/BLANKGAME")
set (TestPlatformPath "${TestPlatformRoot}/bin/win_x64")
set (TestPlatformProject "${TestPlatformRoot}/Game.cryproject")

# Default to this project, if no test platform project is used
if (NOT USE_TEST_PLATFORM_PROJECT)
	set (TestPlatformProject "${PROJECT_FILE}")
endif()

set (ProjectName "SplashExample")

cmake_minimum_required (VERSION 3.6.0)
project (${ProjectName} VERSION 1.3 LANGUAGES CXX)

if (${CMAKE_GENERATOR} MATCHES "^Visual Studio")
	# Set startup project properties
	configure_file ("${PROJECT_NAME}.vcxproj.user.in" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.vcxproj.user")
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()

# Set output directory for executable files
if(CMAKE_CL_64)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}\\..\\..\\bin\\win_x64")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}\\..\\..\\bin\\win_x64_release")
else(CMAKE_CL_64)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}\\..\\..\\bin\\win_x86")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}\\..\\..\\bin\\win_x86_release")
endif(CMAKE_CL_64)

# Set output directory for DLL files
if(CMAKE_CL_64)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}\\..\\..\\bin\\win_x64")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}\\..\\..\\bin\\win_x64_release")
else(CMAKE_CL_64)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}\\..\\..\\bin\\win_x86")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}\\..\\..\\bin\\win_x86_release")
endif(CMAKE_CL_64)

# Set output directory for LIB files
if(CMAKE_CL_64)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}\\..\\..\\bin\\win_x64\\lib")
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}\\..\\..\\bin\\win_x64_release\\lib")
else(CMAKE_CL_64)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}\\..\\..\\bin\\win_x86\\lib")
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}\\..\\..\\bin\\win_x86_release\\lib")
endif(CMAKE_CL_64)

option(OPTION_PCH "Enable Precompiled headers" ON)

find_package (CryCommon REQUIRED)
find_package (CryAction REQUIRED)

set (SourceGroup_Main
	"SplashExample.cpp"
	"SplashExample.h"
	"Plugin.cpp"
	"Plugin.h"
	"CVars.h"
	"StdAfx.cpp"
	"StdAfx.h"
)
source_group("Main" FILES ${SourceGroup_Main})

set (SOURCE
	${SourceGroup_Main}
)
if(OPTION_PCH AND MSVC)
	set (PRECOMPILED_SOURCE ${SOURCE})
	list(REMOVE_ITEM PRECOMPILED_SOURCE "StdAfx.cpp")

	set (PRECOMPILED_BINARY ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pch)
	set_source_files_properties (${PRECOMPILED_SOURCE} PROPERTIES COMPILE_FLAGS "/Yu\"StdAfx.h\" /Fp\"${PRECOMPILED_BINARY}\"" OBJECT_DEPENDS "${PRECOMPILED_BINARY}")
	set_source_files_properties (StdAfx.cpp PROPERTIES COMPILE_FLAGS "/Yc\"StdAfx.h\" /Fp\"${PRECOMPILED_BINARY}\"" OBJECT_OUTPUTS "${PRECOMPILED_BINARY}")
endif (OPTION_PCH AND MSVC)

add_library(${PROJECT_NAME} MODULE ${SOURCE})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE CryCommon CryAction)

# Add _RELEASE define to release config
target_compile_definitions(${PROJECT_NAME}
  PUBLIC $<$<CONFIG:Release>:_RELEASE>)

if(WIN32)
	target_link_libraries(${PROJECT_NAME} PRIVATE Ntdll)
endif(WIN32)

if(USE_TEST_PLATFORM_PROJECT)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
	  COMMAND "${CMAKE_COMMAND}" -E copy 
		 "$<TARGET_FILE:${PROJECT_NAME}>"
		 "${TestPlatformPath}/$<TARGET_FILE_NAME:${PROJECT_NAME}>" 
	  COMMENT "Copying to test platform directory")
endif(USE_TEST_PLATFORM_PROJECT)