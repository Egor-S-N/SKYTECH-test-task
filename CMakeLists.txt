cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(ClientServerApp VERSION 1.0.0)

add_executable(ServerPart server/server.cpp)
add_executable(ClientPart client/client.cpp)
link_libraries(ws2_32 wsock32)