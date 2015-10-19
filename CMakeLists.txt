cmake_minimum_required(VERSION 2.6)
enable_testing()
project(blake2)
set(prefix      ${CMAKE_INSTALL_PREFIX})
set(exec_prefix ${CMAKE_INSTALL_PREFIX})
set(libdir      ${CMAKE_INSTALL_PREFIX}/lib)
set(includedir  ${CMAKE_INSTALL_PREFIX}/include)
ADD_DEFINITIONS( -Wall -Werror -Weffc++ -Wctor-dtor-privacy -Wuninitialized -Winit-self -Wmissing-include-dirs -Wfloat-equal -Wshadow -Wpointer-arith )
SET(B2_INC_SEARCHPATH
  /usr/include
  /usr/local/include
)
FIND_PATH(B2_INCLUDE_PATH 
          NAMES blake2.h 
          PATHS ${B2_INC_SEARCHPATH})
SET(B2_LIB_SEARCHPATH
  /usr/lib
  /usr/local/lib
)
FIND_LIBRARY(B2_LIBRARY b2 ${B2_LIB_SEARCHPATH})
IF(B2_LIBRARY)
  IF(B2_INCLUDE_PATH)
    add_library(blake2 SHARED blake2.cpp)
  ELSE()
    message(FATAL_ERROR "Found the b2 library but not the relevant header files.")
  ENDIF()
ELSE()
  message(FATAL_ERROR "No b2 lib found.")
ENDIF()
SET_TARGET_PROPERTIES(blake2 PROPERTIES VERSION 1.0.0 SOVERSION 1)
target_link_libraries(blake2 b2)
install_targets(/lib blake2)
install(FILES blake2.hpp DESTINATION include)
