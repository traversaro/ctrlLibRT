# Copyright: (C) 2014 Department of Robotics Brain and Cognitive Sciences - Istituto Italiano di Tecnologia
# Authors: Silvio Traversaro
# CopyPolicy: Released under the terms of the GNU GPL v2.0.
cmake_minimum_required(VERSION 2.6)
set(PROJECTNAME ctrlLibRT)

project(${PROJECTNAME})
find_package(YARP)
find_package(ICUB)
find_package(Eigen3)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${YARP_MODULE_PATH})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ICUB_MODULE_PATH})
#include(iCubHelpers)
set(folder_source filter_test.cpp filters.cpp filters.h)
source_group("Source Files" FILES ${folder_source})

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ICUB_LINK_FLAGS}")

include_directories(${ICUB_INCLUDE_DIRS} ${YARP_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIR})
add_executable(${PROJECTNAME} ${folder_source})
target_link_libraries(${PROJECTNAME} ctrlLib ${YARP_LIBRARIES})
