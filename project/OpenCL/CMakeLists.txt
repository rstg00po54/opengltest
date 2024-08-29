cmake_minimum_required(VERSION 3.10)
project(OpenCLTest)

find_package(OpenCL REQUIRED)


configure_file(${CMAKE_CURRENT_SOURCE_DIR}/vector_add_kernel.cl ${CMAKE_CURRENT_BINARY_DIR}/../../bin/vector_add_kernel.cl COPYONLY)


add_executable(test_opencl main.cpp)
target_include_directories(test_opencl PRIVATE ${OpenCL_INCLUDE_DIRS})
target_link_libraries(test_opencl PRIVATE ${OpenCL_LIBRARIES})
