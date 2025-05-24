cmake_minimum_required(VERSION 3.16)

project(tests LANGUAGES C)

add_executable(test-bswap "test-bswap.c")
target_link_libraries(test-bswap PUBLIC xcomm)
add_test(NAME bswap COMMAND test-bswap)

add_executable(test-list "test-list.c")
target_link_libraries(test-list PUBLIC xcomm)
add_test(NAME list COMMAND test-list)

add_executable(test-base64 "test-base64.c")
target_link_libraries(test-base64 PUBLIC xcomm)
add_test(NAME base64 COMMAND test-base64)

add_executable(test-heap "test-heap.c")
target_link_libraries(test-heap PUBLIC xcomm)
add_test(NAME heap COMMAND test-heap)

add_executable(test-logger "test-logger.c")
target_link_libraries(test-logger PUBLIC xcomm)
add_test(NAME logger COMMAND test-logger)

add_executable(test-rbtree "test-rbtree.c")
target_link_libraries(test-rbtree PUBLIC xcomm)
add_test(NAME rbtree COMMAND test-rbtree)

add_executable(test-ringbuffer "test-ringbuffer.c")
target_link_libraries(test-ringbuffer PUBLIC xcomm)
add_test(NAME ringbuffer COMMAND test-ringbuffer)