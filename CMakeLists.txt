cmake_minimum_required(VERSION 3.22)
set(CMAKE_C_STANDARD 23)
project(SocketUDP C)

add_library(networking networking.h networking.c)

add_executable(server server.c)
add_library(client client.c client.h)

target_link_libraries(server networking client)
target_link_libraries(client networking)