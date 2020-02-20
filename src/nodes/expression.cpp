#include <nodes/expression.h>

#include <nodes/utils.h>

#include <queue>

class BinOp {
public:
    std::string eval;

    std::vector<std::vector<std::string>> syntax;
};

std::vector<BinOp> binops = {
    { "+", { { "+" }, { "plus" } } },
    { "-", { { "-" }, { "minus" } } },
    { "isnt", { { "isnt" }, { "is", "not" }, { "!", "=" } } },
    { "is", { { "is" }, { "=", "=" } } },
    { "<=", { { "lte" }, { "<", "=" } } },
    { ">=", { { "gte" }, { ">", "=" } } },
    { "<", { { "lt" }, { "<" } } },
    { ">", { { "gt" }, { ">" } } },
    { "and", { { "and" }, { "&", "&" } } },
    { "or", { { "or" }, { "|", "|" } } },
};

//std::vector<std::string> binops = {
//    "+",
//    "-",
//    "is",
//    "isnt",
//    "lesser",
//    ">",
//    "and",
//    "or",
//};

std::shared_ptr<ExpressionNode> ExpressionNode::eval(Parser &parser, Node *parent) {
    std::vector<std::shared_ptr<ExpressionNode>> nodes;
    std::vector<std::string> keywords;

    while (true) {
        nodes.push_back(std::make_shared<ExpressionNode>(parser, parent));

        parser.select();

        bool match = false;
        for (const auto &pair : binops) {
            for (const std::vector<std::string> &syntax : pair.syntax) {
                match = true;
                for (const std::string &text : syntax) {
                    if (parser.nextWord() != text) {
                        match = false;
                        break;
                    }
                }

                if (match) {
                    keywords.push_back(pair.eval);
                    break;
                }

                parser.back();
            }

            if (match)
                break;
        }

        if (!match)
            break;
    }

    for (const auto &op : binops) {
        for (uint32_t a = 0; a < keywords.size(); a++) {
            if (op.eval != keywords[a])
                continue;

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
            return { { "type", "number" }, { "content", std::stold(content) } };
        case Expression::Method:
            if (content == "input")
                return { { "type", "input" } };
            else
                return { { "type", "method" }, { "content", content } }; // shouldnt happen
        case Expression::Concat: {
            std::vector<json> array;

            uint32_t start = 0;

            for (uint32_t a = 0; a < children.size(); a++) {
                std::string catchup = content.substr(start, indices[a] - start);
                if (!catchup.empty()) {
                    array.push_back({ { "type", "string" }, { "content", catchup } });
                }

                array.push_back(children[a]->build());

                start = indices[a];
            }

            // copy rest of string if there
            std::string catchup = content.substr(start);
            if (!catchup.empty()) {
                array.push_back({ { "type", "string" }, { "content", catchup } });
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

    if (name.empty())
        throw std::runtime_error("whatheheck");

    if (name == "'") { // string
        parser.rollback(); // parseString needs quote for some reason lol
        QuotesValue value = parseString(parent, parser, "'");
        content = value.text;

        if (value.inserts.empty()) {
            expression = Expression::Quotes;
        } else {
            expression = Expression::Concat;
            for (const QuotesInsertPoint &a : value.inserts) {
                children.push_back(a.expressionNode);
                indices.push_back(a.insertPoint);
            }
        }
    } else if (std::isdigit(name[0])) {
        parser.rollback();
        expression = Expression::Number;
        content = parseDigit(parser);
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