cmake_minimum_required(VERSION 3.22)
project(chip8-emulator C)

set(CMAKE_C_STANDARD 99)
set (
        SRC
        main.c
        chip8.h chip8.c renderer.c renderer.h input.c input.h)

add_executable(chip8-emulator ${SRC})
target_link_libraries(${PROJECT_NAME} SDL2)
