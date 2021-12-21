project(libromfs_base)
cmake_minimum_required(VERSION 3.21)

if (NOT DEFINED LIBROMFS_RESOURCE_LOCATION)
    message(FATAL_ERROR "To use libromfs, please point the variable LIBROMFS_RESOURCE_LOCATION to a valid folder path")
endif ()

enable_testing()
add_subdirectory(test EXCLUDE_FROM_ALL)

add_subdirectory(generator)
add_subdirectory(lib)