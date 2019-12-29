cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 17)

project(StreamFlow)
file(GLOB SRC "components/*.h")

message(status ${SRC})

add_executable(${PROJECT_NAME} "main.cpp")
#SET_TARGET_PROPERTIES ( ${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-include ${SRC}")


TARGET_LINK_LIBRARIES( ${PROJECT_NAME} pthread)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PUBLIC components)




