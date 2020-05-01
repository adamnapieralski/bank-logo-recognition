cmake_minimum_required(VERSION 3.0)
project(bank-logo-recognition)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-11")

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} include/)

add_executable(main src/main.cpp)

target_link_libraries(main ${OpenCV_LIBS})
