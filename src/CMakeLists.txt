add_subdirectory(thread)
add_subdirectory(controller)
add_subdirectory(utils)
add_subdirectory(http)

aux_source_directory(. srcs)
add_library(src ${srcs})

target_link_libraries(src thread)
target_link_libraries(src controller)
target_link_libraries(src utils)
target_link_libraries(src http)