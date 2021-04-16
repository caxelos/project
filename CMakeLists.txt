
project  (evaluation)
set ( CMAKE_C_FLAGS "-Wall -g")
add_executable(server src/server.c)
add_executable(client src/client.c)
target_link_libraries(server -lpthread)
target_link_libraries(client -lpthread)