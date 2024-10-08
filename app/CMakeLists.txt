add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Ensure the C++20 standard is available.
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# Enforce UTF-8 encoding on MSVC.
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
endif()

# Enable warnings recommended for new projects.
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

if (EMSCRIPTEN)
    # Attach data folder to Emscripten builds.
    target_link_options(${PROJECT_NAME} PRIVATE --preload-file "${CMAKE_CURRENT_SOURCE_DIR}/../data@data" -fexceptions -lidbfs.js)
    target_compile_options(${PROJECT_NAME} PRIVATE -fexceptions -std=c++20)
    # Set output to html to generate preview pages, otherwise you'll have to make your own html.
    configure_file(
        ${PROJECT_SOURCE_DIR}/emscripten/index.html
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/index.html
    )

endif()


find_package(fmt CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(libtcod CONFIG REQUIRED)
find_package(cereal CONFIG REQUIRED)
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
        cpprl
        SDL2::SDL2
        SDL2::SDL2main
        libtcod::libtcod
        cereal::cereal
        fmt::fmt
)
