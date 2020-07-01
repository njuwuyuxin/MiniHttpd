aux_source_directory(. http_srcs)
add_library(http ${http_srcs})
target_link_libraries(http -lz)