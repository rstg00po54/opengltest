cmake_minimum_required(VERSION 3.26.0)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
 
project(LearnDemo2)

find_package(glfw3 REQUIRED)

# include_directories(SYSTEM ../../library/glad/include/glad)
 
file(GLOB project_file ../src/glad.c src/main.cpp ../src/stb_image.cpp src/testglm.cpp src/mglm.cpp)
add_executable(LearnDemo2 ${project_file})

message(STATUS "->${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "->${CMAKE_CURRENT_BINARY_DIR}")
# add_custom_command(
#   TARGET LearnDemo1 POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E copy skin_debug.jpg ${CMAKE_CURRENT_BINARY_DIR}/1.j
# #   COMMAND ${CMAKE_COMMAND} -E copy "${GENERATE_BIN_PATH}/lib1.a"   ${LIB_OUTPUT_PATH}
# )
# "VertexShader.vs", "FragmentShader.fs"
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/skin_debug.jpg ${CMAKE_CURRENT_BINARY_DIR}/../../bin/skin_debug.jpg COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/VertexShader.vs ${CMAKE_CURRENT_BINARY_DIR}/../../bin/VertexShader.vs COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/FragmentShader.fs ${CMAKE_CURRENT_BINARY_DIR}/../../bin/FragmentShader.fs COPYONLY)

target_link_libraries(LearnDemo2 glfw IMGUI)
# target_link_libraries(LearnDemo1 glfw)