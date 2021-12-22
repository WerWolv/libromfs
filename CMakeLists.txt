cmake_minimum_required(VERSION 3.16)

project(libromfs_base)

if (NOT DEFINED LIBROMFS_RESOURCE_LOCATION)
    message(FATAL_ERROR "To use libromfs, please point the variable LIBROMFS_RESOURCE_LOCATION to a valid folder path")
elseif (NOT EXISTS ${LIBROMFS_RESOURCE_LOCATION} OR NOT IS_DIRECTORY ${LIBROMFS_RESOURCE_LOCATION})
    message(FATAL_ERROR "LIBROMFS_RESOURCE_LOCATION has been defined but does not point to a valid folder")
endif ()

add_subdirectory(generator)
add_subdirectory(lib)