
add_library(fix_compiled STATIC
    db_classes.cpp
    db_session.cpp
    db_traits.cpp
    db_types.cpp
)

add_library(Common::fix_compiled ALIAS fix_compiled)

target_include_directories(fix_compiled
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})