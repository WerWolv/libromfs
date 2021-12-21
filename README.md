# libromfs

libromfs is an easy way to bundle resources directly into any C++ application and access them through a simple interface. 
The main advantage is, that you don't need to depend on the user having the application's resource folder in the right location and having it accessible by the application.

## Usage

libromfs depends on CMake for code generation.

```cmake
# Set the location of our romfs folder
set(LIBROMFS_RESOURCE_LOCATION "${CMAKE_SOURCE_DIR}/romfs")

# Include libromfs
add_subdirectory(libromfs)

# Link libromfs into our application
target_link_libraries(my_application PUBLIC libromfs)
```

Now, you can place all your files and folders into the `./romfs` folder next to your `CMakeLists.txt` file. 

To access the files now, you can simply query libromfs.

```cpp
 /* Include libromfs header */
#include <romfs/romfs.hpp>

#include <cstdio>

int main() {
  /* Get file relative to the romfs folder */
  romfs::Resource my_file = romfs::get("path/to/my/file.txt"); 
  
  /* Print size and content of file */
  std::printf("File Size: %zu bytes\n", my_file.content.size());
  std::printf("File content: %s\n", my_file.content.data());
}
```
