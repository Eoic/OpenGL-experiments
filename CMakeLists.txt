cmake_minimum_required(VERSION 3.27)
project(OpenGL)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} vendor/glad/include)
set(CMAKE_CXX_STANDARD 17)

add_executable(OpenGL main.cpp vendor/glad/src/glad.c)
target_link_libraries(OpenGL ${SDL2_LIBRARIES})
