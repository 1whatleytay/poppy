#pragma once

#include <nodes/node.h>

class NameNode : public Node {
public:
    std::string name;

    NameNode(Parser &parser, Node *parent);
};
