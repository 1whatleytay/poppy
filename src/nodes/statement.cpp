#include <nodes/statement.h>

#include <nodes/expression.h>

#include <fmt/format.h>

json StatementNode::build() {
    assert(children.size() == 1);

    if (op == "=") {
        return {
            { "type", "assign" },
            { "destination", destination },
            { "source", children[0]->build() },
        };
    } else {
        return {
            { "type", "assign" },
            { "destination", destination },
            {
                "source",
                {
                    { "type", op },
                    { "a", { { "type", "variable" }, { "content", destination } } },
                    { "b", children[0]->build() }
                }
            }
        };
    }
}

StatementNode::StatementNode(Parser &parser, Node *parent) : Node(Type::Statement, parent) {
    destination = parser.nextWord();

    if (parser.peekWord() == "+" || parser.peekWord() == "-") {
        op = parser.nextWord();
    }

    if (parser.nextWord() != "=")
        throw std::runtime_error(fmt::format("Invalid statement operator {}.", parser.lastWord()));

    children.push_back(ExpressionNode::eval(parser, this));
}