cmake_minimum_required(VERSION 3.16)

project(examples LANGUAGES C)

add_executable(example-serial-cli "example-serial-cli.c")
target_link_libraries(example-serial-cli PUBLIC xcomm)

add_executable(example-serial-srv "example-serial-srv.c")
target_link_libraries(example-serial-srv PUBLIC xcomm)

install(TARGETS example-serial-cli DESTINATION bin)
install(TARGETS example-serial-srv DESTINATION bin)

#install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/certs DESTINATION bin FILES_MATCHING PATTERN "*")
