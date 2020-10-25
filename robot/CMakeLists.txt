project(Launcher)

include_directories(".")
file(GLOB_RECURSE USER_SOURCE *.cpp)

add_executable(${PROJECT_NAME} ${USER_SOURCE})

#Add kernel
include_directories("../kernel")
target_link_libraries(${PROJECT_NAME} kernel)

#Add interfaces
if(LOCAL_SIMULATION)
    add_definitions(-DLOCAL_SIMULATION)
endif(LOCAL_SIMULATION)

target_link_libraries(${PROJECT_NAME} interfaces)
include_directories("../interfaces")
include_directories("../interfaces/Simulation/coppeliaSim-client")
include_directories("../interfaces/Simulation/coppeliaSim-client/include")
include_directories("../interfaces/Simulation/coppeliaSim-client/include/stack")
include_directories("../interfaces/Simulation/coppeliaSim-client/remoteApi")

#Add common
target_link_libraries(${PROJECT_NAME} common)
include_directories("../common")

#Add GTK-3.0
find_package(PkgConfig REQUIRED)
	pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    message(STATUS "GTK3_INCLUDE_DIRS: ${GTK3_INCLUDE_DIRS}")
    message(STATUS "GKT3_LIBRARIES: ${GTK3_LIBRARIES}")
    message(STATUS "GTK3_LINK_LIBRARIES: ${GTK3_LINK_LIBRARIES}")
	include_directories(${GTK3_INCLUDE_DIRS})
	link_directories(${GTK3_LIBRARY_DIRS})
    list(APPEND FC_DEP_LIBS ${GTK3_LIBRARIES})
    
    target_link_libraries (${PROJECT_NAME} ${FC_DEP_LIBS})

