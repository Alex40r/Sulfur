﻿# CMakeList.txt : CMake project for Sulfur, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
	cmake_policy(SET CMP0141 NEW)
	set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Sulfur")

# Add source to this project's executable.
add_executable (Sulfur Sources/Sulfur.cpp)

# Add all the source files to the project
file(GLOB_RECURSE SOURCES "Sources/*.cpp")
target_sources(Sulfur PRIVATE ${SOURCES})

# Add the include directory to the project
target_include_directories(Sulfur PRIVATE "Sources/")

if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET Sulfur PROPERTY CXX_STANDARD 20)
endif()

# Add the Vulkan SDK
find_package(Vulkan REQUIRED)
target_link_libraries(Sulfur Vulkan::Vulkan)

# Add the DXC shader compiler
option(USE_DXC "Use DXC shader compiler" ON)
if(USE_DXC)
	find_package(Vulkan COMPONENTS dxc)
else()
	set(Vulkan_dxc_FOUND FALSE)
endif()

# Add the GLFW library
add_subdirectory(External/glfw)
target_link_libraries(Sulfur glfw)

# Add the GLM library
add_subdirectory(External/glm)
target_link_libraries(Sulfur glm)

# Add all the shaders to the project
file(GLOB_RECURSE SHADERS "Shaders/*.hlsl")

# If the DXC shader compiler is available, use it to compile the shaders
foreach(SHADER ${SHADERS})
	get_filename_component(FILE_NAME ${SHADER} NAME)
	string(REGEX REPLACE "\\.[^.]*$" "" FILE_NAME_WITHOUT_EXTENTION ${FILE_NAME})
	set(SPIRV "${CMAKE_CURRENT_SOURCE_DIR}/Shaders/SPIR-V/${FILE_NAME_WITHOUT_EXTENTION}.spv")
	set(SPIRV_BUILD  "${CMAKE_CURRENT_BINARY_DIR}/Shaders/${FILE_NAME_WITHOUT_EXTENTION}.spv")

	if(Vulkan_dxc_exe_FOUND)
		message(STATUS "Shader ${SHADER} will be compiled with DXC")
		add_custom_command(
			OUTPUT ${SPIRV}
			COMMAND ${Vulkan_dxc_EXECUTABLE} -Zi -spirv -T lib_6_7 -Fo ${SPIRV} ${SHADER}
			DEPENDS ${SHADER})
	endif()

	# Add the SPIR-V files to the build
	add_custom_command(
		OUTPUT ${SPIRV_BUILD}
		COMMAND ${CMAKE_COMMAND} -E copy ${SPIRV} ${SPIRV_BUILD}
		DEPENDS ${SPIRV})

	list(APPEND SPIRV_BINARY_FILES ${SPIRV_BUILD})
endforeach(SHADER)

add_custom_target(shaders ALL DEPENDS ${SPIRV_BINARY_FILES})
add_dependencies(Sulfur shaders)

if(NOT USE_DXC)
	message(STATUS "DXC usage disabled")
elseif(Vulkan_dxc_exe_FOUND)
	message(STATUS "DXC found: ${Vulkan_dxc_EXECUTABLE}")
else()
	message(STATUS "DXC was not found. Shaders will NOT be compiled")
endif()