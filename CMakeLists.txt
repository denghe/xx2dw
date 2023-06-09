cmake_minimum_required(VERSION 3.20)

set(name xx2dw)
project(${name})

set(CMAKE_CXX_STANDARD 20)


include_directories(
	stb
	pugixml/src
	libvpx_prebuilt/gnusrc
	libvpx_prebuilt/gnusrc/vpx
	spine-runtimes/spine-cpp/spine-cpp/include
	3rd
	xxlib
	src
)

set(SRCS)
file(GLOB_RECURSE SRCS
	stb/*.h
	pugixml/src/*.hpp
	libvpx_prebuilt/gnusrc/*.c
	spine-runtimes/spine-cpp/spine-cpp/src/spine/*.cpp
	3rd/*.*
	xxlib/*.h
	src/*.c
	src/*.cpp
	src/*.h
	src/*.hpp
	src/*.inc
)
add_library(${name} ${SRCS})

source_group("" FILES ${SRCS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})


add_subdirectory(pugixml)


SET(BUILD_TESTING OFF CACHE BOOL "BUILD_TESTING" FORCE)
SET(USE_MSVC_RUNTIME_LIBRARY_DLL OFF CACHE BOOL "USE_MSVC_RUNTIME_LIBRARY_DLL" FORCE)
SET(ZSTD_BUILD_PROGRAMS OFF CACHE BOOL "ZSTD_BUILD_PROGRAMS" FORCE)
SET(ZSTD_BUILD_SHARED OFF CACHE BOOL "ZSTD_BUILD_SHARED" FORCE)
SET(ZSTD_BUILD_TESTS OFF CACHE BOOL "ZSTD_BUILD_SHARED" FORCE)
SET(ZSTD_LEGACY_SUPPORT OFF CACHE BOOL "ZSTD_LEGACY_SUPPORT" FORCE)
SET(ZSTD_MULTITHREAD_SUPPORT OFF CACHE BOOL "ZSTD_MULTITHREAD_SUPPORT" FORCE)
add_subdirectory(zstd/build/cmake zstd)
include_directories(
	zstd/lib
)


if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

macro(append_linker_flags FLAGS)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${FLAGS}")
endmacro()

if (EMSCRIPTEN)
	set(CMAKE_EXECUTABLE_SUFFIX ".html")

	# Link in the JS library file for support code
	#append_linker_flags("--js-library ${CMAKE_CURRENT_LIST_DIR}/library_js.js")

	# Link in to WebGL/GLES system library
	append_linker_flags("-lGL")
	
	append_linker_flags("-lglfw")
	append_linker_flags("-sUSE_GLFW=3")
	append_linker_flags("-sTOTAL_MEMORY=134217728")
	append_linker_flags("-sALLOW_MEMORY_GROWTH=1")
	append_linker_flags("-sASSERTIONS=1")
	append_linker_flags("-sNO_DISABLE_EXCEPTION_CATCHING=1")
	#append_linker_flags("-pthread -sUSE_PTHREADS -sPROXY_TO_PTHREAD")
	
	#append_linker_flags("-sERROR_ON_UNDEFINED_SYMBOLS=0")

	# When marshalling C UTF-8 strings across the JS<->Wasm language boundary, favor smallest generated code size
	# rather than performance
	append_linker_flags("-sTEXTDECODER=2")

	# Enable aggressive MINIMAL_RUNTIME mode.
	#append_linker_flags("-sMINIMAL_RUNTIME=2")

	# Require WebGL 2 support in target browser, for smallest generated code size. (pass -sMIN_WEBGL_VERSION=1 to dual-target WebGL 1 and WebGL 2)
	append_linker_flags("-sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 -sFULL_ES3")

	# Tell the example code in webgl.c that we are only targeting WebGL 2.
	add_definitions(-DMAX_WEBGL_VERSION=2)

	# The generated build output is only to be expected to be run in a web browser, never in a native CLI shell, or in a web worker.
	append_linker_flags("-sENVIRONMENT=web")

	# Choose the oldest browser versions that should be supported. The higher minimum bar you choose, the less
	# emulation code may be present for old browser quirks.
	append_linker_flags("-sMIN_FIREFOX_VERSION=79")
	append_linker_flags("-sMIN_SAFARI_VERSION=150000")
	append_linker_flags("-sMIN_IE_VERSION=0x7FFFFFFF") # Do not support Internet Explorer at all (this is the Emscripten default, shown here for posterity)
	append_linker_flags("-sMIN_EDGE_VERSION=79") # Require Chromium-based Edge browser
	append_linker_flags("-sMIN_CHROME_VERSION=80")

	# Fine tuning for code size: do not generate code to abort program execution on malloc() failures, that will
	# not be interesting here.
	append_linker_flags("-sABORTING_MALLOC=0")

	# Reduce WebGL code size: We do not need GLES2 emulation for automatic GL extension enabling
	append_linker_flags("-sGL_SUPPORT_AUTOMATIC_ENABLE_EXTENSIONS=1")

	# Reduce WebGL code size: We do not need GLES2 emulation for GL extension names
	#append_linker_flags("-sGL_EXTENSIONS_IN_PREFIXED_FORMAT=0")

	# Reduce WebGL code size: No need to specify the GL_VENDOR/GL_RENDERER etc. fields in format required by GLES2 spec.
	#append_linker_flags("-sGL_EMULATE_GLES_VERSION_STRING_FORMAT=0")

	# Reduce WebGL code size at the expense of performance (this only has an effect in WebGL 1, practically a no-op here)
	append_linker_flags("-sGL_POOL_TEMP_BUFFERS=0")

	# Reduce WebGL code size: WebGL bindings layer should not keep track of certain WebGL
	# errors that are only meaningful for C/C++ applications. (good to enable for release when glGetError() is not used, but disable in debug)
	append_linker_flags("-sGL_TRACK_ERRORS=0")

	# Reduce WebGL code size: do not emit code for extensions that we might not need.
	#append_linker_flags("-sGL_SUPPORT_SIMPLE_ENABLE_EXTENSIONS=0")

	# Optimization flag to optimize aggressively for size. (other options -Os, -O3, -O2, -O1, -O0)
	#append_linker_flags("-O0")
	#append_linker_flags("-g")
	append_linker_flags("-Oz")
	# Enable Closure compiler for aggressive JS size minification
	append_linker_flags("--closure 1")

	# Reduce code size: We do not need libc errno field support in our build output.
	append_linker_flags("-sSUPPORT_ERRNO=0")

	# Reduce code size: We do not need native POSIX filesystem emulation support (Emscripten FS/MEMFS)
	#append_linker_flags("-sFILESYSTEM=0")
endif()



########################################################################
########################################################################

set(name test1)
include_directories(
	${name}
)
set(SRCS)
file(GLOB_RECURSE SRCS
	${name}/*.c
	${name}/*.cpp
	${name}/*.h
	${name}/*.hpp
	${name}/*.inc
)
add_executable(${name} ${SRCS})
target_link_libraries(${name} xx2dw)


file(GLOB_RECURSE assets ${name}/res/*.*)
foreach(asset ${assets})
	file(COPY "${asset}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")
endforeach()

if (EMSCRIPTEN)
	set_target_properties(${name} PROPERTIES LINK_FLAGS "-sFILESYSTEM=0 --js-library ${CMAKE_CURRENT_LIST_DIR}/${name}/library_js.js")
endif()


########################################################################
########################################################################

set(name test2)
include_directories(
	${name}
)
set(SRCS)
file(GLOB_RECURSE SRCS
	${name}/*.c
	${name}/*.cpp
	${name}/*.h
	${name}/*.hpp
	${name}/*.inc
)
add_executable(${name} ${SRCS})

file(GLOB_RECURSE assets ${name}/res/*.*)
foreach(asset ${assets})
	file(COPY "${asset}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")
endforeach()

if (EMSCRIPTEN)
	set_target_properties(${name} PROPERTIES LINK_FLAGS "-sFILESYSTEM=0")
endif()

########################################################################
########################################################################

set(name test3)
include_directories(
	${name}
)
set(SRCS)
file(GLOB_RECURSE SRCS
	${name}/*.c
	${name}/*.cpp
	${name}/*.h
	${name}/*.hpp
	${name}/*.inc
)
add_executable(${name} ${SRCS})
target_link_libraries(${name} xx2dw pugixml libzstd_static)

if (EMSCRIPTEN)
	set_target_properties(${name} PROPERTIES LINK_FLAGS "--preload-file ${CMAKE_CURRENT_LIST_DIR}/${name}/res@res")
endif()


########################################################################
########################################################################

set(name examples)
include_directories(
	${name}
	spine-runtimes/spine-cpp/spine-cpp/include
)
set(SRCS)
file(GLOB_RECURSE SRCS
	${name}/*.c
	${name}/*.cpp
	${name}/*.h
	${name}/*.hpp
	${name}/*.inc
)
add_executable(${name} ${SRCS})
target_link_libraries(${name} xx2dw pugixml libzstd_static)

if (EMSCRIPTEN)
	set_target_properties(${name} PROPERTIES LINK_FLAGS "--preload-file ${CMAKE_CURRENT_LIST_DIR}/res@res")
endif()
