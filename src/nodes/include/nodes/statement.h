#pragma once

#include <nodes/node.h>

class StatementNode : public Node {
public:
    std::string destination;

    std::string op = "=";

    json build() override;

    StatementNode(Parser &parser, Node *parent);
};
