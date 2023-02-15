find_package(OpenCV REQUIRED)

add_executable(${PROJECT_NAME} "main.cpp")

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenCV_LIBS} streamflow_component_lib gvc cgraph cdt)
