cmake_minimum_required(VERSION 3.10)

project  (evaluation)

set ( CMAKE_C_FLAGS "-Wall -g")

set(HEADER_FILES include/serverMW.h)

include_directories(include)

file(GLOB SOURCES "src/*.c")

add_executable(server ${SOURCES} ${HEADER_FILES} )
add_executable(client src/client.c)

target_link_libraries(server -lpthread)
target_link_libraries(client -lpthread)