
cmake_minimum_required(VERSION 3.9)

#language
enable_language(C CXX)  # because w use glad glfw, it use C .
# compiler settings
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

project(solution LANGUAGES C CXX)


set( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" ${CMAKE_MODULE_PATH} )
set (CMAKE_CXX_STANDARD 11)  # this is required to use gtkmm, as gtkmm use libsigc++ which requires c++11

# gtkmm
find_package(PkgConfig)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)  # it contains documentation
# Now the variables GTKMM_INCLUDE_DIRS, GTKMM_LIBRARY_DIRS and GTKMM_LIBRARIES 
# contain what you expect 
include_directories(  ${GTKMM_INCLUDE_DIRS} )
link_directories(${GTKMM_LIBRARY_DIRS})

# openGL 
find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS})

# glew 
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})

# #SFML 
# # Detect and add SFML
# set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
# #Find any version 2.X of SFML
# #See the FindSFML.cmake file for additional details and instructions
# find_package(SFML 2 REQUIRED network audio graphics window system)
# include_directories(${SFML_INCLUDE_DIR})

# glfw
find_package(glfw3  REQUIRED)

add_executable(demo3 glwindowmain.cpp glwindow.cpp) 

# set(EXT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/src)
# set(EXT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/include)
# add_library(glad STATIC ${EXT_SOURCE_DIR}/glad.c)
# target_include_directories(glad PRIVATE ${EXT_INCLUDE_DIR})
# set_target_properties(glad PROPERTIES LINKER_LANGUAGE CXX)
# target_include_directories(demo3 PRIVATE ${EXT_INCLUDE_DIR})


target_link_libraries(demo3 ${GTKMM_LIBRARIES})
# target_link_libraries(demo2 ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
target_link_libraries(demo3 glfw)  # we can choose glfw+glad or glfw+glew or glfw+gl3w.  glfw and freeglut, glut are same kind, used to manage wondow, opengl context, deal mouse key input
target_link_libraries(demo3 GLEW::GLEW)  # here we use glfw + glew. this one works in glarea. 
target_link_libraries(demo3 ${OPENGL_LIBRARIES}) # use glew, must use this OpenGL libarary. otherwise it has /usr/lib/x86_64-linux-gnu/libGL.so.1: error adding symbols: DSO missing ...




