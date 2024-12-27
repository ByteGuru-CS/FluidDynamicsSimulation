# Metal library implementation
add_library(METAL_LIB
        ${CMAKE_CURRENT_SOURCE_DIR}/metal_implementation.cpp
        )

# Metal cpp headers
target_include_directories(METAL_LIB PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/metal-cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/metal-cpp-extensions"
        )

# Metal cpp library (linker)
target_link_libraries(METAL_LIB
        "-framework Metal"
        "-framework MetalKit"
        "-framework AppKit"
        "-framework Foundation"
        "-framework QuartzCore"
        )
