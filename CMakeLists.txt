cmake_minimum_required(VERSION 3.0)
project(bank-logo-recognition)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-11")

find_package(OpenCV REQUIRED)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.cpp src/processing/*.cpp include/*.h include/processing/*.h)
add_executable(main ${sources})
target_include_directories(main PUBLIC include PRIVATE src)

target_link_libraries(main ${OpenCV_LIBS})
