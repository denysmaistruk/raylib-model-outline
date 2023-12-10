cmake_minimum_required(VERSION 3.18)

project(raylib_model_outline VERSION 1.0)

# project config
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "raylib_model_outline")

add_subdirectory(extern/raylib)

list(APPEND EXTRA_LIBS raylib)
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/extern/raylib/src")

file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/*.fs)

list(APPEND SOURCE_FILES ${PROJECT_SOURCE_DIR}/main.cpp)

add_executable(raylib_model_outline ${SOURCE_FILES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})

target_link_libraries(raylib_model_outline PUBLIC ${EXTRA_LIBS})
