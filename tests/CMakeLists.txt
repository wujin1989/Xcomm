cmake_minimum_required(VERSION 3.16)

project(tests LANGUAGES C)

add_executable(test-byteorder-swap "test-byteorder-swap.c")
target_link_libraries(test-byteorder-swap PUBLIC xcomm)

add_executable(test-dlist "test-dlist.c")
target_link_libraries(test-dlist PUBLIC xcomm)

add_test(NAME byteorder-swap COMMAND test-byteorder-swap)
add_test(NAME dlist COMMAND test-dlist)

