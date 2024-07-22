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
add_library(GLAD STATIC lib/glad/src/glad.c)

if( MSVC )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
endif()

set( PhysicsOneOneSource
        "src/main.cpp"

        "src/Engine.hpp"

        "src/Logger.hpp" 
        "src/Logger.cpp"

        "src/gl_debug.hpp" 
        "src/gl_debug.cpp"

        "src/ShaderProgram.hpp" 
        "src/ShaderProgram.cpp"

        "src/VertexBuffer.hpp" 
        "src/VertexBuffer.cpp"

        "src/IndexBuffer.hpp" 
        "src/IndexBuffer.cpp"

        "src/VertexArrayObject.hpp"
        "src/VertexArrayObject.cpp"   
        
        "src/Renderer.hpp" 
        "src/Renderer.cpp"
)

add_executable( PhysicsOneOne WIN32 ${PhysicsOneOneSource})

target_link_libraries( PhysicsOneOne ${OPENGL_LIBRARIES} glfw GLAD glm)
target_compile_definitions(PhysicsOneOne PRIVATE $<$<CONFIG:Debug>:DEBUG>)

if( MSVC )
	set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT PhysicsOneOne )
endif()