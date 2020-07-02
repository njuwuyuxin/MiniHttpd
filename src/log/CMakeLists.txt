aux_source_directory(. log_srcs)
add_library(log ${log_srcs})
target_link_libraries(log utils)