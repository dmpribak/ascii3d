cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(ascii3d)

add_library(lib3d
    src/mesh.cpp
    src/linalg.cpp
    src/camera.cpp
    src/raster.cpp
)
target_include_directories(lib3d PUBLIC ${PROJECT_SOURCE_DIR}/include)
add_executable(ascii3d 
    src/main.cpp
)
target_link_libraries(ascii3d PRIVATE lib3d)
