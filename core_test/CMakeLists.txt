project(core_test)

# Source/Include files
include_directories(".")
file(GLOB SOURCE "*.cpp")

add_library(core_test STATIC ${SOURCE})



