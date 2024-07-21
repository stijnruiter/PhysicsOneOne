cmake_minimum_required( VERSION 3.6 )

project( PhysicsOneOne )

find_package( OpenGL REQUIRED )

include_directories( ${OPENGL_INCLUDE_DIRS} )

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )

add_subdirectory( lib/glfw )

include_directories(lib/glad/include)
add_library(GLAD STATIC lib/glad/src/glad.c)

if( MSVC )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
endif()

set( PhysicsOneOneSource
        src/main.cpp
        )

add_executable( PhysicsOneOne WIN32 ${PhysicsOneOneSource})

target_link_libraries( PhysicsOneOne ${OPENGL_LIBRARIES} glfw GLAD)

if( MSVC )
	set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT PhysicsOneOne )
endif()