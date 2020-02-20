#include <nodes/if.h>

#include <nodes/code.h>
#include <nodes/expression.h>

json IfNode::build() {
    std::vector<json> branches;

    for (uint32_t a = 0; a < children.size(); a++) {
        json result;

        if (children[a].get()->type == Type::Expression) {
            result["source"] = children[a].get()->build();
            a++;
        }

        result["code"] = children[a]->build();

        branches.push_back(result);
    }

    return {
        { "type", "if" },
        { "branches", branches }
    };
}

IfNode::IfNode(Parser &parser, Node *parent) : Node(Type::If, parent) {
    do {
        std::string next = parser.nextWord();

        if (next == "else") {
            next = parser.nextWord();
        }

        if (next == "if") {
            children.push_back(ExpressionNode::eval(parser, this));
        } else {
            parser.rollback();
        }

        if (parser.nextWord() != "{")
            throw std::runtime_error("syntax man");

        children.push_back(std::make_shared<CodeNode>(parser, this));

        if (parser.nextWord() != "}")
            throw std::runtime_error("hmm");
    } while (parser.peekWord() == "else");
}