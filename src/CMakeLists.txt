# Get all project dir
FILE(GLOB sample_projects ${CMAKE_CURRENT_SOURCE_DIR}/*)

FILE(GLOB src ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
FILE(GLOB header ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

add_executable(Demo ${src})
target_link_libraries(Demo METAL_LIB)