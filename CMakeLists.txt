# Untitled 'Christian-Gunderman' project

cmake_minimum_required(VERSION 3.9)

#========== Global Configurations =============#
#----------------------------------------------#

project(Gleam)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Set the default build type to Debug if none is specified.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

#========== Targets Configurations ============#

# Build an executable (Unix-like OSes generates ./gleam, on
# Windows ./gleam.exe)
# ..........................................
add_executable(
    gleam WIN32 MACOSX_BUNDLE
    Renderer/Main.cpp
    Renderer/Text/mat4.c
    Renderer/Text/shader.c)

find_package(Freetype REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

# Set executable dependency libraries
# Equivalent to pass flags -lGL, -lGLU and -lglut 
target_link_libraries(gleam PRIVATE GLEW::GLEW)
target_link_libraries(gleam PRIVATE glfw)
target_link_libraries(gleam PRIVATE Freetype::Freetype)

# Freetype-GL package doesn't seem to have right configuration for install to work.
target_link_libraries(gleam PRIVATE "$ENV{VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/${CMAKE_BUILD_TYPE}/lib/freetype-gl.lib")

add_custom_command(TARGET gleam POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Content/ $<TARGET_FILE_DIR:gleam>/Content)

#============= Platform Specific ==============#
if (MSVC)
    # Tell MSVC to use main instead of WinMain for Windows subsystem executables
    set_target_properties(gleam PROPERTIES
                          LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()
