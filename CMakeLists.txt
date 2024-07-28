cmake_minimum_required( VERSION 3.6 )
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


project( PhysicsOneOne )

find_package( OpenGL REQUIRED )

include_directories( ${OPENGL_INCLUDE_DIRS} )

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )

add_subdirectory( lib/glfw )
add_subdirectory( lib/glm )

include_directories(lib/glad/include)
add_library(glad STATIC lib/glad/src/glad.c)

## Download only the stb_image.h libary, and add as static libary
file(DOWNLOAD
    https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
    ${CMAKE_SOURCE_DIR}/lib/stb/include/stb/stb_image.h)
include_directories(lib/stb/include)
add_library(stb STATIC lib/stb/stb_image.cpp)


if( MSVC )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
endif()

set( PhysicsOneOneSource
        "src/main.cpp"

        "src/Engine.hpp"

        "src/Logger.hpp" 
        "src/Logger.cpp"

        "src/Debug/gl_debug.hpp" 
        "src/Debug/gl_debug.cpp"
        
        "src/Math.hpp"

        "src/Rendering/ShaderProgram.hpp" 
        "src/Rendering/ShaderProgram.cpp"

        "src/Rendering/VertexBuffer.hpp" 
        "src/Rendering/VertexBuffer.cpp"

        "src/Rendering/IndexBuffer.hpp" 
        "src/Rendering/IndexBuffer.cpp"

        "src/Rendering/VertexArrayObject.hpp"
        "src/Rendering/VertexArrayObject.cpp"   
        
        "src/Rendering/Renderer.hpp" 
        "src/Rendering/Renderer.cpp"
        
        "src/Rendering/Texture.hpp"
        "src/Rendering/Texture.cpp"
        
        "src/Camera.hpp"
        "src/Camera.cpp"

        "src/Window.hpp"
        "src/Window.cpp"

        "src/Event.hpp"

        "src/Scene/SceneBase.hpp"
        "src/Scene/RotatingCube.hpp"
        "src/Scene/RotatingCube.cpp"
        "src/Scene/LightCube.hpp"
        "src/Scene/LightCube.cpp"
        
)

add_executable( PhysicsOneOne WIN32 ${PhysicsOneOneSource})

target_link_libraries( PhysicsOneOne ${OPENGL_LIBRARIES} glfw glad glm stb)
target_compile_definitions(PhysicsOneOne PRIVATE $<$<CONFIG:Debug>:DEBUG>)

## copy all assets to bin directory.
## TODO: Make this more specific, copy directory is not great..
add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
add_dependencies(PhysicsOneOne copy_assets)

if( MSVC )
	set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT PhysicsOneOne )
endif()