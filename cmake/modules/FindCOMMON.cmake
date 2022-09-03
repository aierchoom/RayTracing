set(_common_HEADER_SEARCH_DIRS
  ${CMAKE_SOURCE_DIR}/includes
)

find_path(COMMON_INCLUDE_DIR "common/" PATHS ${_common_HEADER_SEARCH_DIRS})

set(_common_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/includes/common")
add_library(common STATIC)
set(LIBS ${LIBS} common)

file(GLOB SOURCE
  "${_common_LIBRARY_DIR}/*.h"
  "${_common_LIBRARY_DIR}/*.cc"
)

target_sources(common PRIVATE
  "${SOURCE}"
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(COMMON DEFAULT_MSG ${common} COMMON_INCLUDE_DIR)