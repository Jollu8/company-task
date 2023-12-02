CMAKE_MINIMUM_REQUIRED(VERSION 3.24)

set(PATCH_VERSION "3" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})
set(MY_PROJECT_NAME n7)
project(${MY_PROJECT_NAME} VERSION ${PROJECT_VERSION})

FIND_PACKAGE(OpenSSL REQUIRED)
FIND_PACKAGE(Threads REQUIRED)
#FIND_PACKAGE(fmt REQUIRED)


SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.tar.gz
)
FetchContent_MakeAvailable(googletest)

#  fmt
FetchContent_Declare(fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG master
)
FetchContent_MakeAvailable(fmt)


ADD_EXECUTABLE(Server Server.h runServer.cpp)
TARGET_LINK_LIBRARIES(Server PRIVATE OpenSSL::SSL ${CMAKE_THREAD_LIBS_INIT})

# Full server targets ====================

FILE(GLOB HEADERS_FULL_SERVER server_full/include/*.h)
FILE(GLOB SOURCES_FULL_SERVER server_full/src/*.cpp)
ADD_EXECUTABLE(FullServer
        ${HEADERS_FULL_SERVER}
        ${SOURCES_FULL_SERVER}
)

# Ссылка на fmt
TARGET_LINK_LIBRARIES(FullServer PRIVATE
        OpenSSL::SSL
        ${CMAKE_THREAD_LIBS_INIT}
        fmt::fmt
)

ADD_EXECUTABLE(Client Client.cpp
     )


ADD_EXECUTABLE(Test  test.cpp Server.h)
TARGET_LINK_LIBRARIES(Test PRIVATE OpenSSL::SSL ${CMAKE_THREAD_LIBS_INIT} gtest_main)  # Ссылка на Google Test

SET_TARGET_PROPERTIES( Server FullServer PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)

IF (MSVC)
    TARGET_COMPILE_OPTIONS(Server PRIVATE
            /W4
    )
    TARGET_COMPILE_OPTIONS(FullServer PRIVATE
            /W4
    )
    TARGET_COMPILE_OPTIONS(Client PRIVATE
            /W4
    )
    TARGET_COMPILE_OPTIONS(Test PRIVATE
            /W4
    )

ELSE ()
    TARGET_COMPILE_OPTIONS(Server PRIVATE
            -Wall -Wextra -pedantic -Werror
    )
    TARGET_COMPILE_OPTIONS(FullServer PRIVATE
            -Wall -Wextra -pedantic -Werror
    )
    TARGET_COMPILE_OPTIONS(Test PRIVATE
            -Wall -Wextra -pedantic -Werror
    )
    TARGET_COMPILE_OPTIONS(Client PRIVATE
            -Wall -Wextra -pedantic -Werror
    )

ENDIF ()

cmake_policy(SET CMP0135 NEW)

