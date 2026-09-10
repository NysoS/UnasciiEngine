set (HEADERS 
    Engine.hpp
    uaepch.h
)

add_subdirectories(
    Core
    Runtime
)

register_sources(Engine ${HEADERS})