# Get all project dir

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

FILE(GLOB sample_projects ${CMAKE_CURRENT_SOURCE_DIR}/*)

FILE(GLOB src ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
FILE(GLOB header ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

FILE(GLOB shader ${CMAKE_CURRENT_SOURCE_DIR}/shader/*.metal)

add_executable(Demo MACOSX_BUNDLE ${header} ${src} ${shader})


set_source_files_properties(${shader} PROPERTIES LANGUAGE METAL)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${header})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${shader})

set_property(TARGET Demo APPEND_STRING PROPERTY COMPILE_FLAGS "-fobjc-arc")
set_target_properties(Demo PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Demo")

target_link_libraries(Demo METAL_LIB)
