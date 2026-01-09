cmake_minimum_required(VERSION 3.8)

project(UnasciiEngine)

set (SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set (INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES
    CONFIGURE_DEPENDS
    ${SRC_DIR}/*.cpp
)

file(GLOB_RECURSE HEADERS
    CONFIGURE_DEPENDS
    ${INC_DIR}/*.hpp
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
source_group(
    TREE ${INC_DIR}
    PREFIX "Headers Files"
    FILES ${HEADERS}
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE ${INC_DIR}
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT UnasciiEngine)