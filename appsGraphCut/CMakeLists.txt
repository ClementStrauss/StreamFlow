find_package(OpenCV REQUIRED)

add_executable(autoGraphCut "main.cpp")

target_compile_features(autoGraphCut PRIVATE cxx_std_20)

target_link_libraries(autoGraphCut PUBLIC ${OpenCV_LIBS} streamflow_component_lib gvc cgraph cdt)
