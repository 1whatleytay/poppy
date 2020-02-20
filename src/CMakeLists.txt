add_subdirectory(parser)
add_subdirectory(nodes)
add_subdirectory(compiler)

add_executable(poppy main.cpp)
target_link_libraries(poppy PUBLIC compiler)
