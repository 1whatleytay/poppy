#pragma once

#include <nodes/node.h>

class CodeNode : public Node {
public:
    json build() override;

    CodeNode(Parser &parser, Node *parent);
};