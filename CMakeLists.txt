cmake_minimum_required(VERSION 3.20)
project(dinoKiller)

set(CMAKE_CXX_STANDARD 23)

add_executable(dinoKiller main.cpp)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(dinoKiller sfml-system sfml-window sfml-graphics sfml-audio sfml-network)