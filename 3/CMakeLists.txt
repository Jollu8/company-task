CMAKE_MINIMUM_REQUIRED(VERSION 3.23)

set(PATCH_VERSION "3" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})
set(MY_PROJECT_NAME n3)
project(${MY_PROJECT_NAME} VERSION ${PROJECT_VERSION})




FIND_PACKAGE(Boost  COMPONENTS system REQUIRED)
FIND_PACKAGE(Threads REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

SET(CMAKE_CXX_STANDARD 20)




ADD_EXECUTABLE(Client Client.cpp Common.h json.hpp)
TARGET_LINK_LIBRARIES(Client PRIVATE Threads::Threads ${Boost_LIBRARIES})

ADD_EXECUTABLE(Test test.cpp Common.h core.cpp deal.cpp json.hpp User.cpp doctest.h)
TARGET_LINK_LIBRARIES(Test PRIVATE Threads::Threads ${Boost_LIBRARIES})

ADD_EXECUTABLE(Server Server.cpp Common.h core.cpp deal.cpp json.hpp User.cpp
        core.h)
TARGET_LINK_LIBRARIES(Server PRIVATE Threads::Threads ${Boost_LIBRARIES})


SET_TARGET_PROPERTIES(Test Client Server PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)

if (MSVC)
    target_compile_options(Server PRIVATE
            /W4
    )
    target_compile_options(Client PRIVATE
            /W4
    )
    target_compile_options(Test PRIVATE
            /W4
    )

else ()
    target_compile_options(Server PRIVATE
            -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(Client PRIVATE
            -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(Test PRIVATE
            -Wall -Wextra -pedantic -Werror
    )

endif()