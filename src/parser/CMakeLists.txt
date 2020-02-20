add_library(parser STATIC
    include/parser/parser.h

    parser.cpp)

target_include_directories(parser PUBLIC include)
