project(kernel)

# Source/Include files
include_directories(".")
include_directories("controller/")
include_directories("common/")
file(GLOB SOURCE "*.cpp" "controller/*.cpp" )

add_library(kernel STATIC ${SOURCE})





