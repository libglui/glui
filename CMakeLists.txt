cmake_minimum_required(VERSION 2.8.12)

project(glui)
set(PROJECT_VERSION 2.37)

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set (CMAKE_CXX_FLAGS "--std=c++11 ${CMAKE_CXX_FLAGS}")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set (CMAKE_CXX_FLAGS "--std=c++11 ${CMAKE_CXX_FLAGS}")
endif ()

if (APPLE)
  set (CMAKE_CXX_FLAGS "-Wno-deprecated-declarations ${CMAKE_CXX_FLAGS}")
endif ()

SET(GLUI_SRCS
 algebra3.cpp
 arcball.cpp
 glui_add_controls.cpp
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
 glui_textbox.cpp
 glui_translation.cpp
 glui_tree.cpp
 glui_treepanel.cpp
 glui_window.cpp
 quaternion.cpp
 viewmodel.cpp
)
add_library(glui_obj OBJECT ${GLUI_SRCS})
target_include_directories(glui_obj
      PUBLIC
      "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
      ${OPENGL_INCLUDE_DIR}
      ${GLUT_INCLUDE_DIR})
# we need to enable -fPIC this so that the same object code can be used to
# create static *and* shared libraries without double compilation
set_target_properties( glui_obj PROPERTIES POSITION_INDEPENDENT_CODE 1)

add_library(glui SHARED $<TARGET_OBJECTS:glui_obj>)
target_include_directories(glui
      PUBLIC
      "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
      "$<INSTALL_INTERFACE:include>"
      ${OPENGL_INCLUDE_DIR}
      ${GLUT_INCLUDE_DIR})
target_link_libraries(glui PUBLIC ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
set_target_properties(glui PROPERTIES
  DEBUG_POSTFIX "d"
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION})

add_library(glui_static STATIC $<TARGET_OBJECTS:glui_obj>)
target_include_directories(glui_static
      PUBLIC
      "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
      "$<INSTALL_INTERFACE:include>"
      ${OPENGL_INCLUDE_DIR}
      ${GLUT_INCLUDE_DIR})
target_link_libraries(glui_static PUBLIC ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
set_target_properties(glui_static PROPERTIES DEBUG_POSTFIX "d")

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



####
# Installation

set(config_install_dir "lib/cmake/${PROJECT_NAME}")
set(include_install_dir "include")

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

# Configuration
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(targets_export_name "${PROJECT_NAME}Targets")
set(namespace "${PROJECT_NAME}::")

# Include module with fuction 'write_basic_package_version_file'
include(CMakePackageConfigHelpers)

# Configure '<PROJECT-NAME>ConfigVersion.cmake'
write_basic_package_version_file(
    "${version_config}" VERSION ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion
)

# Configure '<PROJECT-NAME>Config.cmake'
configure_package_config_file(
    "cmake/Config.cmake.in"
    "${project_config}"
    INSTALL_DESTINATION "${config_install_dir}"
)

# Targets:
install(
    TARGETS glui_static glui
    EXPORT "${targets_export_name}"
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "${include_install_dir}"
)

# Headers:
install(
    DIRECTORY "${CMAKE_SOURCE_DIR}/include/"
    DESTINATION "${include_install_dir}"
    FILES_MATCHING PATTERN "*.h"
)


# Config
install(
    FILES "${project_config}" "${version_config}"
    DESTINATION "${config_install_dir}"
)

# Config
install(
    EXPORT "${targets_export_name}"
    NAMESPACE "${namespace}"
    DESTINATION "${config_install_dir}"
)
