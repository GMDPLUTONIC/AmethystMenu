cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(AmethystMenu VERSION 1.0.0)

# Recursively search for all .cpp files in the src directory
file(GLOB_RECURSE SOURCE_FILES "src/*")

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

# Ensure CPM is available
if (NOT CPM_SOURCE_CACHE)
    set(CPM_SOURCE_CACHE "${CMAKE_CURRENT_BINARY_DIR}/cpm-cache")
endif()

# Add imgui-cocos package
CPMAddPackage(
    NAME imgui-cocos
    GITHUB_REPOSITORY matcool/gd-imgui-cocos
    GIT_TAG 09a9555
)

# Add imgui package
CPMAddPackage(
    NAME imgui
    GITHUB_REPOSITORY ocornut/imgui
    GIT_TAG 6864a7f
)

# Include directories for imgui-cocos and imgui
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${imgui-cocos_SOURCE_DIR}
    ${imgui_SOURCE_DIR}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} imgui-cocos)
target_link_libraries(${PROJECT_NAME} imgui)

setup_geode_mod(${PROJECT_NAME})
