# GLUI

##  Dependencies

GLUI depends on OpenGL and GLUT. You can use a CMake with a minimum version is 2.8.12
to build the library.

## Building Instructions

In order to build the library you can do

```bash
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX:PATH=path/to/install
make install -j4
```

In order to use the library as a third party in a cmake project you can add this to your `CMakeLists.txt`:

```cmake
# Find the package from the gluiConfig.cmake
# in <prefix>/lib/cmake/glui/. Under the namespace glui::
# it exposes the target glui that allows you to compile
# and link with the library
find_package(glui CONFIG REQUIRED)
...
# suppose you want to try it out in a executable
add_executable(gluitest yourfile.cpp)
# add link to the library, use glui::glui_static if you want to link with the static version
target_link_libraries(gluitest PUBLIC glui::glui)
```
Check the `CMakeLists.txt` in the examples folder for an example.

When you install glui, a file `gluiConfig.cmake` is installed in `path/to/install/lib/cmake/glui/`
that allows you to import the library in your CMake project by passing the
location of `gluiConfig.cmake` from the cmake command line:

```bash
cmake .. -Dglui_DIR=path/to/install/lib/cmake/glui/
```
