CMAKE_MINIMUM_REQUIRED(VERSION 3.23)
SET(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
SET(PROJECT_VERSION 0.0.${PATCH_VERSION})
SET(MY_PROJECT_NAME n5)
PROJECT(${MY_PROJECT_NAME} VERSION ${PROJECT_VERSION})





FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLUT REQUIRED)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(SDL2 REQUIRED)

#  OpenGL и GLUT
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})

# OpenGL example
ADD_EXECUTABLE(OpenGLProject_example opengl_example.cpp)
TARGET_LINK_LIBRARIES(OpenGLProject_example ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

# OOP example without GUI
ADD_EXECUTABLE(OOP_example oop_example.cpp)

# SDL example
ADD_EXECUTABLE(SDLProject_example sdl_example.cpp)
TARGET_LINK_LIBRARIES(SDLProject_example ${SDL2_LIBRARIES})



SET_TARGET_PROPERTIES(OpenGLProject_example SDLProject_example OOP_example PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)

IF (MSVC)
    TARGET_LINK_OPTIONS(OpenGLProject_example PRIVATE
            /W4
    )
    target_compile_options(SDLProject_example PRIVATE
            /W4
    )
    target_compile_options(OOP_example PRIVATE
            /W4
    )

else ()
    target_compile_options(OpenGLProject_example PRIVATE
            -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(SDLProject_example PRIVATE
            -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(OOP_example PRIVATE
            -Wall -Wextra -pedantic -Werror
    )

endif()