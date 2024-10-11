
add_subdirectory(rhi)

# Add source to this project's executable.
add_executable(${TARGET_NAME} )

target_sources(${TARGET_NAME} 
	PRIVATE
				main.cpp)

set_property(TARGET ${TARGET_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:vulkan_guide>")

#target_include_directories(${TARGET_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(${TARGET_NAME} vma glm  stb_image)

target_link_libraries(${TARGET_NAME} rhi glfw)

add_dependencies(${TARGET_NAME} Shaders)
