add_library(
    pathlib
    "${CMAKE_CURRENT_SOURCE_DIR}/src/pathtest.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/EnvironmentManager.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/options.cpp" 
)

target_include_directories(pathlib PUBLIC "${PROJECT_SOURCE_DIR}/include")

target_compile_definitions(pathlib PRIVATE VERSION=v0.0.1)

