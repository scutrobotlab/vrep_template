project(core_test)

# Source/Include files
include_directories(".")
include_directories("controller/")
include_directories("common/")
file(GLOB SOURCE "*.cpp" "controller/*.cpp" )

add_library(core_test STATIC ${SOURCE})

#Add common
#target_link_libraries(${PROJECT_NAME} common)
#include_directories("../common")



