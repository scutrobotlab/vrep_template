cmake_minimum_required(VERSION 3.12)

# Source/Include files
include_directories(".")
file(GLOB COMMON_SOURCE "*.cpp")

#Static Library
add_library(common STATIC ${COMMON_SOURCE})

#For matplotlib-cpp
find_package(Python2 COMPONENTS Development NumPy)
target_include_directories(common PRIVATE ${Python2_INCLUDE_DIRS} ${Python2_NumPy_INCLUDE_DIRS})
target_link_libraries(common Python2::Python Python2::NumPy)