project(Unascii)

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

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${INC_DIR}
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    Engine
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND_EXPAND_LISTS
)

source_group(
    TREE ${INC_DIR}
    PREFIX "Headers Files"
    FILES ${HEADERS}
)

source_group(
    TREE ${SRC_DIR}
    PREFIX "Source Files"
    FILES ${SOURCES}
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC 
    $<BUILD_INTERFACE:${INC_DIR}>
    $<INSTALL_INTERFACE:include>
)

install(
    DIRECTORY ${INC_DIR}/
    DESTINATION include
)

if(CMAKE_VERSION VERSION_GREATER 3.8)
    set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
else()
    set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
endif()
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)