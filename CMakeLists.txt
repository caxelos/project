cmake_minimum_required(VERSION 3.10)

project  (evaluation)

set ( CMAKE_C_FLAGS "-Wall -g")

include_directories(include)

set(SERVER_SOURCES src/server.c src/serverMW.c )
set(CLIENT_SOURCES src/client.c src/clientMW.c)

add_executable(server ${SERVER_SOURCES} )
add_executable(client ${CLIENT_SOURCES})

target_link_libraries(server -lpthread)
target_link_libraries(client -lpthread)