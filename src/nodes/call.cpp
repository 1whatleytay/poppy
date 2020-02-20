#include <nodes/call.h>

#include <nodes/expression.h>

#include <fmt/format.h>

json CallNode::build() {
    if (name == "say" || name == "delay") {
        return { { "type", name }, { "source", children[0]->build() } };
    } else {
        return { { "type", "call" } };
    }
}

CallNode::CallNode(Parser &parser, Node *parent) : Node(Type::Call, parent) {
    name = parser.nextWord();

    if (parser.nextWord() != "(")
        throw std::runtime_error(fmt::format("Internal error related to calling function {}.", name));

    if (name == "say" || name == "delay") {
        // takes one param
        children.push_back(ExpressionNode::eval(parser, this));
    }

    if (parser.nextWord() != ")")
        throw std::runtime_error(fmt::format("Missing ) related to calling function {}.", name));
}