cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)
project(StreamFlow CXX) 

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_definitions("-std=c++17")

file(GLOB_RECURSE all_SRCS
        "${PROJECT_SOURCE_DIR}/components/**.h"
        "${PROJECT_SOURCE_DIR}/components/**.cpp"
        "${PROJECT_SOURCE_DIR}/ComponentFactory.cpp"
        "${PROJECT_SOURCE_DIR}/include/**.h"
        )

message(status ${all_SRCS})


add_executable(${PROJECT_NAME} "main.cpp" ${all_SRCS} )
#SET_TARGET_PROPERTIES ( ${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-include ${SRC}")


message(CONAN_LIBS= "${CONAN_LIBS}")
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} pthread  ${CONAN_LIBS} opencv::opencv)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PUBLIC components)
target_include_directories(${PROJECT_NAME} PUBLIC components/webcam)



