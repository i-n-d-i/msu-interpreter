#include <interpreter.hpp>

int main() {
    std::string codeline;
    std::vector< std::vector<Lexem *> > infixLines;
    std::vector< std::vector<Lexem *> > postfixLines;
    int value;
    while (std::getline(std::cin, codeline) && codeline != "exit") {
        infixLines.push_back(parseLexem(codeline));
    }
    //std::cout << "infix:" << std::endl;
    //print(infixLines);
    for (int row = 0; row < (int)infixLines.size(); row++) {
        initLabels(infixLines[row], row);
    }

    initJumps(infixLines);
    //std::cout << "initJumps:" << std::endl;
    //print(infixLines);

    for (auto infix: infixLines) {
        postfixLines.push_back(buildPostfix(infix));
    }
    //std::cout << "postfix:" << std::endl;
    //print(postfixLines);
    int row = 0;
    while (0 <= row && row < (int)postfixLines.size()) {
        row = evaluatePostfix(postfixLines, row, &value);
    }
    printMap();
    std::cout << "answer: " << value << std::endl;
    return 0;
}
