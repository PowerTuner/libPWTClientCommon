cmake_minimum_required(VERSION 3.23)

project(PWTClientCommon VERSION 1.1 DESCRIPTION "Shared library for PowerTuner clients" LANGUAGES CXX)

option(DEV_BUILD_SETUP "Enable options to build the library stand-alone for development" OFF)

set(PROJECT_AUTHOR "kylon")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(LINK_LIBS "")

find_package(Qt6 6.10 REQUIRED COMPONENTS Core)

configure_file(pwtClientCommon/Resources/version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/version.h)

set(PROJECT_SOURCES
	pwtClientCommon/Include/clientcommonExport.h
	pwtClientCommon/Include/PWTWindowState.h

	pwtClientCommon/InputRanges/InputRanges.h
	pwtClientCommon/InputRanges/InputRanges.cpp

	pwtClientCommon/StartupUtil/StartupUtil.h

	pwtClientCommon/CommonUtils.h
	pwtClientCommon/CommonUtils.cpp
	pwtClientCommon/UILogger.h
	pwtClientCommon/UILogger.cpp
)

if (LINUX)
	list(APPEND PROJECT_SOURCES
		pwtClientCommon/StartupUtil/StartupUtilLinux.h
		pwtClientCommon/StartupUtil/StartupUtilLinux.cpp
	)
elseif (WIN32)
	configure_file(pwtClientCommon/Resources/Windows/win.rc.in ${CMAKE_CURRENT_SOURCE_DIR}/win.rc)

	list(APPEND PROJECT_SOURCES
		pwtClientCommon/StartupUtil/StartupUtilWindows.h
		pwtClientCommon/StartupUtil/StartupUtilWindows.cpp
		win.rc
	)

	list(APPEND LINK_LIBS
		PWT::WIN32
	)
endif ()

if (DEV_BUILD_SETUP)
	include(FetchContent)

	set(FETCH_SRC "")
	set(FETCHCONTENT_QUIET FALSE)

	FetchContent_Declare(
		libpwtshared
		GIT_REPOSITORY https://github.com/PowerTuner/libPWTShared
		GIT_TAG origin/main
		GIT_PROGRESS TRUE
	)

	if (WIN32)
		FetchContent_Declare(
			libpwtwin32
			GIT_REPOSITORY https://github.com/PowerTuner/libPWTWin32
			GIT_TAG origin/main
			GIT_PROGRESS TRUE
		)

		list(APPEND FETCH_SRC libpwtwin32)
	endif ()

	FetchContent_MakeAvailable(libpwtshared ${FETCH_SRC})
endif ()

add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})
add_library("PWT::ClientCommon" ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt::Core ${LINK_LIBS} PWT::Shared)
target_compile_definitions(${PROJECT_NAME} PRIVATE PWTCLIENTCOMMON_LIBRARY)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

include(CheckIPOSupported)
check_ipo_supported(RESULT has_ipo OUTPUT ipo_error)
if (has_ipo)
	message(STATUS "${PROJECT_NAME}: IPO/LTO enabled")
	set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION}
	CXX_VISIBLITY_PRESET hidden
	VISIBILITY_INLINE_HIDDEN YES
)
