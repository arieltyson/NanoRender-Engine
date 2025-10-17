add_executable(nanorender_example main.cpp)

target_link_libraries(nanorender_example PRIVATE nanorender)

set_target_properties(
    nanorender_example
    PROPERTIES
        OUTPUT_NAME "nanorender-example"
)
