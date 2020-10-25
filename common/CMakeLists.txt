cmake_minimum_required(VERSION 3.12)

add_definitions(-DWITHOUT_NUMPY)

# Source/Include files
file(GLOB COMMON_SOURCE "*.cpp")

#Static Library
add_library(common STATIC ${COMMON_SOURCE})

#For matplotlib-cpp
find_package(Python3 COMPONENTS Development)
target_include_directories(common PRIVATE ${Python3_INCLUDE_DIRS})
target_link_libraries(common Python3::Python)