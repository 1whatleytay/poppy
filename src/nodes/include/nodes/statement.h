#pragma once

#include <nodes/node.h>

class StatementNode : public Node {
public:
    std::string destination;

    json build() override;

    StatementNode(Parser &parser, Node *parent);
};
