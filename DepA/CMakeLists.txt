####
# 'DepA' Deployment:
#
# Based on Ref/CMakeLists.txt
#
# This sets up the build for the 'DepA' Application, including the custom reference
# components. In addition, it imports FPrime.cmake, which includes the core F Prime
# components.
#
# This file has several sections.
#
# 1. Header Section: define basic properties of the build
# 2. F prime core: includes all F prime core components, and build-system properties
# 3. Local subdirectories: contains all deployment specific directory additions
####

##
# Section 1: Basic Project Setup
#
# This contains the basic project information. Specifically, a cmake version and
# project definition.
##
cmake_minimum_required(VERSION 3.13)
cmake_policy(SET CMP0048 NEW)
project(DepA VERSION 1.0.0 LANGUAGES C CXX)

##
# Section 2: F prime Core
#
# This includes all of the F prime core components, and imports the make-system. F prime core
# components will be placed in the F-Prime binary subdirectory to keep them from
# colliding with deployment specific items.
##
include("${FPRIME_FRAMEWORK_PATH}/cmake/FPrime.cmake")
# NOTE: register custom targets between these two lines
include("${FPRIME_FRAMEWORK_PATH}/cmake/FPrime-Code.cmake")

include_directories("${FPRIME_PROJECT_ROOT}")

add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../mesh_network-drivers/MultiTcpServer")
# Note: when building a deployment outside of the F prime core directories, then the
# build root must be re-mapped for use with the standard build system components.
#
# In this way, the module names can be predicted as an offset from the (new) build
# root, without breaking the standard locations of F prime.
#
# Uncomment the following lines, and set them to the BUILD_ROOT of your deployment,
# which is typically one directory up from the CMakeLists.txt in the deployment dir.
set(FPRIME_CURRENT_BUILD_ROOT "${FPRIME_PROJECT_ROOT}")
message(STATUS "F prime BUILD_ROOT currently set to: ${FPRIME_CURRENT_BUILD_ROOT}")
##
# Section 3: Components and Topology
#
# This section includes deployment specific directories. This allows use of non-
# core components in the topology, which is also added here.
##
# Add component subdirectories
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/SimplePort/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/SimpleProducer/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/SwarmFramer/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/SwarmDeframer/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/SimpleReceiver/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/sGPS/")
# Add Topology subdirectory
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Top/")

set(SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/Top/Main.cpp")
set(MOD_DEPS ${PROJECT_NAME}/Top)

register_fprime_deployment()
# The following compile options will only apply to the deployment executable.
# The extra warnings trigger in core F Prime so we don't apply them there.
target_compile_options("${PROJECT_NAME}" PUBLIC -Wall)
target_compile_options("${PROJECT_NAME}" PUBLIC -Wextra)
target_compile_options("${PROJECT_NAME}" PUBLIC -Werror)
#target_compile_options("${PROJECT_NAME}" PUBLIC -Wshadow)
target_compile_options("${PROJECT_NAME}" PUBLIC -Wconversion)
target_compile_options("${PROJECT_NAME}" PUBLIC -Wsign-conversion)
target_compile_options("${PROJECT_NAME}" PUBLIC -Wformat-security)
target_compile_options("${PROJECT_NAME}" PUBLIC -Wnon-virtual-dtor)
target_compile_options("${PROJECT_NAME}" PUBLIC -Wold-style-cast)
target_compile_options("${PROJECT_NAME}" PUBLIC -Woverloaded-virtual)
target_compile_options("${PROJECT_NAME}" PUBLIC -Wno-unused-parameter)
target_compile_options("${PROJECT_NAME}" PUBLIC -Wundef)
set_property(TARGET "${PROJECT_NAME}" PROPERTY CXX_STANDARD 11)
