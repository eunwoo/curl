cmake_minimum_required(VERSION 3.0.0)
project(webcrawler VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(webcrawler main.cpp send_post.cpp send_request.cpp)
target_link_libraries(webcrawler curl pthread)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
