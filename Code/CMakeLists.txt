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
set (TestPlatformPath "C:/dev/git/sf/BLANKGAME")
set (TestPlatformProject "${TestPlatformPath}/Game.cryproject")
set (ProjectName "Plugin")

cmake_minimum_required (VERSION 3.6.0)
project (${ProjectName} VERSION 1.0 LANGUAGES CXX)

if (${CMAKE_GENERATOR} MATCHES "^Visual Studio")
	# Set startup project properties
	configure_file ("${PROJECT_NAME}.vcxproj.user.in" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.vcxproj.user")
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()

option(OPTION_PCH "Enable Precompiled headers" ON)

find_package (CryCommon REQUIRED)

set (SourceGroup_Helpers
	"Helpers/debug.h"
)
source_group("Helpers" FILES ${SourceGroup_Helpers})

set (SourceGroup_Interface
	"Interface/ISplashExample.h"
	"Interface/ISplashExamplePlugin.h"
)
source_group("Interface" FILES ${SourceGroup_Interface})

set (SourceGroup_Main
	"Plugin.cpp"
	"Plugin.h"
	"StdAfx.cpp"
	"StdAfx.h"
)
source_group("Main" FILES ${SourceGroup_Main})

set (SourceGroup_System
	"System/SplashExample.cpp"
	"System/SplashExample.h"
)
source_group("System" FILES ${SourceGroup_System})

set (SOURCE
	${SourceGroup_Helpers}
	${SourceGroup_Interface}
	${SourceGroup_Main}
	${SourceGroup_System}
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
target_link_libraries(${PROJECT_NAME} PRIVATE CryCommon)

if (WIN32)
	target_link_libraries(${PROJECT_NAME} PRIVATE Ntdll)
endif(WIN32)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy 
     "$<TARGET_FILE:${PROJECT_NAME}>"
     "${TestPlatformPath}/$<TARGET_FILE_NAME:${PROJECT_NAME}>" 
  COMMENT "Copying to test platform directory")
  
  macro(copy_files GLOBPAT DESTINATION)
  file(GLOB COPY_FILES
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ${GLOBPAT})
  add_custom_target(copy ALL
    COMMENT "Copying files: ${GLOBPAT}")

  foreach(FILENAME ${COPY_FILES})
    set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}")
    set(DST "${DESTINATION}/${FILENAME}")

    add_custom_command(
      TARGET copy
      COMMAND ${CMAKE_COMMAND} -E copy ${SRC} ${DST}
      )
  endforeach(FILENAME)
endmacro(copy_files)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy 
     "$<TARGET_FILE:${PROJECT_NAME}>"
     "${TestPlatformPath}/$<TARGET_FILE_NAME:${PROJECT_NAME}>" 
  COMMENT "Copying to test platform directory")