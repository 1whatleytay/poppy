#include <nodes/statement.h>

#include <nodes/expression.h>

json StatementNode::build() {
    assert(children.size() == 1);

    return {
        { "type", "assign" },
        { "destination", destination },
        { "source", children[0]->build() },
    };
}

StatementNode::StatementNode(Parser &parser, Node *parent) : Node(Type::Statement, parent) {
    destination = parser.nextWord();

    if (parser.nextWord() != "=")
        throw std::runtime_error("wtf");

    children.push_back(ExpressionNode::eval(parser, this));
}