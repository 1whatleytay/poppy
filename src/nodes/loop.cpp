#include <nodes/loop.h>

#include <nodes/code.h>

json LoopNode::build() {
    assert(children.size() == 1);

    return {
        { "type", "loop" },
        { "code", children[0]->build() }
    };
}

LoopNode::LoopNode(Parser &parser, Node *parent) : Node(Type::Loop, parent) {
    if (parser.nextWord() != "loop")
        throw std::runtime_error("Internal error related to a loop.");

    if (parser.nextWord() != "{")
        throw std::runtime_error("Missing { after opening a loop.");

    children.push_back(std::make_shared<CodeNode>(parser, this));

    if (parser.nextWord() != "}")
        throw std::runtime_error("Missing } after closing a loop.");
}