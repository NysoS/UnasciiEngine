project(Exar)

set (SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set (INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

cmake_policy(SET CMP0076 NEW) 
include(../cmake/functions.cmake)

add_library(${PROJECT_NAME} SHARED)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC 
    $<BUILD_INTERFACE:${INC_DIR}>
    $<INSTALL_INTERFACE:include>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Exar/Internal
)

target_compile_definitions(${PROJECT_NAME} PRIVATE EXAR_BUILD_DLL)
target_precompile_headers(${PROJECT_NAME} PRIVATE "${INC_DIR}/${PROJECT_NAME}/exarpch.h")

add_subdirectory(include)
add_subdirectory(src)

get_target_property(ALL_PROJECT_SOURCES ${PROJECT_NAME} SOURCES)

if(ALL_PROJECT_SOURCES)
    source_group(
        TREE "${CMAKE_CURRENT_SOURCE_DIR}"
        PREFIX ""
        FILES ${ALL_PROJECT_SOURCES}
    )
endif()

install(
    DIRECTORY ${INC_DIR}/
    DESTINATION include
    PATTERN "Internal" EXCLUDE
)

if(CMAKE_VERSION VERSION_GREATER 3.8)
    set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
else()
    set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
endif()
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:
    /Zc:preprocessor
    /Zc:__cplusplus>
)