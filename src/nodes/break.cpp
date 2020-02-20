#include <nodes/break.h>

json BreakNode::build() {
    return {
        { "type", "break" }
    };
}

BreakNode::BreakNode(Parser &parser, Node *parent) : Node(Type::Break, parent) {
    if (parser.nextWord() != "break")
        throw std::runtime_error("impossible!");
}
