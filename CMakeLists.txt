cmake_minimum_required(VERSION 3.16)

project(Xcomm LANGUAGES C)

option(BUILD_DYNAMIC_LIBRARY "Build Dynamic Library" OFF)
option(CMAKE_EXPORT_COMPILE_COMMANDS "Export Compile-Commands" OFF)

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install")

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
	src/xcomm-base64.c
	src/xcomm-heap.c
	src/xcomm-list.c
	src/xcomm-logger.c
	src/xcomm-queue.c
	src/xcomm-rbtree.c
	src/xcomm-sha1.c
	src/xcomm-sha256.c
	src/xcomm-utils.c
	src/xcomm-varint.c
	src/xcomm-stack.c
	src/xcomm-timer.c
	src/xcomm-rwlock.c
	src/xcomm-spinlock.c
	src/xcomm-wg.c
	src/xcomm-thrdpool.c
	src/xcomm-ringbuffer.c
	src/xcomm-bswap.c

	src/modules/dumper/xcomm-dumper.c
	src/modules/dumper/xcomm-dumper-module.c

	src/modules/serial/xcomm-serial.c
	src/modules/serial/xcomm-serial-module.c
	
	src/modules/tcp/xcomm-sync-tcp.c
	src/modules/tcp/xcomm-tcp-module.c

	src/modules/melsec/xcomm-melsec-1c.c
	src/modules/melsec/xcomm-melsec-1e.c
	src/modules/melsec/xcomm-melsec-3c.c
	src/modules/melsec/xcomm-melsec-3e.c
	src/modules/melsec/xcomm-melsec-common.c
	src/modules/melsec/xcomm-melsec-module.c
)

if(WIN32)
	list(APPEND SRCS 
		src/platform/win/platform-info.c
		src/platform/win/platform-thrd.c
		src/platform/win/platform-process.c
		src/platform/win/platform-socket.c
		src/platform/win/platform-event.c
		src/platform/win/wepoll/wepoll.c
		src/platform/win/platform-time.c
		src/platform/win/platform-loader.c
		src/platform/win/platform-io.c
		src/platform/win/platform-uart.c
	)
endif()

if(UNIX)
	list(APPEND SRCS 
		src/platform/unix/platform-info.c
		src/platform/unix/platform-thrd.c
		src/platform/unix/platform-process.c
		src/platform/unix/platform-socket.c
		src/platform/unix/platform-event.c
		src/platform/unix/platform-time.c
		src/platform/unix/platform-loader.c
		src/platform/unix/platform-io.c
		src/platform/unix/platform-uart.c
	)
endif()

if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
	set(CMAKE_SYSTEM_PROCESSOR "AMD64")
endif()

if (CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
	set(CMAKE_SYSTEM_PROCESSOR "aarch64")
endif()

add_library(ssl IMPORTED STATIC)
add_library(crypto IMPORTED STATIC)

if(WIN32)
	set_target_properties(ssl PROPERTIES
		IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/third-party/openssl3/lib/${CMAKE_SYSTEM_PROCESSOR}/win/ssl.lib"
		INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/third-party/openssl3//include")

	set_target_properties(crypto PROPERTIES
		IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/third-party/openssl3/lib/${CMAKE_SYSTEM_PROCESSOR}/win/crypto.lib"
		INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/third-party/openssl3//include" 
		INTERFACE_LINK_LIBRARIES "Crypt32.lib")
endif()

if(UNIX)
	set_target_properties(ssl PROPERTIES
		IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/third-party/openssl3/lib/${CMAKE_SYSTEM_PROCESSOR}/unix/libssl.a"
		INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/third-party/openssl3//include")

	set_target_properties(crypto PROPERTIES
		IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/third-party/openssl3/lib/${CMAKE_SYSTEM_PROCESSOR}/unix/libcrypto.a"
		INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/third-party/openssl3//include")
endif()

if(BUILD_DYNAMIC_LIBRARY)
    add_library(xcomm SHARED ${SRCS})
	target_link_libraries(xcomm PRIVATE ssl crypto ${CMAKE_DL_LIBS})
	set_target_properties(xcomm PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS 1)
else()
    add_library(xcomm STATIC ${SRCS})
	target_link_libraries(xcomm PRIVATE ssl crypto ${CMAKE_DL_LIBS})
endif()

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include DESTINATION . FILES_MATCHING PATTERN "*")
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs DESTINATION . FILES_MATCHING PATTERN "*")
install(TARGETS xcomm DESTINATION lib)

add_subdirectory(examples)

