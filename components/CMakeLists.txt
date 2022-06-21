# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

# Optionally glob, but only for CMake 3.12 or later:
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/components/include/*.h")

file(GLOB SRC_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/components/src/*.cpp")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenCV REQUIRED)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

# Make an automatic library - will be static or dynamic based on user setting
add_library(streamflow_component_lib SHARED ${SRC_LIST}  ${HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(streamflow_component_lib PUBLIC include)

# This depends on (header only) boost
target_link_libraries(streamflow_component_lib PRIVATE opencv::opencv pthread)

# All users of this library will need at least C++11
target_compile_features(streamflow_component_lib PUBLIC cxx_std_17)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/components/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
