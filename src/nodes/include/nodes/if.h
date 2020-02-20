#pragma once

#include <nodes/node.h>

class IfNode : public Node {
public:
    json build() override;

    IfNode(Parser &parser, Node *parent);
};