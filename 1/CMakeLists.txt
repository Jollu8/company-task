cmake_minimum_required(VERSION 3.23)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})
set(MY_PROJECT_NAME n1)
project(${MY_PROJECT_NAME} VERSION ${PROJECT_VERSION})

add_executable(${MY_PROJECT_NAME}
        run.cpp
        OrderBookTester.h
        Order.h
)
set_target_properties(${MY_PROJECT_NAME} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)
if (MSVC)
    target_compile_options(${MY_PROJECT_NAME} PRIVATE
            /W4
    )

else ()
    target_compile_options(${MY_PROJECT_NAME} PRIVATE
            -Wall -Wextra -pedantic -Werror
    )

endif()

install(
        TARGETS ${MY_PROJECT_NAME}
        DESTINATION bin
        EXPORT ${MY_PROJECT_NAME}
)