#
#	@author: Omar Moreno <omoreno1@ucsc.edu>
#	@date: December 06, 2013
#
#       Rewrite: July 18, 2016 - Maurik <holtrop@jlab.org>
#
# Specify the minimum CMake version required.  If the cmake version 
# minimum is not met, cmake will fail.
cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

cmake_policy(SET CMP0005 NEW)

if( CMAKE_VERSION VERSION_GREATER 3.0)
    cmake_policy(SET CMP0042 NEW)
endif()

# Set the name of the project
project(HPS_DST)

#
# Set a default build type if none was specified
set(default_build_type "RELEASE")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()


# Set the project version
set(HPS_DST_VERSION_MAJOR 1)
set(HPS_DST_VERSION_MINOR 2)

#############################################
#
# Sane RPATH handling. See: https://cmake.org/Wiki/CMake_RPATH_handling
#
############################################
#
# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF("${isSystemDir}" STREQUAL "-1")


# Check that the compiler version is OK, must have C++11
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # require at least gcc 4.8
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8)
        message(FATAL_ERROR "GCC version must be at least 4.8!")
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # require at least clang 3.2
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.2)
        message(FATAL_ERROR "Clang version must be at least 3.2!")
    endif()
else()
    message(WARNING "You are using an unsupported compiler! Compilation has only been tested with Clang and GCC.")
endif()


# Set additional compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++11")

# Set the output directories of the binaries, static and shared libraries

IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT) 
        SET(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR} CACHE PATH "HPS DST install prefix, default is project top dir." FORCE) 
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT) 


if(NOT DEFINED HPS_DST_BINARY_DIR )  # For backward compatibility with old CMakeFiles.txt
  SET(HPS_DST_BINARY_DIR  ${CMAKE_CURRENT_BINARY_DIR} CACHE PATH "Build location for lib and bin directories. " FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${HPS_DST_BINARY_DIR}/bin" CACHE PATH "Build location of binary executatbles.")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${HPS_DST_BINARY_DIR}/lib" CACHE PATH "Build location of shared libraries.")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${HPS_DST_BINARY_DIR}/lib" CACHE PATH "Build location of static libraries.")

set(CMAKE_INSTALL_LIBDIR           "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Location of libraries and pcm files. ")
set(CMAKE_INSTALL_BINDIR           "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Location of binary executable files. ")

option(HPS_INSTALL_ALL_LIBS   "Set to ON if you want to install all the libs" OFF)

# Set the location of the LCIO root directory.
set(LCIO_ROOT_DIR "$ENV{LCIO}" CACHE PATH "Location of the LCIO installation. ")

message(STATUS "Using LCIO from ${LCIO_ROOT_DIR}")

# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT

if( NOT DEFINED ENV{ROOTSYS} ) 
    message( FATAL_ERROR  "You need to setup ROOT first. ")
else()
    message(STATUS "Using ROOT from $ENV{ROOTSYS} ")
endif()

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS} )

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS MathCore RIO Hist Tree Net)

#---Overwrite some of the ROOT_DEFINITIONS since these are way too pedantic.
set(ROOT_DEFINITIONS "")

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

# Include the LCIO headers
include_directories(${LCIO_ROOT_DIR}/include)

# Find the LCIO libraries
find_library(LCIO_LIB lcio ${LCIO_ROOT_DIR}/lib)
if(NOT LCIO_LIB)
	message(FATAL_ERROR
		"\nThe library liblcio was not found.  "
		"Please make sure LCIO has been installed."
		"Then specify -DLCIO_ROOT_DIR=<path> or "
		"set the environmental variable LCIO=<path> "
		"See documentation for details.")
endif()

message(STATUS "LCIO: ${LCIO_LIB}")

# Include the HPS Event headers
set(HPS_EVENT_HDR_DIR "${PROJECT_SOURCE_DIR}/include/hps_event")
include_directories("${HPS_EVENT_HDR_DIR}")

# Include DST headers
set(DST_HDR_DIR "${PROJECT_SOURCE_DIR}/include/dst")
include_directories("${DST_HDR_DIR}")

# Include Utils headers
set(UTILS_HDR_DIR "${PROJECT_SOURCE_DIR}/include/utils")
include_directories("${UTILS_HDR_DIR}")

# Add the hps_event subdirectory to the build
#add_subdirectory("${CMAKE_SOURCE_DIR}/src/hps_event")
add_subdirectory("src/hps_event")

# Add the utils directory to the build
#add_subdirectory("${CMAKE_SOURCE_DIR}/src/utils")
add_subdirectory("src/utils")

# Add dst directory to the build
#add_subdirectory("${CMAKE_SOURCE_DIR}/src/dst")
add_subdirectory("src/dst")

#------------------------------#
#--- Generate Documentation ---#
#------------------------------#

# Find Doxygen
find_package(Doxygen)

# If Doxygen is found, add a target to generate the documentation
if(DOXYGEN_FOUND)
	configure_file(doc/Doxyfile ${HPS_DST_BINARY_DIR}/doc/Doxyfile @ONLY)
	add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${HPS_DST_BINARY_DIR}/doc/Doxyfile WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/doc COMMENT "Generating documentation" VERBATIM)
else()
	MESSAGE(STATUS "Doxygen was not found.  Documentation will not be generated.")
endif(DOXYGEN_FOUND)

message("-----------------------------------------------")
message("* Configuration Summary: ")
message("*")
message("* ROOT version: ${ROOT_VERSION} ")
message("* ROOT location: $ENV{ROOTSYS} ")
message("* LCIO location: ${LCIO_ROOT_DIR} ")
message("* Compiler ID:  ${CMAKE_CXX_COMPILER_ID}")
message("* Compiler Ver: ${CMAKE_CXX_COMPILER_VERSION}")

if(HPS_INSTALL_ALL_LIBS)
  message("* Will install all the libraries with 'make install' ")
else()
  message("* Will install only libHpsEvent with 'make install' ")
endif()

message("*")
message("-----------------------------------------------")


