cmake_minimum_required(VERSION 3.25.1)
project (client_side)

include_directories(./../include)

file(GLOB SOURCE ./../include/*.c ./*.c)

add_executable(client_side ${SOURCE})
