cmake_minimum_required(VERSION 3.10)
cmake_policy(VERSION 3.10)

project(RayTracing)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug cache string "Choose the type of build (Debug or Release)" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

if(WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif(WIN32)


set(CMAKE_CXX_STANDARD 11)

set(CHAPTERS
  chapter1
)

set(chapter1
  1.ppm
)

foreach(CHAPTER ${CHAPTERS})
  foreach(DEMO ${${CHAPTER}})
    file(GLOB SOURCE
      "src/${CHAPTER}/${DEMO}/*.h"
      "src/${CHAPTER}/${DEMO}/*.cc"
    )
    set(NAME "${CHAPTER}_${DEMO}")
    add_executable(${NAME} ${SOURCE})
  endforeach(DEMO)
endforeach(CHAPTER)

include_directories(${CMAKE_SOURCE_DIR}/includes/)
include_directories(${CMAKE_SOURCE_DIR}/includes/stb)
