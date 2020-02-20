#include <compiler/compiler.h>

#include <nodes/code.h>
#include <nodes/name.h>

json Compiler::compile() {
    Parser parser(source);

    std::shared_ptr<Node> root = std::make_shared<CodeNode>(parser, nullptr);

    json result;

    Node *nameNode = root->searchThis([](Node *node) {
        return node->type == Node::Type::Name;
    });

    if (nameNode) {
        result["name"] = reinterpret_cast<NameNode *>(nameNode)->name;
    }

    result["code"] = root->build();

    return result;
}

Compiler::Compiler(std::string source) : source(std::move(source)) {

}
