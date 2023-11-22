CMAKE_MINIMUM_REQUIRED(VERSION 3.23)

set(PATCH_VERSION "3" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})
set(MY_PROJECT_NAME n7)
project(${MY_PROJECT_NAME} VERSION ${PROJECT_VERSION})

FIND_PACKAGE(OpenSSL REQUIRED)
FIND_PACKAGE(Threads REQUIRED)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавьте этот блок для загрузки Google Test
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.tar.gz
)
FetchContent_MakeAvailable(googletest)

ADD_EXECUTABLE(Server Server.h runServer.cpp)
TARGET_LINK_LIBRARIES(Server PRIVATE OpenSSL::SSL ${CMAKE_THREAD_LIBS_INIT})
ADD_EXECUTABLE(Client Client.cpp)

#INCLUDE_DIRECTORIES(../test)
ADD_EXECUTABLE(Test  test.cpp Server.h)
TARGET_LINK_LIBRARIES(Test PRIVATE OpenSSL::SSL ${CMAKE_THREAD_LIBS_INIT} gtest_main)  # Ссылка на Google Test

SET_TARGET_PROPERTIES( Server PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)

IF (MSVC)
    TARGET_COMPILE_OPTIONS(Server PRIVATE
            /W4
    )
    TARGET_COMPILE_OPTIONS(Test PRIVATE
            /W4
    )

ELSE ()
    TARGET_COMPILE_OPTIONS(Server PRIVATE
            -Wall -Wextra -pedantic -Werror
    )
    TARGET_COMPILE_OPTIONS(Test PRIVATE
            -Wall -Wextra -pedantic -Werror
    )

ENDIF ()
