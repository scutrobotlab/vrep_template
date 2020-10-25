cmake_minimum_required(VERSION 3.12)

project(Robot)

#Make sure no in-source build -----------------------------------------------------------------------------#
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif ()

#Configure CMake and Compiler -----------------------------------------------------------------------------#
set( CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set( CMAKE_CXX_STANDARD 14)
set( CMAKE_CXX_FLAGS "-pthread")
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/bin) 
set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/lib) 

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message("Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for test_footStest_footPlacementtepPlan-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

option(LOCAL_SIMULATION "CoppeliaSim Client services" ON)
if(LOCAL_SIMULATION)
    add_definitions(-DLOCAL_SIMULATION)
    add_definitions(-DNON_MATLAB_PARSING
                    -DMAX_EXT_API_CONNECTIONS=255)
endif(LOCAL_SIMULATION)
#Add sub directories --------------------------------------------------------------------------------------#
add_subdirectory(kernel)
add_subdirectory(common)
add_subdirectory(interfaces)
add_subdirectory(user)
add_subdirectory(test)
