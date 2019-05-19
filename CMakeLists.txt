cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

####This is a hook for developers to use.
####Please have it print out what changes it makes via the CMake's message API.
include(developer.cmake OPTIONAL)

project(glui VERSION 3.0.0 LANGUAGES CXX)
set(PROJECT_VERSION 3.00)

message(STATUS "Compiling GLUI Library Version ${PROJECT_VERSION}")
message(STATUS "Using cmake version ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" )

message(STATUS "Installing to ${CMAKE_INSTALL_PREFIX}")

####CMake-GUI options##########################

#These names need to stick out when building the external-libs projects.
option(GLUI_BUILD_EXAMPLES "Include glui_example to launch demo" ON)
option(GLUI_READLINE_MODIFIERS "Restore Unix's Readline key bindings" OFF)
option(GLUI_DEBUG_STDOUT "Restore (dump) debug output feature" OFF)

if(GLUI_BUILD_EXAMPLES)
	add_definitions(-DGLUI_BUILD_EXAMPLES)
endif()
if(GLUI_READLINE_MODIFIERS)
	add_definitions(-DGLUI_READLINE_MODIFIERS)
endif()
if(GLUI_DEBUG_STDOUT)
	add_definitions(-DGLUI_DEBUG_STDOUT)
endif()

#Is this good enough for static builds??
if(WIN32 OR CYGWIN)
	add_definitions(-DGLUI_DLL)
endif()

####BUILD/INSTALL###############################

string(APPEND CMAKE_CXX_FLAGS_DEBUG " -D_DEBUG")
string(APPEND CMAKE_CXX_FLAGS_RELEASE " -DNDEBUG")
string(APPEND CMAKE_CXX_FLAGS_MINSIZEREL " -DNDEBUG")
string(APPEND CMAKE_CXX_FLAGS_RELWITHDEBINFO " -DNDEBUG")

if(CMAKE_COMPILER_IS_GNUCXX)
add_compile_options(-frtti -fvisibility=hidden -fpermissive)
endif()


find_package(X11)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

set(glui_libs ${X11_LIBRARIES} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
set(glui_incl ${X11_INCLUDE_DIR} ${GLUT_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})

file(GLOB glui_files "src/*.cpp" "src/example/glui_*" "include/*.*" "include/GL/*.*")

add_library(glui_obj OBJECT ${glui_files})
target_include_directories(glui_obj
      PUBLIC
      "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
      ${glui_incl})
# we need to enable -fPIC this so that the same object code can be used to
# create static *and* shared libraries without double compilation
set_target_properties(glui_obj PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

add_library(glui SHARED $<TARGET_OBJECTS:glui_obj>)
target_include_directories(glui
      PUBLIC
      "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
      "$<INSTALL_INTERFACE:include>"
      ${glui_incl})
target_link_libraries(glui PUBLIC ${glui_libs})
set_target_properties(glui PROPERTIES
  DEBUG_POSTFIX "d"
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION})

add_library(glui_static STATIC $<TARGET_OBJECTS:glui_obj>)
target_include_directories(glui_static
      PUBLIC
      "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
      "$<INSTALL_INTERFACE:include>"
      ${glui_incl})
target_link_libraries(glui_static PUBLIC ${glui_libs})
set_target_properties(glui_static PROPERTIES DEBUG_POSTFIX "d")


file(GLOB ppm2array_files "src/ppm2array/*.cpp" "src/ppm2array/*.h")

add_executable(ppm2array ${ppm2array_files})
target_link_libraries(ppm2array)


if(GLUI_BUILD_EXAMPLES)

	file(GLOB example_files "src/example/example.*")
	set_source_files_properties(example.rc PROPERTIES LANGUAGE RC)

	add_executable(example ${example_files})
	target_link_libraries(example glui_static ${glui_libs})

endif()

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

# Include module with function 'write_basic_package_version_file'
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
