cmake_minimum_required(VERSION 3.10)

project(rhi VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(rhi
			RenderDevice.h
			RenderDevice.cpp)

# being a cross-platform target, we enforce standards conformance on MSVC
target_compile_options(rhi PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->")
target_compile_options(rhi PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/WX->")

target_link_libraries(rhi Vulkan::Vulkan)