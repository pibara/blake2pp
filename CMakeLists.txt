cmake_minimum_required(VERSION 2.6)
enable_testing()
project(mattock-ohash)
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
    add_library(mattock-ohash SHARED mattock-ohash.cpp)
  ELSE()
    message(FATAL_ERROR "Found the b2 library but not the relevant header files.")
  ENDIF()
ELSE()
  message(FATAL_ERROR "No b2 lib found.")
ENDIF()
SET_TARGET_PROPERTIES(mattock-ohash PROPERTIES VERSION 1.0.0 SOVERSION 1)
target_link_libraries(mattock-ohash b2)
install_targets(/lib mattock-ohash)
install(FILES mattock-ohash.hpp DESTINATION include)
