project(generator)
set(CMAKE_CXX_STANDARD 20)

add_executable(generator
    source/main.cpp
)

target_compile_definitions(generator PRIVATE RESOURCE_LOCATION="${LIBROMFS_RESOURCE_LOCATION}")
target_include_directories(generator PRIVATE include)

set(ROMFS "libromfs_resources.cpp" PARENT_SCOPE)
set(ROMFS "libromfs_resources.cpp")