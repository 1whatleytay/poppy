#include <nodes/utils.h>

#include <nodes/expression.h>

#include <parser/parser.h>

#include <sstream>

QuotesValue parseString(Node *parent, Parser &parser, const std::string &quote) {
    std::string self = parser.nextWord();
    assert(self == quote);

    QuotesValue value = { };

    std::stringstream output;
    parser.mode = Parser::Mode::Original;

    while (!parser.reachedEnd()) {
        output << parser.untilNextWords({"\\", quote});
        std::string next = parser.nextWord();
        if (next == quote) {
            break;
        } else if (next == "\\") {
            std::string nextSymbol = parser.nextWord();
            if (nextSymbol == "(") {
                QuotesInsertPoint insert = { };
                insert.insertPoint = output.str().size();
                insert.expressionNode = ExpressionNode::eval(parser, parent);
                value.inserts.push_back(insert);
                if (parser.nextWord() != ")")
                    throw std::exception();
            } else if (nextSymbol == "'") {
                output << "'";
            } else {
                throw std::exception();
            }
        }
    }

    parser.mode = Parser::Mode::Generic;

    value.text = output.str();

    return value;
}

std::string parseDigit(Parser &parser) {
    std::string text = parser.nextWord();
    if (parser.peekWord() == ".") {
        text += parser.nextWord() + parser.nextWord(); // .12
    }

    return text;
}
