project(core_test)

# Source/Include files
include_directories(".")
file(GLOB SOURCE "*.cpp")

add_library(core_test STATIC ${SOURCE})

#Add common
#target_link_libraries(${PROJECT_NAME} common)
#include_directories("../common")



