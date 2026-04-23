# This file is part of the "Learn WebGPU for C++" book.
#   https://eliemichel.github.io/LearnWebGPU
# 
# MIT License
# Copyright (c) 2022-2025 Elie Michel and the wgpu-native authors
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# This file is part of the "Learn WebGPU for C++" book.
#   https://eliemichel.github.io/LearnWebGPU
#
# MIT License
# Copyright (c) 2022-2024 Elie Michel and the wgpu-native authors
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# Target architecture detection (thank you CMake for not providing that...)
function(detect_system_architecture)
	if (NOT ARCH)
		set(SYSTEM_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR})
		if (SYSTEM_PROCESSOR STREQUAL "AMD64" OR SYSTEM_PROCESSOR STREQUAL "x86_64")
			if (CMAKE_SIZEOF_VOID_P EQUAL 8)
				set(ARCH "x86_64")
			elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
				set(ARCH "i686")
			endif()
		elseif (SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64|armv8|arm)$")
			set(ARCH "aarch64")
		elseif(SYSTEM_PROCESSOR MATCHES "^(armv7|armv6|armhf)$")
			set(ARCH "arm")
		else()
			message(WARNING "Unknown architecture: ${SYSTEM_PROCESSOR}")
			set(ARCH "unknown")
		endif()
	endif()
	set(ARCH "${ARCH}" PARENT_SCOPE)
endfunction()

# Create the name of the static or shared library file given the target system.
function(build_lib_filename OUT_VAR LIB_NAME USE_SHARED_LIB)
	set(STATIC_LIB_EXT)
	set(SHARED_LIB_EXT)
	set(STATIC_LIB_PREFIX)
	set(SHARED_LIB_PREFIX)

	if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

		set(SHARED_LIB_EXT "dll")
		if (MSVC)
			set(STATIC_LIB_EXT "lib")
		else()
			set(STATIC_LIB_EXT "a")
			set(STATIC_LIB_PREFIX "lib")
		endif()

	elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")

		set(STATIC_LIB_EXT "a")
		set(SHARED_LIB_EXT "so")
		set(STATIC_LIB_PREFIX "lib")
		set(SHARED_LIB_PREFIX "lib")

	elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")

		set(STATIC_LIB_EXT "a")
		set(SHARED_LIB_EXT "dylib")
		set(STATIC_LIB_PREFIX "lib")
		set(SHARED_LIB_PREFIX "lib")

	else()

		message(FATAL_ERROR "Platform system '${CMAKE_SYSTEM_NAME}' not supported.")

	endif()

	if (USE_SHARED_LIB)
		set(${OUT_VAR} "${SHARED_LIB_PREFIX}${LIB_NAME}.${SHARED_LIB_EXT}" PARENT_SCOPE)
	else()
		set(${OUT_VAR} "${STATIC_LIB_PREFIX}${LIB_NAME}.${STATIC_LIB_EXT}" PARENT_SCOPE)
	endif()
endfunction()


include(FetchContent)

set(WGPU_LINK_TYPE "SHARED" CACHE STRING "Whether the wgpu-native WebGPU implementation must be statically or dynamically linked. Possible values are STATIC and SHARED")
set_property(CACHE WGPU_LINK_TYPE PROPERTY STRINGS SHARED STATIC)
set(WGPU_VERSION "v29.0.0.0" CACHE STRING "Version of the wgpu-native WebGPU implementation to use. Must correspond to the tag name of an existing release on https://github.com/gfx-rs/wgpu-native/releases. Warning: The webgpu.hpp file provided in include/ may not be compatible with other versions.")
set(WGPU_MIRROR "https://github.com/gfx-rs/wgpu-native" CACHE STRING "This is ultimately supposed to be https://github.com/gfx-rs/wgpu-native, where official binaries will be auto-released, but in the meantime we use a different mirror.")

# Not using emscripten, so we download binaries. There are many different
# combinations of OS, CPU architecture and compiler (the later is only
# relevant when using static linking), so here are a lot of boring "if".

detect_system_architecture()

# Check 'WGPU_LINK_TYPE' argument
set(USE_SHARED_LIB)
if (WGPU_LINK_TYPE STREQUAL "SHARED")
	set(USE_SHARED_LIB TRUE)
elseif (WGPU_LINK_TYPE STREQUAL "STATIC")
	set(USE_SHARED_LIB FALSE)
else()
	message(FATAL_ERROR "Link type '${WGPU_LINK_TYPE}' is not valid. Possible values for WGPU_LINK_TYPE are SHARED and STATIC.")
endif()

# Build URL to fetch
set(URL_OS)
set(URL_COMPILER)
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

	set(URL_OS "windows")

	if (MSVC)
		set(URL_COMPILER "msvc")
	else()
		set(URL_COMPILER "gnu")
	endif()

elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")

	set(URL_OS "linux")

elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")

	set(URL_OS "macos")

else()

	message(FATAL_ERROR "Platform system '${CMAKE_SYSTEM_NAME}' not supported by this release of WebGPU. You may consider building it yourself from its source (see https://github.com/gfx-rs/wgpu-native)")

endif()

set(URL_ARCH)
if (ARCH STREQUAL "x86_64")
	set(URL_ARCH "x86_64")
elseif (ARCH STREQUAL "aarch64")
	set(URL_ARCH "aarch64")
elseif (ARCH STREQUAL "i686" AND CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(URL_ARCH "i686")
else()
	message(FATAL_ERROR "Platform architecture '${ARCH}' not supported by this release of WebGPU. You may consider building it yourself from its source (see https://github.com/gfx-rs/wgpu-native)")
endif()

# We only fetch release builds (NB: this may cause issue when using static
# linking, but not with dynamic)
set(URL_CONFIG release)

# Finally build the URL. The URL_NAME is also used as FetchContent
# identifier (BTW it must be lowercase).
if (URL_COMPILER)
	set(URL_NAME "wgpu-${URL_OS}-${URL_ARCH}-${URL_COMPILER}-${URL_CONFIG}")
else()
	set(URL_NAME "wgpu-${URL_OS}-${URL_ARCH}-${URL_CONFIG}")
endif()
set(URL "${WGPU_MIRROR}/releases/download/${WGPU_VERSION}/${URL_NAME}.zip")

# Declare FetchContent, then make available
FetchContent_Declare(${URL_NAME}
	URL ${URL}
)
# TODO: Display the "Fetching" message only when actually downloading
message(STATUS "Fetching WebGPU implementation from '${URL}'")
FetchContent_MakeAvailable(${URL_NAME})
set(ZIP_DIR "${${URL_NAME}_SOURCE_DIR}")

# A pre-compiled target (IMPORTED) that is a dynamically linked library
# (SHARED, meaning .dll, .so or .dylib) or statically linked (.a or .lib).
if (USE_SHARED_LIB)
	add_library(webgpu SHARED IMPORTED GLOBAL)
else()
	add_library(webgpu STATIC IMPORTED GLOBAL)
endif()

# This is used to advertise the flavor of WebGPU that this zip provides
target_compile_definitions(webgpu INTERFACE WEBGPU_BACKEND_WGPU)

# This add webgpu.hpp
target_include_directories(webgpu INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include")

# TODO: There should be a wgpu-native-config.cmake file provided together with wgpu-native
build_lib_filename(BINARY_FILENAME "wgpu_native" ${USE_SHARED_LIB})
set(WEBGPU_RUNTIME_LIB "${ZIP_DIR}/lib/${BINARY_FILENAME}")
set_target_properties(
	webgpu
	PROPERTIES
		IMPORTED_LOCATION "${WEBGPU_RUNTIME_LIB}"
)

target_include_directories(webgpu INTERFACE
	"${ZIP_DIR}/include"
	"${ZIP_DIR}/include/webgpu"  # see https://github.com/gfx-rs/wgpu-native/pull/424
)

if (USE_SHARED_LIB)

	if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

		if (MSVC)
			set(STATIC_LIB_EXT "lib")
			set(STATIC_LIB_PREFIX "")
		else()
			set(STATIC_LIB_EXT "a")
			set(STATIC_LIB_PREFIX "lib")
		endif()

		set(WGPU_IMPLIB "${ZIP_DIR}/lib/${STATIC_LIB_PREFIX}${BINARY_FILENAME}.${STATIC_LIB_EXT}")
		set_target_properties(
			webgpu
			PROPERTIES
				IMPORTED_IMPLIB "${WGPU_IMPLIB}"
		)

	elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")

		set_target_properties(
			webgpu
			PROPERTIES
				IMPORTED_NO_SONAME TRUE
		)

	endif()

	message(STATUS "Using WebGPU runtime from '${WEBGPU_RUNTIME_LIB}'")
	set(WEBGPU_RUNTIME_LIB ${WEBGPU_RUNTIME_LIB} CACHE INTERNAL "Path to the WebGPU library binary")

	# The application's binary must find the .dll/.so/.dylib at runtime,
	# so we automatically copy it (it's called WEBGPU_RUNTIME_LIB in general)
	# next to the binary.
	# Also make sure that the binary's RPATH is set to find this shared library.
	function(target_copy_webgpu_binaries Target)
		add_custom_command(
			TARGET ${Target} POST_BUILD
			COMMAND
				${CMAKE_COMMAND} -E copy_if_different
				${WEBGPU_RUNTIME_LIB}
				$<TARGET_FILE_DIR:${Target}>
			COMMENT
				"Copying '${WEBGPU_RUNTIME_LIB}' to '$<TARGET_FILE_DIR:${Target}>'..."
		)
	endfunction()

else (USE_SHARED_LIB)

	if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

		target_link_libraries(
			webgpu
			INTERFACE
				d3dcompiler.lib
				Ws2_32.lib
				Userenv.lib
				ntdll.lib
				Bcrypt.lib
				Opengl32.lib
				Propsys.lib
				RuntimeObject.lib
		)

	elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")

		target_link_libraries(
			webgpu
			INTERFACE
				dl
				pthread
				m
		)

	elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")

		target_link_libraries(
			webgpu
			INTERFACE
				"-framework Metal"
				"-framework QuartzCore"
				"-framework MetalKit"
		)

	endif()

	function(target_copy_webgpu_binaries Target)
	endfunction()

endif (USE_SHARED_LIB)
