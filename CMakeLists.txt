cmake_minimum_required(VERSION 2.8...3.5)

project(assignment1)

add_executable(Alien.out 
    Alien.cpp
    Model.cpp
    Animation.cpp
    Utils.cpp
    Global.cpp
)

set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries( Alien.out ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )
