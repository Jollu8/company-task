cmake_minimum_required(VERSION 3.23)
project(tasks_exec)

set(CMAKE_CXX_STANDARD 20)

#include(FetchContent)

#FetchContent_Declare(
#        json
#        GIT_REPOSITORY https://github.com/nlohmann/json.git
#        GIT_TAG v3.10.4
#)
#
#FetchContent_MakeAvailable(json)

#add_subdirectory(${PROJECT_SOURCE_DIR}/1)
#add_subdirectory(${PROJECT_SOURCE_DIR}/2)
#add_subdirectory(${PROJECT_SOURCE_DIR}/3)
add_subdirectory(${PROJECT_SOURCE_DIR}/5)
add_executable(tasks_exec main.cpp)

#target_link_libraries(tasks_exec PRIVATE nlohmann_json::nlohmann_json)

