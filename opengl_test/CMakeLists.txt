cmake_minimum_required(VERSION 3.15...3.31)

project(OpenglTest
                VERSION 1.0
                LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,-rpath lib/ -lGL -lGLU")

set(BIN_NAME main)

set(SOURCES main.cpp
        src/app.h
        src/extern/glad.c
        src/constants.hpp
        src/core/util.hpp
        src/core/texture.h
        src/core/vec2.hpp
        # src/shader.h
        # src/shader.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src/extern/glad)

add_executable(${BIN_NAME} ${SOURCES})

set(SDL2_LIBRARIES SDL2main SDL2 SDL2_image SDL2_mixer SDL2_ttf)
target_link_libraries(${BIN_NAME} PRIVATE GL GLU ${SDL2_LIBRARIES})
