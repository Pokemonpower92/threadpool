cmake_minimum_required(VERSION 3.0.0)
project(threadpool VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

add_library(threadpool src/threadpool.cpp)
target_include_directories(threadpool PUBLIC ./include)

set_target_properties(threadpool PROPERTIES PUBLIC_HEADER ./include/threadpool.h)
install(TARGETS threadpool LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
