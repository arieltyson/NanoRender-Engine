add_executable(nanorender_example main.cpp)

target_link_libraries(nanorender_example PRIVATE nanorender)

target_include_directories(
    nanorender_example
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../external/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/../include/ThirdParty/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/../external
        ${CMAKE_CURRENT_SOURCE_DIR}/../include/ThirdParty/tinyobjloader
)

set_target_properties(
    nanorender_example
    PROPERTIES
        OUTPUT_NAME "nanorender-example"
)
