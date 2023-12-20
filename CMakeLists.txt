set(PROJ_NAME Laba14)
set(PROJ_SRC 
				"main.cpp"
				"src/application.cpp"
				"src/application.h"		
				"src/logger.hpp"
				"src/callback_manager.h"
				"src/callback_manager.cpp"
				"src/renderer.cpp"
				"src/renderer.h"
				"src/shader_program.cpp"
				"src/shader_program.h" 
				"src/buffer_objects.cpp" 
				"src/buffer_objects.h" 
				"src/resource_manager.cpp" 
				"src/resource_manager.h"
				"src/texture.h"
				"src/texture.cpp"
				"src/sub_texture.h"
				"src/sub_texture.cpp"
				"src/stb_image.h"
				"src/object.h"
				"src/cube.h"
				"src/cube.cpp"
				"src/tetra.h"
				"src/tetra.cpp"
				"src/cub_mixed_textures.h"
				"src/cub_mixed_textures.cpp"
				"src/mesh.h"
				"src/mesh.cpp"
				"src/circle.h" 
				"src/circle.cpp" "src/camera.h" "src/camera.cpp")

add_executable (${PROJ_NAME} ${PROJ_SRC})

target_include_directories(${PROJ_NAME} PRIVATE src)

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJ_NAME} PRIVATE ${OPENGL_LIBRARIES} glfw glad glm)

set_target_properties(${PROJ_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/)
add_custom_command(TARGET ${PROJ_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/${PROJ_NAME}/res $<TARGET_FILE_DIR:${PROJ_NAME}>/res)