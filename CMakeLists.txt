cmake_minimum_required(VERSION 2.8)

project(supershod-hd C)

if (ANDROID)
	SET(LIBSUPERDERPY_GAMENAME "supershodhd" CACHE INTERNAL "")
else(ANDROID)
	SET(LIBSUPERDERPY_GAMENAME "supershod-hd" CACHE INTERNAL "")
endif(ANDROID)
SET(LIBSUPERDERPY_GAMENAME_PRETTY "SUPERSHOD HD" CACHE INTERNAL "")
SET(LIBSUPERDERPY_ORIENTATION "LANDSCAPE" CACHE INTERNAL "")
SET(LIBSUPERDERPY_VERSION "1.1" CACHE INTERNAL "")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" "${CMAKE_SOURCE_DIR}/libsuperderpy/cmake")

include(libsuperderpy)
include(SetPaths)

set(EMSCRIPTEN_TOTAL_MEMORY "256")

add_subdirectory(libsuperderpy)
add_subdirectory(src)
add_subdirectory(data)

# uninstall target
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/libsuperderpy/cmake/cmake_uninstall.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
