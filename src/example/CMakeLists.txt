cmake_minimum_required(VERSION 2.8.12)

project(GluiExamples)

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

find_package(glui REQUIRED)

add_executable(example1 example1.cpp)
target_link_libraries(example1 glui::glui_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
add_executable(example2 example2.cpp)
target_link_libraries(example2 glui::glui_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
add_executable(example3 example3.cpp)
target_link_libraries(example3 glui::glui_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
add_executable(example4 example4.cpp)
target_link_libraries(example4 glui::glui_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
add_executable(example5 example5.cpp)
target_link_libraries(example5 glui::glui_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
add_executable(example6 example6.cpp)
target_link_libraries(example6 glui::glui_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
