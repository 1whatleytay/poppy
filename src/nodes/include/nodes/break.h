#pragma once

#include <nodes/node.h>

class BreakNode : public Node {
public:
    json build() override;

    BreakNode(Parser &parser, Node *parent);
};
