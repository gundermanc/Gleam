# Untitled 'Christian-Gunderman' project

cmake_minimum_required(VERSION 3.9)

#========== Global Configurations =============#
#----------------------------------------------#

project(Gleam)
set(CMAKE_CXX_STANDARD 17)     
set(CMAKE_VERBOSE_MAKEFILE ON)

#========== Targets Configurations ============#

# Build an executable (Unix-like OSes generates ./openglDemo1, on
# Windows ./opengDemo1.exe)
# ..........................................
add_executable(gleam Renderer/Main.cpp)

#find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)


# Set executable dependency libraries
# Equivalent to pass flags -lGL, -lGLU and -lglut 
#target_link_libraries(gleam PRIVATE GL)
target_link_libraries(gleam PRIVATE glad::glad)
target_link_libraries(gleam PRIVATE glfw)

# Add target to run executable 
add_custom_target(run
    COMMAND gleam
    DEPENDS gleam 
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)