#pragma once

#include <nodes/node.h>

class ExpressionNode : public Node {
public:
    enum class Expression {
        Variable,
        Quotes,
        Number,
        Method,
        Concat,
        Comparator,
        Unknown
    };
    Expression expression = Expression::Unknown;

    std::string content;

    static std::shared_ptr<ExpressionNode> eval(Parser &parser, Node *parent);

    json build() override;

    explicit ExpressionNode(Node *parent);
    ExpressionNode(Parser &parser, Node *parent);
};