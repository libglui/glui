cmake_minimum_required(VERSION 2.8)

project(glui)

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${GLUT_INCLUDE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

SET(GLUI_SRCS
 algebra3.cpp
 arcball.cpp
 glui_add_controls.cpp
 glui_bitmap_img_data.cpp
 glui_bitmaps.cpp
 glui_button.cpp
 glui_checkbox.cpp
 glui_column.cpp
 glui_commandline.cpp
 glui_control.cpp
 glui.cpp
 glui_edittext.cpp
 glui_filebrowser.cpp
 glui_listbox.cpp
 glui_list.cpp
 glui_mouse_iaction.cpp
 glui_node.cpp
 glui_panel.cpp
 glui_radio.cpp
 glui_rollout.cpp
 glui_rotation.cpp
 glui_scrollbar.cpp
 glui_separator.cpp
 glui_spinner.cpp
 glui_statictext.cpp
 glui_string.cpp
 glui_textbox.cpp
 glui_translation.cpp
 glui_tree.cpp
 glui_treepanel.cpp
 glui_window.cpp
 quaternion.cpp
 viewmodel.cpp
)
add_library(glui_obj OBJECT ${GLUI_SRCS})
# we need to enable -fPIC this so that the same object code can be used to
# create static *and* shared libraries without double compilation
set_property(TARGET glui_obj PROPERTY POSITION_INDEPENDENT_CODE 1)

add_library(glui SHARED $<TARGET_OBJECTS:glui_obj>)
target_link_libraries(glui ${GLUT_LIBRARIES})

add_library(glui_static STATIC $<TARGET_OBJECTS:glui_obj>)
target_link_libraries(glui_static ${GLUT_LIBRARIES})

add_executable(ppm2array tools/ppm.cpp tools/ppm2array.cpp)
target_link_libraries(ppm2array)

add_executable(example1 example/example1.cpp)
target_link_libraries(example1 glui_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
add_executable(example2 example/example2.cpp)
target_link_libraries(example2 glui_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
add_executable(example3 example/example3.cpp)
target_link_libraries(example3 glui_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
add_executable(example4 example/example4.cpp)
target_link_libraries(example4 glui_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
add_executable(example5 example/example5.cpp)
target_link_libraries(example5 glui_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
add_executable(example6 example/example6.cpp)
target_link_libraries(example6 glui_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
