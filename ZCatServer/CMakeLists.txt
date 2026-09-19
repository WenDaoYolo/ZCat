cmake_minimum_required(VERSION 3.2.1)
project(ZCatServer)

aux_source_directory(${CMAKE_SOURCE_DIR}/source SRC_LIST)
add_executable(ZCatServer ${SRC_LIST})
set_target_properties(
    ZCatServer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

target_include_directories(ZCatServer PRIVATE ${CMAKE_SOURCE_DIR}/include)
#target_include_directories(ZCatServer PRIVATE /usr/include)->default path
#target_link_directories(ZCatServer PRIVATE /usr/lib)       ->default path

target_link_libraries(ZCatServer PRIVATE pqxx pq)