file(
    GLOB_RECURSE
    cpprl_SOURCE_FILES
    CONFIGURE_DEPENDS  # Automatically reconfigure if source files are added/removed.
    "./src/*.cpp"
    "./include/*.hpp"
    "./include/*.h"
)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_library(cpprl ${cpprl_SOURCE_FILES})

target_include_directories(cpprl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
