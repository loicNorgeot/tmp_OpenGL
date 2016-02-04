#Install GLFW with cmake .. -DCMAKE_INSTALL_PREFIX=~/ -DBUILD_SHARED_LIBS=1

################################################################
# INITIALIZATION
################################################################
cmake_minimum_required(     VERSION 2.8)
set( CMAKE_BUILD_TYPE       "Release")
set( CMAKE_MACOSX_RPATH     1)
project( Minimal )
set( CMAKE_CXX_FLAGS        "-std=c++0x -g3 -O2 -w ")
include_directories(        "~/include/" "include/")

################################################################
#Dependencies
################################################################
find_library( common NAMES Commons HINTS "$ENV{HOME}/lib")
find_library( glfw   NAMES glfw    HINTS "$ENV{HOME}/lib")
find_library( glew   NAMES GLEW    HINTS "$ENV{HOME}/lib")
find_package( OpenGL REQUIRED)
find_package( X11    REQUIRED)
set(CORELIBS ${common} ${glfw} ${OPENGL_LIBRARY} ${X11_LIBRARIES} ${glew})

################################################################
#Library
################################################################
file(GLOB_RECURSE source_files sources/* include/*)
add_library(OGL SHARED ${source_files})
target_link_libraries(OGL ${CORELIBS})

################################################################
#Executable
################################################################
add_executable(        cube main.cpp)
target_link_libraries( cube OGL)
install(TARGETS cube RUNTIME DESTINATION "$ENV{HOME}/bin")
