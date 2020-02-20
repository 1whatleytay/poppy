#pragma once

#include <nodes/node.h>

class LoopNode : public Node {
public:
    json build() override;

    LoopNode(Parser &parser, Node *parent);
};