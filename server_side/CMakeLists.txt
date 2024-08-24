cmake_minimum_required(VERSION 3.25.1)
project (server_side)

include_directories(./../include ./../server_side/headers)

file(GLOB SOURCE ./../include/*.c ./src/*.c ./*.c)

add_executable(server_side ${SOURCE})
