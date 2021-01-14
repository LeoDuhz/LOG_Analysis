include_directories(${PROTOBUF_INCLUDE_DIR})

set(SOURCES
    examplereader.cpp
)

add_executable(examplereader ${SOURCES})
target_link_libraries(examplereader protobuf)
set_target_properties(examplereader PROPERTIES EXCLUDE_FROM_ALL true)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
