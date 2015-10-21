cmake_minimum_required(VERSION 2.6)
enable_testing()
project(mattock-ohashi_demo)
include_directories(".")
set(prefix      ${CMAKE_INSTALL_PREFIX})
set(exec_prefix ${CMAKE_INSTALL_PREFIX})
set(libdir      ${CMAKE_INSTALL_PREFIX}/lib)
set(includedir  ${CMAKE_INSTALL_PREFIX}/include)
ADD_DEFINITIONS( -g -Wall -Werror -Weffc++ -Wctor-dtor-privacy -Wuninitialized -Winit-self -Wmissing-include-dirs -Wfloat-equal -Wshadow -Wpointer-arith )
SET(INC_SEARCHPATH
  /usr/include
  /usr/local/include
)
FIND_PATH(B2_INCLUDE_PATH 
          NAMES blake2.h 
          PATHS ${INC_SEARCHPATH})
SET(LIB_SEARCHPATH
  /usr/lib
  /usr/local/lib
)
FIND_LIBRARY(B2_LIBRARY b2 ${LIB_SEARCHPATH})
IF(B2_LIBRARY)
  IF(B2_INCLUDE_PATH)
    message(STATUS "Found b2 library and header.")
  ELSE()
    message(FATAL_ERROR "Found the b2 library but not the relevant header files.")
  ENDIF()
ELSE()
  message(FATAL_ERROR "No b2 lib found.")
ENDIF()
FIND_PATH(SHA_INCLUDE_PATH 
          NAMES crypto++/sha.h 
          PATHS ${INC_SEARCHPATH})
IF(SHA_INCLUDE_PATH)
  message(STATUS "Found Crypto++ SHA header.")
ELSE()
  message(FATAL_ERROR "No Crypto++ SHA header found.")
ENDIF()


add_executable(sequentialread sequentialread.cpp)
target_link_libraries(sequentialread b2 crypto++)
add_test(sequentialread sequentialread)

add_executable(sequentialwrite sequentialwrite.cpp)
target_link_libraries(sequentialwrite b2 crypto++)
add_test(sequentialwrite sequentialwrite)

add_executable(jumpingread jumpingread.cpp)
target_link_libraries(jumpingread b2 crypto++)
add_test(jumpingread jumpingread)

add_executable(jumpingwrite jumpingwrite.cpp)
target_link_libraries(jumpingwrite b2 crypto++)
add_test(jumpingwrite jumpingwrite)

add_executable(sparsewrite sparsewrite.cpp)
target_link_libraries(sparsewrite b2 crypto++)
add_test(sparsewrite sparsewrite)

install(FILES mattock-ohash.hpp DESTINATION include)
