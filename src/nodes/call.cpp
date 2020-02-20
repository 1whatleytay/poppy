#include <nodes/call.h>

#include <nodes/expression.h>

json CallNode::build() {
    if (name == "say") {
        return { { "type", "say" }, { "source", children[0]->build() } };
    } else {
        return { { "type", "call" } };
    }
}

CallNode::CallNode(Parser &parser, Node *parent) : Node(Type::Call, parent) {
    name = parser.nextWord();

    if (parser.nextWord() != "(")
        throw std::runtime_error("scuexem");

    if (name == "say") {
        // takes one param
        children.push_back(ExpressionNode::eval(parser, this));
    }

    if (parser.nextWord() != ")")
        throw std::runtime_error("Wat");
}