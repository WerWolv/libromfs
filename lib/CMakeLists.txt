project(libromfs)

set(CMAKE_CXX_STANDARD 20)

# Gather romfs files
file(GLOB_RECURSE ROMFS_FILES
    "${LIBROMFS_RESOURCE_LOCATION}/*"
)

# Add sources
add_library(libromfs STATIC
    ${ROMFS}
    source/romfs.cpp
)
target_include_directories(libromfs PUBLIC include)
set_target_properties(libromfs PROPERTIES PREFIX "")


# Make sure libromfs gets rebuilt when any of the resources are changed
add_custom_command(OUTPUT ${ROMFS}
        COMMAND $<TARGET_FILE:generator>
        DEPENDS ../generator ${ROMFS_FILES}
        )

add_custom_target(romfs_file_packer ALL DEPENDS ${ROMFS_FILES})