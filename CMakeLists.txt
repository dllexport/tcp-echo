cmake_minimum_required(VERSION 3.15)
project(echo)

set(CMAKE_CXX_STANDARD 17)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS regex system coroutine context thread serialization REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(client client.cpp)
    add_executable(echo_server echo_server.cpp)
    target_link_libraries (client ${Boost_LIBRARIES})
    target_link_libraries (echo_server ${Boost_LIBRARIES})
endif()


