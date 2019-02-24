cmake_minimum_required(VERSION 2.8)

project(glui)

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${GLUT_INCLUDE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/glui/include)

add_subdirectory(glui)
add_subdirectory(samples)
#add_executable(ppm2array tools/ppm.cpp tools/ppm2array.cpp)
#target_link_libraries(ppm2array)

