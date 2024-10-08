include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(
    GLOB_RECURSE
    cpprl_tests_SOURCE_FILES
    CONFIGURE_DEPENDS
    "./src/*.cpp"
)

add_executable(roguelike_tests ${cpprl_tests_SOURCE_FILES})

target_link_libraries(roguelike_tests
 PRIVATE
  GTest::gtest_main
  cpprl)

include(GoogleTest)
gtest_discover_tests(roguelike_tests)
