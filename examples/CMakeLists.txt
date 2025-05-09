cmake_minimum_required(VERSION 3.16)

project(examples LANGUAGES C)

add_executable(example-serial-cli "example-serial-cli.c")
target_link_libraries(example-serial-cli PUBLIC xcomm)

add_executable(example-serial-srv "example-serial-srv.c")
target_link_libraries(example-serial-srv PUBLIC xcomm)

add_executable(example-sync-tcp-cli "example-sync-tcp-cli.c")
target_link_libraries(example-sync-tcp-cli PUBLIC xcomm)

add_executable(example-sync-tcp-srv "example-sync-tcp-srv.c")
target_link_libraries(example-sync-tcp-srv PUBLIC xcomm)

add_executable(example-melsec-1c-cli "example-melsec-1c-cli.c")
target_link_libraries(example-melsec-1c-cli PUBLIC xcomm)

install(TARGETS example-serial-cli DESTINATION bin)
install(TARGETS example-serial-srv DESTINATION bin)
install(TARGETS example-sync-tcp-cli DESTINATION bin)
install(TARGETS example-sync-tcp-srv DESTINATION bin)
install(TARGETS example-melsec-1c-cli DESTINATION bin)

#install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/certs DESTINATION bin FILES_MATCHING PATTERN "*")
