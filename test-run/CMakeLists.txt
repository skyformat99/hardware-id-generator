cmake_minimum_required (VERSION 3.5) 

project( test_exec )

add_executable( test_exec main.cpp )

set( CMAKE_CXX_FLAGS "-std=c++11" )

find_library( HARDWARE_ID_LIB "HardwareIdGenerator" "${CMAKE_SOURCE_DIR}/bin/" )
TARGET_LINK_LIBRARIES( test_exec ${HARDWARE_ID_LIB} )
