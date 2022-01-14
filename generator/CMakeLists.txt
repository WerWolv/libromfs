project(generator)
set(CMAKE_CXX_STANDARD 20)

# Add sources
add_executable(generator
    source/main.cpp
)
target_compile_definitions(generator PRIVATE RESOURCE_LOCATION="${LIBROMFS_RESOURCE_LOCATION}")
target_include_directories(generator PRIVATE include)

# Export generated romfs resource file to libromfs
set(ROMFS "libromfs_resources.cpp" PARENT_SCOPE)
set(ROMFS "libromfs_resources.cpp")