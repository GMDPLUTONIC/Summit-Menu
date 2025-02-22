cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

if ("${GEODE_TARGET_PLATFORM}" STREQUAL "iOS" OR IOS)
	set(CMAKE_OSX_ARCHITECTURES "arm64")
	add_definitions(-DGLES_SILENCE_DEPRECATION)
else()
	set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Building on linux, disabling SSE and using builtin offsetof")
    add_definitions(-D_CRT_USE_BUILTIN_OFFSETOF -DIMGUI_DISABLE_SSE)
endif()

if (DEFINED ENV{GITHUB_ACTIONS})
    message("Github Actions Build")
    add_definitions(-DGITHUB_ACTIONS)
endif()

if (DEFINED ENV{RELEASE_BUILD})
    message("Building for release!")
    add_definitions(-DRELEASE_BUILD)
else()
    message("Building for debug!")
    add_definitions(-DDEBUG_BUILD)
endif()

project(SummitMenu VERSION 1.0.0)

file(GLOB_RECURSE SOURCES src/*.cpp )

add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "src/internal" "include")

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

set(GEODE_DISABLE_PRECOMPILED_HEADERS ON)

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

# no imgui for now
# CPMAddPackage("gh:matcool/gd-imgui-cocos#0207a36")

target_link_libraries(${PROJECT_NAME}) # imgui-cocos)

setup_geode_mod(${PROJECT_NAME})
