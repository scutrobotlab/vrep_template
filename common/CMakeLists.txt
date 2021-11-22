cmake_minimum_required(VERSION 3.12)

# Source/Include files
file(GLOB COMMON_SOURCE "*.cpp")

#Static Library
add_library(common STATIC ${COMMON_SOURCE})
#For matplotlib-cpp
add_definitions(-DWITHOUT_NUMPY)
find_package(Python3 COMPONENTS Development)

#For eigen3
find_package(Eigen3 REQUIRED)

include_directories(${EIGEN3_INCLUDE_DIR})
target_include_directories(common PRIVATE ${Python3_INCLUDE_DIRS})
target_link_libraries(common Python3::Python)