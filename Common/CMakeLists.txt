
add_library(Common STATIC
    db_classes.cpp
    db_session.cpp
    db_traits.cpp
    db_types.cpp
    ConsoleFileLogger.cpp
)

target_include_directories(Common
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
