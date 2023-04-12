cmake_minimum_required(VERSION 3.10)
cmake_policy(VERSION 3.10)

project(RayTracing)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug cache string "Choose the type of build (Debug or Release)" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

if(WIN32)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/)
endif(WIN32)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

find_package(COMMON REQUIRED)
message(STATUS "COMMON included at ${COMMON_INCLUDE_DIR}")

file(GLOB SOURCE "src/*")
set(NAME "RayTracing")
add_executable(${NAME} ${SOURCE})
target_link_libraries(${NAME} ${LIBS})

include_directories(${CMAKE_SOURCE_DIR}/includes/stb)
include_directories(${CMAKE_SOURCE_DIR}/includes)