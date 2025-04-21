cmake_minimum_required(VERSION 3.16)

project(xlink LANGUAGES C)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_C_STANDARD 11)

if(WIN32)
	if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
		add_compile_options("/experimental:c11atomics")
	endif()
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SRCS
	src/xlink.c
	src/xlink-logger.c

	src/melsec/melsec-1c.c
	src/melsec/melsec-3c.c
	src/melsec/melsec-3e.c
)

if(WIN32)
	list(APPEND SRCS 
		src/platform/win/platform-utils.c
		src/platform/win/platform-socket.c
		src/platform/win/platform-event.c
		src/platform/win/platform-time.c
		src/platform/win/platform-io.c
	)
endif()

if(UNIX)
	list(APPEND SRCS 
		src/platform/unix/platform-utils.c
		src/platform/unix/platform-socket.c
		src/platform/unix/platform-event.c
		src/platform/unix/platform-time.c
		src/platform/unix/platform-io.c
	)
endif()

add_library(xlink STATIC ${SRCS})
target_link_libraries(xlink PRIVATE ${CMAKE_DL_LIBS})

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include DESTINATION . FILES_MATCHING PATTERN "*")
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs DESTINATION . FILES_MATCHING PATTERN "*")
install(TARGETS xlink DESTINATION lib)

add_subdirectory(examples)

