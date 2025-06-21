cmake_minimum_required(VERSION 3.15...3.31)
project(Defblade VERSION 1.0
                  DESCRIPTION "Pixel art platformer"
                  LANGUAGES CXX)

set(CMAKE_CPP_COMPILER g++)
set(CMAKE_MAKE_PROGRAM ninja)

set(SOURCES main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(main ${SOURCES})

set(SDL2_LIBRARIES SDL2config SDL2main SDL2 SDL2_image SDL2_mixer SDL2_ttf)

target_link_libraries(main PUBLIC ${SDL2_LIBRARIES})