cmake_minimum_required(VERSION 3.21)
project(ServidorHTTP)

set(CMAKE_CXX_STANDARD 17)

add_executable(ServidorHTTP main.cpp)

# Vincule a biblioteca WinSock
target_link_libraries(ServidorHTTP ws2_32)
