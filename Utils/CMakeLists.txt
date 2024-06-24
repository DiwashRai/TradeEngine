
add_library(utils INTERFACE)
add_library(Common::utils ALIAS utils)
target_include_directories(utils INTERFACE .)