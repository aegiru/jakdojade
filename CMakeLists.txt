cmake_minimum_required(VERSION 3.0.0)
project(jakdojade VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(jakdojade main.cpp main.h)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(CPack)
