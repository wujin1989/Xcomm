#/bin/sh

OHOS_NDK_HOME="your ndk path"
export PATH=${OHOS_NDK_HOME}/build-tools/cmake/bin:$PATH

cmake.exe 																		\
	-G Ninja 																	\
	-B out 																	    \
	-DCMAKE_TOOLCHAIN_FILE="${OHOS_NDK_HOME}/build/cmake/ohos.toolchain.cmake" 	\
	-DOHOS_ARCH=arm64-v8a														\
	
cmake.exe --build out --config Release -j 8
cmake.exe --install out 