# CMakeGen template
#
# This file is used by CMakeGen as a template for 
# generating the CMakeList.txt file required to 
# build your cryengine 5 projects.
#
# Tested with CryEngine Release branch version 5.3
#
# Make sure you set these variables appropriately!
#
set (TestPlatformRoot "C:/dev/git/sf/BLANKGAME")
set (TestPlatformPath "${TestPlatformRoot}/bin/win_x64")
set (TestPlatformProject "${TestPlatformRoot}/Game.cryproject")
set (ProjectName "SplashExample")

cmake_minimum_required (VERSION 3.6.0)
project (${ProjectName} VERSION 1.0 LANGUAGES CXX)

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
	"SplashExamplePlugin.cpp"
	"SplashExamplePlugin.h"
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

if (WIN32)
	target_link_libraries(${PROJECT_NAME} PRIVATE Ntdll)
endif(WIN32)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy 
     "$<TARGET_FILE:${PROJECT_NAME}>"
     "${TestPlatformPath}/$<TARGET_FILE_NAME:${PROJECT_NAME}>" 
  COMMENT "Copying to test platform directory")