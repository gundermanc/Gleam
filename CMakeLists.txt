# Untitled 'Christian-Gunderman' project

cmake_minimum_required(VERSION 3.9)

#========== Global Configurations =============#
#----------------------------------------------#

project(Gleam)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Set the default build type to debug if none is specified.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "debug" CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

enable_testing()

add_executable(
    TextLibraryTests
    Test/Renderer/Text/ImmutableBTreeTests.cpp)
target_link_libraries(TextLibraryTests PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

add_test(TextLibraryTests TextLibrary)



#========== Targets Configurations ============#

# Build an executable (Unix-like OSes generates ./gleam, on
# Windows ./gleam.exe)
# ..........................................
add_executable(
    gleam WIN32 MACOSX_BUNDLE
    Renderer/Main.cpp)
target_link_libraries(gleam PRIVATE Utilities)
target_link_libraries(gleam PRIVATE RendererFramework)
target_link_libraries(gleam PRIVATE OpenGLRenderer)
target_link_libraries(gleam PRIVATE AppLibrary)

find_package(Freetype REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

# Build RenderFramework.. abstraction for the renderer.
add_library(
    RendererFramework
    Renderer/Framework/Controls/AbstractControl.cpp
    Renderer/Framework/Controls/TextSegment.cpp
    Renderer/Framework/AbstractGraphicsContext.cpp
    Renderer/Framework/AbstractViewport.cpp)

# Build OpenGLFramework.. the OpenGL version of the renderer.
add_library(
    OpenGLRenderer
    Renderer/OpenGLViewport/OpenGLGraphicsContext.cpp
    Renderer/OpenGLViewport/OpenGLViewport.cpp
    Renderer/OpenGLViewport/Text/mat4.c
    Renderer/OpenGLViewport/Text/OpenGLTextContext.cpp
    Renderer/OpenGLViewport/Text/shader.c)
target_link_libraries(OpenGLRenderer PRIVATE GLEW::GLEW)
target_link_libraries(OpenGLRenderer PRIVATE glfw)
target_link_libraries(OpenGLRenderer PRIVATE Freetype::Freetype)

# Build Text library.. library for opening and interacting with text and files.
add_library(
    TextLibrary
    Renderer/Text/ImmutableBTree.cpp
    Renderer/Text/TextDocument.cpp)

# Build Util library.. library for common shared bits.
add_library(
    Utilities
    Renderer/Utilities/FileLogger.cpp)

# Build Text box library.. library for displaying files.
add_library(
    TextBox
    Renderer/TextBox/TextBox.cpp)

# Build xplat app. Has no awareness of OpenGl, only the RendererFramework.
add_library(
    AppLibrary
    Renderer/App/CommandLineHelpView.cpp
    Renderer/App/DocumentView.cpp)
target_link_libraries(AppLibrary RendererFramework)
target_link_libraries(AppLibrary TextLibrary)
target_link_libraries(AppLibrary TextBox)

# Freetype-GL package doesn't seem to have right configuration for install to work.
if (MSVC)
    target_link_libraries(OpenGLRenderer PRIVATE "$ENV{VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/${CMAKE_BUILD_TYPE}/lib/freetype-gl.lib")
else()
    target_link_libraries(OpenGLRenderer PRIVATE "$ENV{VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/${CMAKE_BUILD_TYPE}/lib/libfreetype-gl.a")
endif (MSVC)

add_custom_command(TARGET gleam POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Content/ $<TARGET_FILE_DIR:gleam>/Content)

#============= Platform Specific ==============#
if (MSVC)
    # Tell MSVC to use main instead of WinMain for Windows subsystem executables
    set_target_properties(gleam PROPERTIES
                          LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()
