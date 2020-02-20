#include <nodes/expression.h>

#include <nodes/utils.h>

#include <queue>

std::vector<std::string> binops = {
    "is",
    "and",
    "or",
};

std::shared_ptr<ExpressionNode> ExpressionNode::eval(Parser &parser, Node *parent) {
    std::vector<std::shared_ptr<ExpressionNode>> nodes;
    std::vector<std::string> keywords;

    while (true) {
        nodes.push_back(std::make_shared<ExpressionNode>(parser, parent));

        std::string check = parser.peekWord();

        if (std::find(binops.begin(), binops.end(), check) != binops.end()) {
            parser.nextWord();
            keywords.push_back(check);
        } else {
             break;
        }
    }

    for (const std::string &op : binops) {
        for (uint32_t a = 0; a < keywords.size(); a++) {
            std::shared_ptr<ExpressionNode> expression = std::make_shared<ExpressionNode>(parent);

            std::shared_ptr<ExpressionNode> paramA = nodes[a];
            std::shared_ptr<ExpressionNode> paramB = nodes[a + 1];

            expression->children.push_back(paramA);
            expression->children.push_back(paramB);

            paramA->parent = expression.get();
            paramB->parent = expression.get();

            expression->content = keywords[a];

            keywords.erase(keywords.begin() + a);
            nodes.erase(nodes.begin() + a + 1);
            nodes[a] = expression;
        }
    }

    assert(nodes.size() == 1);
    assert(keywords.empty());

    return nodes[0];
}

json ExpressionNode::build() {
    switch (expression) {
        case Expression::Variable:
            return { { "type", "variable" }, { "content", content } };
        case Expression::Quotes:
            return { { "type", "string" }, { "content", content } };
        case Expression::Number:
            return { { "type", "string" }, { "content", std::stold(content) } };
        case Expression::Method:
            if (content == "input")
                return { { "type", "input" } };
            else
                return { { "type", "method" }, { "content", content } }; // shouldnt happen
        case Expression::Concat: {
            std::vector<json> array;

            for (const std::shared_ptr<Node> &node : children) {
                array.push_back(node->build());
            }

            return { { "type", "concat" }, { "content", array } };
        }
        case Expression::Comparator: {
            assert(children.size() == 2);
            return {
                { "type", content },
                { "a", children[0]->build() },
                { "b", children[1]->build() },
            };
        }
        default:
            throw std::runtime_error("what lol");
    }
}

ExpressionNode::ExpressionNode(Node *parent) : Node(Type::Expression, parent), expression(Expression::Comparator) { }

ExpressionNode::ExpressionNode(Parser &parser, Node *parent) : Node(Type::Expression, parent) {
    std::string name = parser.nextWord();

    if (name == "'") { // string
        parser.rollback(); // parseString needs quote for some reason lol
        QuotesValue value = parseString(parent, parser, "'");

        if (value.inserts.empty()) {
            expression = Expression::Quotes;
            content = value.text;
        } else {
            expression = Expression::Concat;
            assert(false); // fuck me
        }
    } else {
        content = name;

        if (parser.peekWord() == "(") {
            expression = Expression::Method;
            parser.nextWord();
            // no content :) for input()
            if (parser.nextWord() != ")")
                throw std::runtime_error("eyo what");
        } else {
            expression = Expression::Variable;
        }
    }
}