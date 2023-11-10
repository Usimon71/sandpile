cmake_minimum_required(VERSION 3.0.0)
project(SandPile VERSION 0.1.0 LANGUAGES C CXX)

add_executable(SandPile main.cpp
                src/SandBox.cpp
                src/bmp.cpp
                src/parser.cpp
                src/images.cpp)

target_compile_features(SandPile PRIVATE cxx_std_20)
target_include_directories(SandPile PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)