
cmake_minimum_required(VERSION 3.15)

# Project name and a few useful settings. Other commands can pick up the results
project(
  StreamFlow
  VERSION 0.1
  DESCRIPTION "An example project with CMake"
  LANGUAGES CXX)


  find_package(OpenCV REQUIRED)
  include(${CMAKE_BINARY_DIR}/conan_paths.cmake)


set(CMAKE_CXX_STANDARD 17)

# Let's ensure -std=c++xx instead of -std=g++xx
set(CMAKE_CXX_EXTENSIONS OFF)

# Let's nicely support folders in IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


# The compiled library code is here
add_subdirectory(components)

# The executable code is here
add_subdirectory(apps)








#add_definitions("-std=c++17")


# file(GLOB_RECURSE all_SRCS
#     "${PROJECT_SOURCE_DIR}/components/**.cpp"
#     "${PROJECT_SOURCE_DIR}/ComponentFactory.cpp"
# )

# file(GLOB_RECURSE all_HEADERS
#     "${PROJECT_SOURCE_DIR}/components/**.h"
#     "${PROJECT_SOURCE_DIR}/include/**.h"
# )


# message(status ${all_SRCS})

# add_executable(${PROJECT_NAME} "main.cpp" ${all_SRCS})
# #SET_TARGET_PROPERTIES ( ${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-include ${SRC}")


# target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# target_include_directories(${PROJECT_NAME} PUBLIC include)
# target_include_directories(${PROJECT_NAME} PUBLIC components)
# target_include_directories(${PROJECT_NAME} PUBLIC components/webcam)

# target_link_libraries( ${PROJECT_NAME} ${CONAN_LIBS} pthread opencv::opencv opencv::highgui)





