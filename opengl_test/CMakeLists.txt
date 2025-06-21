cmake_minimum_required(VERSION 3.15...3.31)

project(OpenglTest VERSION 1.0
                             LANGUAGES CXX)

set(CMAKE_CPP_COMPILER g++)

set(BIN_NAME main)

set(
        SOURCES main.cpp
        src/extern/glad.c
        src/constants.hpp src/core/util.hpp
        src/app.h
        src/core/texture.h
        src/core/vec2.hpp
        # src/shader.h
        # src/shader.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(${BIN_NAME} ${SOURCES})

set(SDL2_LIBRARIES SDL2main SDL2 SDL2_image SDL2_mixer SDL2_ttf)
target_link_libraries(${BIN_NAME} GL GLU ${SDL2_LIBRARIES})
