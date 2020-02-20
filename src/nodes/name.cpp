#include <nodes/name.h>

#include <nodes/utils.h>

NameNode::NameNode(Parser &parser, Node *parent) : Node(Type::Name, parent) {
    if (parser.nextWord() != "name")
        throw std::runtime_error("NAMELSSS ??");

    if (parser.peekWord() == "'") {
        QuotesValue value = parseString(this, parser, "'");
        if (!value.inserts.empty())
            throw std::runtime_error("lol you can't do that");

        name = value.text;
    } else {
        name = parser.nextWord();
    }
}