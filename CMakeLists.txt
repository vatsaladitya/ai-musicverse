cmake_minimum_required(VERSION 3.0)

set(exec "AI MUSICVERSE")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(aimusicverse)

file(GLOB_RECURSE sources src/*.cpp include/*.h include/SDL2/*.h)
file(GLOB_RECURSE resources res/*)

add_executable(aimusicverse ${sources} ${resources})
target_compile_definitions(aimusicverse PUBLIC _WIN32)
target_include_directories(aimusicverse PUBLIC include)
target_link_directories(aimusicverse PUBLIC lib)
target_link_libraries(aimusicverse PUBLIC
    SDL2_image
    SDL2_ttf
    SDL2_mixer
    SDL2main
    SDL2
    user32
    gdi32
    kernel32
    winmm
)

set_target_properties(aimusicverse PROPERTIES OUTPUT_NAME ${exec})