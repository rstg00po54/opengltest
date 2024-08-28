cmake_minimum_required(VERSION 3.26.0)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
 
project(SoftMini3D)
# include_directories(SYSTEM ../include/)
include_directories(SYSTEM .)
include_directories(SYSTEM ./math/)
include_directories(SYSTEM ./draw/)
include_directories(SYSTEM ./render/)
include_directories(SYSTEM ./device/)
find_package(glfw3 REQUIRED)
find_package(SDL2_ttf REQUIRED)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/skin_debug.bmp ${CMAKE_CURRENT_BINARY_DIR}/../../bin/skin_debug.bmp COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/simsun.ttc ${CMAKE_CURRENT_BINARY_DIR}/../../bin/simsun.ttc COPYONLY)
 
add_executable(SoftMini3D 
    main.cpp 
    math/Mini3DMath.cpp 
    math/Mini3DTransform.cpp 
    math/Mini3DGeometryCalc.cpp 
    Mini3Dbmp.cpp 
    draw/Mini3DCalcLines.cpp
    device/Mini3DDevice.cpp
    render/Mini3DRender.cpp
    draw/Mini3DDrawCube.cpp
    draw/Mini3DDrawPanel.cpp
)
 
# target_link_libraries(SDLDemo glfw SDL2 glew32)
# target_link_libraries(IMGUIDemo PRIVATE opengl32 glfw SDL2 glew32)
target_link_libraries(SoftMini3D glfw IMGUI SDL2 glad opengl32 SDL2_ttf)