cmake_minimum_required(VERSION 3.10)
# set(CMAKE_BUILD_TYPE Release)
# set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_CXX_FLAGS_DEBUG "-fno-omit-frame-pointer")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(termesh)

add_subdirectory(eigen)

add_library(lib3d
    src/mesh.cpp
    src/linalg.cpp
    src/camera.cpp
    src/raster.cpp
    include/render.h
)

target_include_directories(lib3d PUBLIC ${PROJECT_SOURCE_DIR}/include)
add_executable(termesh 
    src/main.cpp
)
target_link_libraries(lib3d PUBLIC eigen)
target_link_libraries(termesh PRIVATE lib3d)

# find_package(ImageMagick COMPONENTS Magick++)
# target_link_libraries(termesh PRIVATE ImageMagick::Magick++)

find_package(PkgConfig REQUIRED)

pkg_check_modules(NOTCURSES REQUIRED notcurses)
target_include_directories(termesh PRIVATE ${NOTCURSES_INCLUDE_DIRS})
target_link_libraries(termesh PRIVATE ${NOTCURSES_LIBRARIES})

target_compile_options(termesh PRIVATE
    -O3
    -march=native
    -ffast-math
    -fno-trapping-math
    -funroll-loops
)

# add_compile_options(-O2)
# add_link_options(-O2)
# add_compile_options(-fno-omit-frame-pointer -g)
# add_link_options(-fno-omit-frame-pointer -g)
set_property(TARGET termesh PROPERTY CXX_STANDARD 23)
