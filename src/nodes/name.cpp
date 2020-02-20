#include <nodes/name.h>

#include <nodes/utils.h>

NameNode::NameNode(Parser &parser, Node *parent) : Node(Type::Name, parent) {
    if (parser.nextWord() != "name")
        throw std::runtime_error("No name provided.");

    if (parser.peekWord() == "'") {
        QuotesValue value = parseString(this, parser, "'");
        if (!value.inserts.empty())
            throw std::runtime_error("String templates used in name.");

        name = value.text;
    } else {
        name = parser.nextWord();
    }
}