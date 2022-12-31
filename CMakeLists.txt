cmake_minimum_required(VERSION 3.0.0)
project(buttplug-cpp VERSION 0.1.0)

include(CTest)
enable_testing()

file(GLOB SRCS src/*.cpp)

add_library(buttplug-cpp ${SRCS})

target_include_directories(buttplug-cpp
    PUBLIC inc
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


project(test-app)

add_executable(test-app EXCLUDE_FROM_ALL test.cpp)
target_link_libraries(test-app buttplug-cpp)

