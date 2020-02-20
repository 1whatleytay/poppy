#include <compiler/compiler.h>

#include <fstream>
#include <iostream>

int main(int count, char **args) {
    if (count != 2) {
        return 1;
    }

    std::ifstream stream(args[1], std::ios::ate);
    std::vector<char> data(stream.tellg());
    stream.seekg(0, std::ios::beg);
    stream.read(data.data(), data.size());
    stream.close();

    Compiler compiler(std::string(data.begin(), data.end()));
    json out = compiler.compile();

    std::cout << out.dump(4) << std::endl;

    return 0;
}
