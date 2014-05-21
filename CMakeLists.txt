project(gittin)
cmake_minimum_required(VERSION 2.8.9)

option(ENABLE_MULTIARCH "Enable support for multi-arch distros (lib64)" OFF)
IF(ENABLE_MULTIARCH)
  IF(CMAKE_SIZEOF_VOID_P EQUAL 4)
    SET(LIB_SUFFIX "")
  ELSE(CMAKE_SIZEOF_VOID_P EQUAL 4)
    SET(LIB_SUFFIX 64)
  ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 4)
ELSE()
  SET(LIB_SUFFIX "")
ENDIF()
message(STATUS "Multiarch support is ${ENABLE_MULTIARCH}")
set(DST_LIB lib${LIB_SUFFIX})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

configure_file(gittin.pc.in gittin.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/gittin.pc DESTINATION ${DST_LIB}/pkgconfig)

# Find the QtWidgets library
find_package(Qt5Core 5.2 REQUIRED)

set(CMAKE_CXX_FLAGS "-Wall -g -std=c++0x")

#-Wno-psabi is to remove next g++ warning/note:
#the mangling of 'va_list' has changed in GCC 4.4
set(CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS} -Wno-psabi")

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(src)
add_subdirectory(examples)
