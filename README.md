# gtkmm_openGL
# install gtkmm. 
https://www.gtkmm.org/en/download.html

apt-get install libgtkmm-3.0-dev

configure it in CMakeList.txt
# gtkmm
find_package(PkgConfig)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)  # it contains documentation
# Now the variables GTKMM_INCLUDE_DIRS, GTKMM_LIBRARY_DIRS and GTKMM_LIBRARIES 
# contain what you expect 
include_directories(  ${GTKMM_INCLUDE_DIRS} )
link_directories(${GTKMM_LIBRARY_DIRS})
target_link_libraries(demo3 ${GTKMM_LIBRARIES})



# openGL. 
# Two kinds of library:
one is managment of framework , openGL context, widnow, mouse, keyboard. such as glut(outdated), freeglut(used to replace glut), GLFW.
These toolkits are designed to create and manage OpenGL windows, and manage input, but little beyond that
https://en.wikipedia.org/wiki/OpenGL 

another one is for openGL loading library, such as Glad, glew.
https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library

We can use combinations like freeglut + glade. freeglut + glew. GLFW + glad, GLFW + glew. 
I tried GLFW + glad and GLFW + glew, so far only GLFW + glew works in gtkmm. 
when using glad or glew + GLFW, we must include glad or glew before #include <GLFW>. 
#include <glad/glad.h>
#include <GLFW/glfw3.h>
or 
#include <GL/glew.h> 
#include <GLFW/glfw3.h>

# glm

http://www.opengl-tutorial.org/beginners-tutorials/tutorial-1-opening-a-window/
We don’t actually need this one right now, but this is a library for 3D mathematics. It will prove very useful soon. There is no magic in GLM, 
you can write your own if you want; it’s just handy. The “using namespace” is there to avoid typing “glm::vec3”, but “vec3” instead.
// Include GLM
glm is simple, you don't need write CMakelists.txt, directly #include <glm/used_head.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

git clone https://github.com/g-truc/glm.git
cd glm
mkdir build && cd build
cmake ..
make -j8
#sudo make install   # other people used this step, but I did not use this, also got no error.



# ubuntu install GLFW
sudo apt-get install libglfw3
sudo apt-get install libglfw3-dev
or you can build from source 
git clone https://github.com/glfw/glfw.git
unzip glfw.zip
cd glfw
mkdir build && cd build
cmake ..
make -j8
sudo make install


configure it in cmakelists.txt
# if you have multiple glfw, you could specify its version number
#find_package(glfw3 3.3 REQUIRED) 
find_package(glfw3 REQUIRED) 
target_link_libraries(${execute_name} glfw)


# install glad.
go to glad.dav1d.de
set profile as core
api gl version >= 3.3
generate
download glad.zio
glade.zip has three files. 
unzip glad.zip
cd /usr/local/include && mkdir glad # create this folder to store the two head files
sudo mv glad.h /usr/local/include/glad # don't store at /usr/include , otherwise it contaminate the system files
sudo mv khrplatform.h /usr/local/include/glad
copy glad.c file to yourown project folder. 
in CMakeLists.txt you may 
enable_language(C CXX)  # because we use glad, glfw, it use C .
add_executable(${your execute name} glad.c) 


include GLEW, this one actually is a little bit magic, but let's leave this for later.
#include GLEW, always include it before gl.h and glfw3.h sincie it's a bit magic
# install glew.
sudo apt-get install libglew-dev
# or you can install from source.
$ sudo apt-get install build-essential libxmu-dev libxi-dev libgl-dev cmake git
$ git clone https://github.com/nigels-com/glew.git
$ cd glew
$ mkdir build
$ cd build
$ cmake ./cmake
$ make -j4

#configure it in cmakelists.txt
# glew
# openGL 
find_package(OpenGL REQUIRED)  # we may need this because include glew  below only cause 
include_directories( ${OPENGL_INCLUDE_DIRS})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})

target_link_libraries(demo3 GLEW::GLEW)  # here we use glfw + glew. this one works in glarea. 
target_link_libraries(demo3 ${OPENGL_LIBRARIES}) # use glew, must use this OpenGL libarary. otherwise it has /usr/lib/x86_64-linux-gnu/libGL.so.1: error adding symbols: DSO missing ...







