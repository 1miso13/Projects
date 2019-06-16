# Photon Splatting Using a View-Sample Cluster Hierarchy
# Brno University of Technology (BUT)
# Faculty of Information Technologies (FIT)
# Author: Marcel Kiss, xkissm01@stud.fit.vutbr.cz

# Universal CMakeLists.txt for root directory
# - Compile all sources recursively in source directory
# - Make Library target for external libs and links to project

# Minimum version of CMake
cmake_minimum_required (VERSION 3.0.2)

set(CMAKE_CXX_STANDARD 14)

# Set project name
set(PROJECT VGE)
# Set project name
set(SOURCE_DIR "src")

set(LIBRARY_SOURCE_DIR "libs")
set(LIBRARY_INCLUDE_DIR "include")

set(SOURCE_EXTENSIONS cpp c cxx cc)
set(HEADER_EXTENSIONS hpp h)
set(RESOURCE_EXTENSIONS glsl cl)

# Get file names recursively with given extensions relative to path
# Usage: get_files_ext(SRC_FILES "src" cpp c cxx)
macro(get_ext_files result dir)
	set(extensions "${ARGN}")
	foreach(ext ${extensions})
		file(GLOB_RECURSE files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${dir}/[^.]*.${ext}")	
		list(APPEND ${result} "${files}")
	endforeach()	
endmacro()

# Get list of directories recursively containing at least one file with one of given extension
# Usage: get_ext_directories(HEADER_DIRS "src" hpp h)
macro(get_ext_directories return_list dir)
	set(extensions "${ARGN}")
	set(new_list "")
	get_ext_files(new_list "${dir}" "${extensions}")
    set(dir_list "")
    foreach(file_path ${new_list})
        get_filename_component(dir_path ${file_path} PATH)
        set(dir_list ${dir_list} ${dir_path})
	endforeach()
    list(REMOVE_DUPLICATES dir_list)
    set(${return_list} ${dir_list})
endmacro()



# Use project name
project(${PROJECT})

get_ext_files(SOURCES "${SOURCE_DIR}" "${SOURCE_EXTENSIONS}")
get_ext_files(HEADERS "${SOURCE_DIR}" "${HEADER_EXTENSIONS}")
get_ext_files(RESOURCES "${SOURCE_DIR}" "${RESOURCE_EXTENSIONS}")

get_ext_directories(QHEADER_DIRS "${SOURCE_DIR}" "${HEADER_EXTENSIONS}")

get_ext_files(LIBRARY_SOURCES "${LIBRARY_SOURCE_DIR}" "${SOURCE_EXTENSIONS}")
get_ext_files(LIBRARY_HEADERS "${LIBRARY_INCLUDE_DIR}" "${HEADER_EXTENSIONS}")
get_ext_directories(LIBRARY_HEADER_DIRS "${LIBRARY_INCLUDE_DIR}" "${HEADER_EXTENSIONS}")
set(HEADER_DIRS "${LIBRARY_INCLUDE_DIR}")

message("${LIBRARY_SOURCES}")

include_directories("${QHEADER_DIRS}")       # For files: #include "file.h"
include_directories(SYSTEM "${HEADER_DIRS}") # For files: #include <file.h>



if(APPLE)
	# macOS Mojave has problem with SDL2:
	#     /usr/local/include/SDL2/SDL_syswm.h:66:10: fatal error: 'X11/Xlib.h' file not found
	# Add system include direcotry to solve this error:
	include_directories(SYSTEM AFTER "/opt/X11/include")
endif()

#message(STATUS "SOURCES: ${SOURCES}\n")
#message(STATUS "HEADERS: ${HEADERS}\n")
#message(STATUS "COPY-RESOURCES: ${RESOURCES}\n")
#message(STATUS "QINCLUDE-DIRS: ${QHEADER_DIRS}\n")
#message(STATUS "INCLUDE-DIRS: ${HEADER_DIRS}\n")

# Header only library, therefore INTERFACE
add_library(Libraries STATIC "${LIBRARY_SOURCES}" "${LIBRARY_HEADERS}")
# Prepend current directory to header dir list to make them absolute
list(TRANSFORM LIBRARY_HEADER_DIRS PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
target_include_directories(Libraries PUBLIC "${LIBRARY_HEADER_DIRS}")

target_compile_definitions(Libraries PRIVATE CL_SILENCE_DEPRECATION=1 RR_USE_OPENCL=1 USE_OPENCL=1 CALC_STATIC_LIBRARY=1 RR_EMBED_KERNELS=1)



# Setup target
add_executable(${PROJECT} "${SOURCES}" "${HEADERS}" "${RESOURCES}")

# Copy all ${RESOURCES} to target file dir
foreach(resource ${RESOURCES})
	add_custom_command(TARGET ${PROJECT} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/${resource}" $<TARGET_FILE_DIR:${PROJECT}>)
endforeach()

# macOS has deprecated OpenCL and OpenGL libraries
target_compile_definitions(${PROJECT} PRIVATE CL_SILENCE_DEPRECATION=1 GL_SILENCE_DEPRECATION=1 FREEIMAGE_LIB=1)


# set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-tautological-undefined-compare")
#target_compile_options(Libraries PRIVATE -Wtautological-undefined-compare)


# Set CMake module path (directory: "CMakeModules")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLM REQUIRED)
if (WIN32)
	find_package(GLEW REQUIRED) # Required for Windows
endif (WIN32)

include_directories(${OpenGL_INCLUDE_DIRS})
include_directories(${OpenCL_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIRS})

target_link_libraries(${PROJECT} Libraries)
if ("${SDL2_CONFIG}" STREQUAL "")
    message("SDL2 Old way!")
    target_link_libraries(${PROJECT} ${OPENGL_LIBRARIES} ${OpenCL_LIBRARIES} ${GLM_LIBRARY} ${SDL2_LIBRARY} ${GLEW_LIBRARY})
else()
    target_link_libraries(${PROJECT} ${OPENGL_LIBRARIES} ${OpenCL_LIBRARIES} ${GLM_LIBRARY} SDL2::SDL2 ${GLEW_LIBRARY})
endif()

target_compile_features(${PROJECT} PRIVATE cxx_range_for)

