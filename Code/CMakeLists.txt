cmake_minimum_required (VERSION 3.6.0)
project (Plugin VERSION 1.0 LANGUAGES CXX)

#set (TestPlatformPath DIRECTORY "C:/dev/git/sf/INZGAME")

if (${CMAKE_GENERATOR} MATCHES "^Visual Studio")
	# Set startup project properties
	configure_file ("Plugin.vcxproj.user.in" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.vcxproj.user")
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()

option(OPTION_PCH "Enable Precompiled headers" ON)

find_package (CryCommon REQUIRED)

set (SourceGroup_PluginInterface
	"Interface/ICryFreeVideoSystem.h"
	"Interface/ICryFreeVideoPlugin.h"
)
source_group("Interface" FILES ${SourceGroup_PluginInterface})

set (SourceGroup_PluginMain
	"StdAfx.cpp"
	"StdAfx.h"
	"Main/Plugin.cpp"
	"Main/Plugin.h"
)
source_group("Main" FILES ${SourceGroup_PluginMain})

set (SourceGroup_PluginSystem
	"System/CryFreeVideoSystem.cpp"
	"System/CryFreeVideoSystem.h"
)
source_group("System" FILES ${SourceGroup_PluginSystem})

set (SOURCE
	${SourceGroup_PluginInterface}
	${SourceGroup_PluginMain}
	${SourceGroup_PluginSystem}
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
     "C:/dev/git/sf/BLANKGAME/$<TARGET_FILE_NAME:${PROJECT_NAME}>" 
  COMMENT "Copying to test platform directory")