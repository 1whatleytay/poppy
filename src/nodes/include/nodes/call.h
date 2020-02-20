#pragma once

#include <nodes/node.h>

class CallNode : public Node {
public:
    std::string name;

    json build() override;

    CallNode(Parser &parser, Node *parent);
};
