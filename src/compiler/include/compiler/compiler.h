#pragma once

#include <string>

#include <nodes/node.h>

class Compiler {
    std::string source;

public:
    json compile();

    explicit Compiler(std::string source);
};
