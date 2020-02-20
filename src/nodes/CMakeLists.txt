add_library(nodes STATIC
    include/nodes/node.h
    include/nodes/code.h
    include/nodes/if.h
    include/nodes/call.h
    include/nodes/expression.h
    include/nodes/statement.h
    include/nodes/loop.h
    include/nodes/name.h
    include/nodes/break.h
    include/nodes/utils.h

    node.cpp
    code.cpp
    if.cpp
    call.cpp
    expression.cpp
    statement.cpp
    loop.cpp
    name.cpp
    break.cpp
    utils.cpp)

target_include_directories(nodes PUBLIC include)
target_link_libraries(nodes PUBLIC parser nlohmann_json)
