# Untitled 'Christian-Gunderman' project
# Initially inspired by: https://gist.github.com/caiorss/fd9e61887e9b2575f2275a80af44fac2

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

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Set executable dependency libraries
# Equivalent to pass flags -lGL, -lGLU and -lglut 
#target_link_libraries(gleam GL)
target_link_libraries(gleam PRIVATE GLUT::GLUT)

# Add target to run executable 
add_custom_target(run
    COMMAND gleam
    DEPENDS gleam 
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)