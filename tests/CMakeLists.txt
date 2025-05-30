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

add_executable(test-rwlock "test-rwlock.c")
target_link_libraries(test-rwlock PUBLIC xcomm)
add_test(NAME rwlock COMMAND test-rwlock)

add_executable(test-sha1 "test-sha1.c")
target_link_libraries(test-sha1 PUBLIC xcomm)
add_test(NAME sha1 COMMAND test-sha1)

add_executable(test-sha256 "test-sha256.c")
target_link_libraries(test-sha256 PUBLIC xcomm)
add_test(NAME sha256 COMMAND test-sha256)

add_executable(test-thrdpool "test-thrdpool.c")
target_link_libraries(test-thrdpool PUBLIC xcomm)
add_test(NAME thrdpool COMMAND test-thrdpool)

add_executable(test-timers "test-timers.c")
target_link_libraries(test-timers PUBLIC xcomm)
add_test(NAME timers COMMAND test-timers)

add_executable(test-varint "test-varint.c")
target_link_libraries(test-varint PUBLIC xcomm)
add_test(NAME varint COMMAND test-varint)

add_executable(test-wg "test-wg.c")
target_link_libraries(test-wg PUBLIC xcomm)
add_test(NAME wg COMMAND test-wg)