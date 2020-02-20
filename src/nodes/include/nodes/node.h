#pragma once

// save me time
#include <parser/parser.h>

#include <nlohmann/json.hpp>

#include <string>
#include <vector>
#include <memory>
#include <functional>

using nlohmann::json;

typedef const std::vector<std::string> &Attributes;

class Node {
public:
    enum class Type {
        Code,
        Name,
        If,
        Call,
        Statement,
        Expression,
    };

    Type type;
    Node *parent;
    std::vector<std::shared_ptr<Node>> children;

    Node *searchThis(const std::function<bool(Node *)> &checker);
    Node *searchParent(const std::function<bool(Node *)> &checker);

    virtual json build();

    Node(Type type, Node *parent);
    virtual ~Node();
};
