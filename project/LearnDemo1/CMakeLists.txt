cmake_minimum_required(VERSION 3.26.0)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
 
project(LearnDemo1)
include_directories(SYSTEM ../../library/imgui)
include_directories(SYSTEM ../../library/glad/include/glad)
include_directories(SYSTEM ../../library/stb_image)

find_package(glfw3 REQUIRED)
 
file(GLOB project_file ../src/glad.c src/main.cpp ../src/stb_image.cpp src/testglm.cpp src/mglm.cpp)
add_executable(LearnDemo1 ${project_file})

message(STATUS "->${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "->${CMAKE_CURRENT_BINARY_DIR}")
# add_custom_command(
#   TARGET LearnDemo1 POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E copy skin_debug.jpg ${CMAKE_CURRENT_BINARY_DIR}/1.j
# #   COMMAND ${CMAKE_COMMAND} -E copy "${GENERATE_BIN_PATH}/lib1.a"   ${LIB_OUTPUT_PATH}
# )

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/skin_debug.jpg ${CMAKE_CURRENT_BINARY_DIR}/../../bin/skin_debug.jpg COPYONLY)

target_link_libraries(LearnDemo1 glfw IMGUI)
# target_link_libraries(LearnDemo1 glfw)