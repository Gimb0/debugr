cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")
project(debugr)
enable_testing()
set(CMAKE_C_FLAGS "${C_MAKE_FLAGS} -Wall -pedantic -Wextra -std=c99")
add_subdirectory(src)
add_subdirectory(test)
