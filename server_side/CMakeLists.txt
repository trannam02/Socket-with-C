cmake_minimum_required(VERSION 3.25.1)
project (server_side)

include_directories(./../include)

file(GLOB SOURCE ./../src/*.c ./*.c)

add_executable(server_side ${SOURCE})
