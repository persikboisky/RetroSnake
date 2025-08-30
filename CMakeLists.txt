set(PROJECT_NAME snake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/x64)

project(${PROJECT_NAME})

cmake_minimum_required(VERSION 3.5)

add_executable(${PROJECT_NAME} ${CMAKE_HOME_DIRECTORY}/src/main.cpp)

add_subdirectory(${CMAKE_HOME_DIRECTORY}/lib/SFML)
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window)
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_HOME_DIRECTORY}/lib/SFML/include)

set(SFML_BUILD_AUDIO OFF)
set(SFML_BUILD_NETWORK OFF)