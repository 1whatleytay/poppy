#include <nodes/code.h>

#include <nodes/if.h>
#include <nodes/call.h>
#include <nodes/loop.h>
#include <nodes/name.h>
#include <nodes/break.h>
#include <nodes/statement.h>

#include <fmt/format.h>

json CodeNode::build() {
    std::vector<json> result;

    for (const std::shared_ptr<Node> &node : children) {
        if (node->type != Type::Name)
            result.push_back(node->build());
    }

    return result;
}

CodeNode::CodeNode(Parser &parser, Node *parent) : Node(Type::Code, parent) {
    while (!parser.reachedEnd()) {
        std::string keyword = parser.peekWord();
        if (keyword == "if") {
            children.push_back(std::make_shared<IfNode>(parser, this));
        } else if (keyword == "loop") {
            children.push_back(std::make_shared<LoopNode>(parser, this));
        } else if (keyword == "break") {
            children.push_back(std::make_shared<BreakNode>(parser, this));
        } else if (keyword == "name") {
            children.push_back(std::make_shared<NameNode>(parser, this));
        } else if (keyword == "}") {
            break;
        } else { // keyword no match
            parser.nextWord();
            std::string next = parser.peekWord();
            parser.rollback();
            if (next == "(") { // call
                children.push_back(std::make_shared<CallNode>(parser, this));
            } else if (next == "=" || next == "+" || next == "-") { // assign
                children.push_back(std::make_shared<StatementNode>(parser, this));
            } else {
                throw std::runtime_error(
                    fmt::format("Unknown statement with keyword {} (followed by {}).", keyword, next));
            }
        }
    }
}