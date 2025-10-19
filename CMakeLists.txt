cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(ascii3d)

add_library(lib3d
    src/mesh.cpp
    src/linalg.cpp
    src/camera.cpp
    src/raster.cpp
    include/render.h
)

target_include_directories(lib3d PUBLIC ${PROJECT_SOURCE_DIR}/include)
add_executable(ascii3d 
    src/main.cpp
)
target_link_libraries(ascii3d PRIVATE lib3d)

# find_package(ImageMagick COMPONENTS Magick++)
# target_link_libraries(ascii3d PRIVATE ImageMagick::Magick++)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBSIXEL REQUIRED libsixel)
target_include_directories(ascii3d PRIVATE ${LIBSIXEL_INCLUDE_DIRS})
target_link_libraries(ascii3d PRIVATE ${LIBSIXEL_LIBRARIES})

add_compile_options(-O3)
