cmake_minimum_required(VERSION 3.10)
project(chip8 C)

find_package(SDL2 REQUIRED)

add_executable(chip8 src/main.c)

target_include_directories(chip8 PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(chip8 PRIVATE ${SDL2_LIBRARIES})