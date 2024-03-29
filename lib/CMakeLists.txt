project("libromfs-${LIBROMFS_PROJECT_NAME}")

set(CMAKE_CXX_STANDARD 20)

# Gather romfs files
set(ROMFS_FILES "")

foreach (path ${LIBROMFS_RESOURCE_LOCATION})
    file(GLOB_RECURSE ROMFS_FILES_PART
        "${path}/*"
    )
    list(APPEND ROMFS_FILES ${ROMFS_FILES_PART})
endforeach ()

# Add sources
add_library(${PROJECT_NAME} STATIC
    ${ROMFS}
    source/romfs.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC include)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
target_compile_definitions(${PROJECT_NAME} PUBLIC LIBROMFS_PROJECT_NAME=${LIBROMFS_PROJECT_NAME})

# Make sure libromfs gets rebuilt when any of the resources are changed
add_custom_command(OUTPUT ${ROMFS}
        COMMAND $<TARGET_FILE:generator-${LIBROMFS_PROJECT_NAME}>
        DEPENDS ../generator ${ROMFS_FILES}
        )

add_custom_target(romfs_file_packer-${LIBROMFS_PROJECT_NAME} ALL DEPENDS ${ROMFS_FILES})