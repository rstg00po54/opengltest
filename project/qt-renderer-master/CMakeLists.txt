cmake_minimum_required(VERSION 3.1.0)

project(helloworld)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

message("cmake source dir = ${CMAKE_SOURCE_DIR}")

# set (Qt5_DIR "/mingw64/lib/cmake/Qt5")

include_directories(SYSTEM ./QtRenderer/)

# find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(qtrender
    QtRenderer/main.cpp
    QtRenderer/QtRenderer.cpp
    QtRenderer/Model.cpp
    QtRenderer/AddDialog.cpp
    QtRenderer/Rasterizer.cpp
    QtRenderer/Renderer.cpp
    QtRenderer/Scene.cpp
    QtRenderer/Texture.cpp
)

target_link_libraries(qtrender Assimp)