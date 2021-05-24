#include <iostream>
#include <string>
#include <vector>
#include <stack>

enum LexemType {
    NUM_TYPE,
    OPER_TYPE,
};

         //********************
class Lexem {
    LexemType type;
public:
    Lexem();
    virtual void print() {}
    virtual int getType() {}
    virtual int getPriority() {}
    virtual int getValue() {}
    virtual int getOpertype() {}
};

Lexem::Lexem() {
}
        //*********************
class Number: public Lexem {
    int value;
public:
    Number();
    Number(int newValue);
    int getValue();
    int getType();
    void print();
};

Number::Number() {
}

Number::Number(int newValue) {
    value = newValue;
}

int Number::getValue() {
    return value;
}

void Number::print() {
    std::cout << value;
}

int Number::getType() {
    return NUM_TYPE;
}
        
        //*********************
enum OPERATOR {
    PLUS, MINUS,
    MULTIPLY, DIV, MOD,
    LBRACKET, RBRACKET
};

int PRIORITY[] = {
    0, 0,
    1, 1, 1,
    -1, -1
};

std::string OPERTEXT[] = {
    "+", "-",
    "*", "/", "%",
    "(", ")"
};

class Oper: public Lexem {
    OPERATOR opertype;
    int n = 7;
public:
    Oper();
    Oper(std::string oper);
    int getPriority();
    int getType();
    int getOpertype();
    void print();
};

Oper::Oper() {
}

Oper::Oper(std::string oper) { 
    for(int i = 0; i < n; i++) {
        if (OPERTEXT[i] == oper) {
            opertype = (OPERATOR)i;
        }
    }
}

int Oper::getOpertype() {
    return opertype;
}

int Oper::getPriority() {
    return PRIORITY[opertype];
}

int Oper::getType() {
    return OPER_TYPE;
}

void Oper::print() {
    std::cout << OPERTEXT[(int) opertype];
}

                //********************

void printVector(std::vector<Lexem*> &vector) {
    for (auto lexem: vector) {
        lexem -> print();
        std::cout << ' ';
    }
    std::cout << std::endl;
}


std::vector<Lexem *> parseLexem(std::string codeline) {
    std::vector<Lexem *> infix;
    std::string oper;
    int i = 0;
    while(i < codeline.size()) {
        while(codeline[i] == ' ' || codeline[i] == '\t') {
            i++;
        }
        if (isdigit(codeline[i])) {
            int value = 0;
            while (isdigit(codeline[i])) {
                value = value * 10 + codeline[i] - '0';
                i++;
            }
            infix.push_back(new Number(value));
        } else { 
            oper = codeline[i];
            infix.push_back(new Oper(oper));
            i++;
        }
    }
    //printVector(infix);
    return infix;
}

std::vector<Lexem *> buildPostfix(std::vector<Lexem *> infix) {
    std::vector<Lexem *> postfix;
    std::stack<Lexem *> st;
    for(auto s: infix) {
        if (s -> getType() == NUM_TYPE) {
            postfix.push_back(s);
        } else if (s -> getOpertype() == LBRACKET) {        //! рассмотреть случаи, когда неправильный ввод
            st.push(s);                                      // неправильное ко-во скобок
        } else if (s -> getOpertype() == RBRACKET) {  
            while (true) {
                if (st.top() -> getOpertype() == LBRACKET)
                    break;
                postfix.push_back(st.top());
                st.pop();
            }
            st.pop();
        } else if (!st.empty()){
            while(!st.empty() && st.top() -> getPriority() >= s -> getPriority()) {
                postfix.push_back(st.top());
                st.pop();
            }
            st.push(s);
        } else {
            st.push(s);
        }
    }
    while(!st.empty()) {
        postfix.push_back(st.top());
        st.pop();
    }
    printVector(postfix);
    return postfix;
}

int calculate(int opertype, int l, int r) {
    int value;
    if (opertype == PLUS) {
        value = l + r;
    } else if (opertype == MINUS) {
        value = l - r;
    } else if (opertype == MULTIPLY) {
        value = l * r;
    } else if (opertype == DIV) {
        value = l / r;
    } else if (opertype == MOD) {
        value = l % r;
    }
    return value;
}

int evaluatePostfix(std::vector<Lexem *> postfix) {
    std::stack<int> st;
    int value;
    int right_elem, left_elem;
    for (auto s: postfix) {
        if (s -> getType() == NUM_TYPE) {
            st.push(s -> getValue());
        } else {
            right_elem = st.top();
            st.pop();
            left_elem = st.top();
            st.pop();
            
            value = calculate(s -> getOpertype(), left_elem, right_elem);
            st.push(value);
        }
    }
    value = st.top();
    st.pop();
    return value;
}

int main() {
    std::string codeline;
    std::vector<Lexem *> infix;
    std::vector<Lexem *> postfix;
    int value;
    while (std::getline(std::cin, codeline)) {
        infix = parseLexem(codeline);
        postfix = buildPostfix(infix);
        value = evaluatePostfix(postfix);
        std::cout << "answer: " << value << std::endl;
    }
    return 0;
}

