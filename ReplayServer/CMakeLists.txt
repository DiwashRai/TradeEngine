
project(ReplayServer)
include(CMakePrintHelpers)

find_package(CLI11 CONFIG REQUIRED)
find_package(httplib CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(zstd CONFIG REQUIRED)
if(TARGET zstd::libzstd_static)
    set(ZSTD_TARGET zstd::libzstd_static)
elseif(TARGET zstd::zstd)
    set(ZSTD_TARGET zstd::zstd)
else()
    message(FATAL_ERROR "Zstd target not found.")
endif()
find_package(databento REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp
        src/ReplayServer.cpp
        src/ReplayServer.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        CLI11::CLI11
        ${ZSTD_TARGET}
        databento::databento
)
