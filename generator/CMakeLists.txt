project(generator-${LIBROMFS_PROJECT_NAME})

# This is a code generator, so when cross compiling it should be built with the host (native) toolchain
if (CMAKE_CROSSCOMPILING)
    SET(CMAKE_C_COMPILER ${NATIVE_CMAKE_C_COMPILER})
    SET(CMAKE_CXX_COMPILER ${NATIVE_CMAKE_CXX_COMPILER})
    unset(CMAKE_C_OSX_DEPLOYMENT_TARGET_FLAG)
    unset(CMAKE_CXX_OSX_DEPLOYMENT_TARGET_FLAG)
    SET(CMAKE_C_FLAGS ${NATIVE_CMAKE_C_FLAGS})
    SET(CMAKE_CXX_FLAGS ${NATIVE_CMAKE_CXX_FLAGS})
endif()


# Add sources
add_executable(${PROJECT_NAME}
    source/main.cpp
)

string(REPLACE ";" "," LIBROMFS_RESOURCE_LOCATION_STRING "${LIBROMFS_RESOURCE_LOCATION}")

target_compile_definitions(${PROJECT_NAME} PRIVATE RESOURCE_LOCATION="${LIBROMFS_RESOURCE_LOCATION_STRING}" LIBROMFS_PROJECT_NAME="${LIBROMFS_PROJECT_NAME}")
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

if (LIBROMFS_COMPRESS_RESOURCES)
    find_package(ZLIB QUIET)
    if (ZLIB_FOUND)
        list(LENGTH ZLIB_LIBRARIES ZLIB_LIBRARIES_COUNT)

        if (${ZLIB_LIBRARIES_COUNT} EQUAL 1)
            if (TARGET ${ZLIB_LIBRARIES})
                target_link_libraries(${PROJECT_NAME} PRIVATE ${ZLIB_LIBRARIES})
                target_compile_definitions(${PROJECT_NAME} PRIVATE LIBROMFS_COMPRESS_RESOURCES=1)
            endif()
        elseif(${ZLIB_LIBRARIES_COUNT} GREATER 1)
            target_link_libraries(${PROJECT_NAME} PRIVATE ${ZLIB_LIBRARIES})
            target_include_directories(${PROJECT_NAME} PRIVATE ${ZLIB_INCLUDE_DIRS})
            target_compile_definitions(${PROJECT_NAME} PRIVATE LIBROMFS_COMPRESS_RESOURCES=1)
        endif()
    else()
        message(WARNING "Requested RomFS to be compressed but zlib is unavailable! Resulting RomFS will NOT be compressed.")
    endif()
endif()

if (LIBROMFS_USE_EMBED)
    set(LIBROMFS_COMPRESSED_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/compressed/${LIBROMFS_PROJECT_NAME}")
    file(MAKE_DIRECTORY ${LIBROMFS_COMPRESSED_OUTPUT_DIR})
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        LIBROMFS_USE_EMBED
        COMPRESSED_OUTPUT_DIR="${LIBROMFS_COMPRESSED_OUTPUT_DIR}"
    )
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE "/EHsc")
endif()

# Export generated romfs resource file to libromfs
set(ROMFS "libromfs_resources.cpp" PARENT_SCOPE)
set(ROMFS "libromfs_resources.cpp")