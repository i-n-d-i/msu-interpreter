#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>

enum LexemType {
    NUM_TYPE,
    OPER_TYPE,
    VAR_TYPE
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
    virtual void setValue(int newValue) {}
    virtual int getOpertype() {}
};

Lexem::Lexem() {
}
        //*********************
class Number: public Lexem {
    int value;
public:
    Number() {}
    Number(int newValue);
    int getValue();
    int getType();
    void print();
};

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
    ASSIGN,
    PLUS, MINUS,
    MULTIPLY, DIV, MOD,
    LBRACKET, RBRACKET
};

int PRIORITY[] = {
    -1,
    0, 0,
    1, 1, 1,
    -1, -1
};

std::string OPERTEXT[] = {
    ":=",
    "+", "-",
    "*", "/", "%",
    "(", ")"
};

class Oper: public Lexem {
    OPERATOR opertype;
    int operNum = 7;
public:
    Oper() {}
    Oper(std::string oper);
    int getPriority();
    int getType();
    int getOpertype();
    void print();
};

Oper::Oper(std::string oper) { 
    for(int i = 0; i < operNum; i++) {
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
std::map <std::string, int> VarTable;

class Variable: public Lexem {
    int value;
    std::string name;
public:
    Variable() {}
    Variable(std::string newName);
    void setValue(int newValue);     
    int getValue();
    void print();
    int getType();
};

Variable::Variable(std::string newName) {
    name = newName;
}

void Variable::setValue(int newValue) {
    value = newValue;
    VarTable[name] = value;
}

int Variable::getValue() {
    return VarTable[name];
}

void Variable::print() {
    std::cout << name;
}

int Variable::getType() {
    return VAR_TYPE;
}

                //********************
Lexem *getNumber(std::string codeline, int *i) {
    Lexem *number;
    int value = 0;
    while((*i < codeline.size()) && isdigit(codeline[*i])) {
        if (codeline[*i] == ' ' || codeline[*i] == '\t') {
            break;
        }
        value = value * 10 + (codeline[*i]) - '0';
        (*i)++;
    }
    number = new Number(value);
    return number;
}

Lexem *getVariable(std::string codeline, int *i) {
    Lexem *variable;
    std::string name;
    while((*i < codeline.size()) && isalpha(codeline[*i])) {
        if (codeline[*i] == ' ' || codeline[*i] == '\t') {
            break;
        }
        name += codeline[*i];
        (*i)++;
    }
    variable = new Variable(name);
    return variable;
}

Lexem *getOperation(std::string codeline, int *i) {
    Lexem *operation;
    std::string oper;
    while ((*i < codeline.size()) && isalpha(codeline[*i]) == 0 && isdigit(codeline[*i]) == 0) {
        if (codeline[*i] == ' ' || codeline[*i] == '\t') {
            break;
        }
        oper += codeline[*i];
        (*i)++;
    }
    operation = new Oper(oper);
    return operation;
}

void printVector(std::vector<Lexem*> &vector) {
    for (auto lexem: vector) {
        lexem -> print();
        std::cout << ' ';
    }
    std::cout << std::endl;
}

std::vector<Lexem *> parseLexem(std::string codeline) {
    std::vector<Lexem *> infix;
    Lexem *lexem;
    int i = 0;
    while (i < codeline.size()) {
        while(codeline[i] == ' ' || codeline[i] == '\t') {
            i++;
        }
        if (isdigit(codeline[i])) {
            lexem = getNumber(codeline, &i);
        } else if (isalpha(codeline[i])){
            lexem = getVariable(codeline, &i);
        } else {
            lexem = getOperation(codeline, &i);
        }
        infix.push_back(lexem);
    }
    printVector(infix);
    return infix;
}

std::vector<Lexem *> buildPostfix(std::vector<Lexem *> infix) {
    std::vector<Lexem *> postfix;
    std::stack<Lexem *> st;
    for(auto s: infix) {
        if (s -> getType() == NUM_TYPE || s -> getType() == VAR_TYPE) {
            postfix.push_back(s);
        } else if (s -> getOpertype() == LBRACKET) {       
            st.push(s);                                      
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

Lexem *calculate(int opertype, Lexem *left, Lexem *right) {
    int val = 0, l = 0, r = 0;
    Lexem *value;
    if (opertype == ASSIGN) {
        val = right -> getValue();
        left -> setValue(val);
        value = new Number(val);
        return value;
    }
    l = left -> getValue();
    r = right -> getValue();
    if (opertype == PLUS) {
        val = l + r;
        value = new Number(val);
    } else if (opertype == MINUS) {
        val = l - r;
        value = new Number(val);
    } else if (opertype == MULTIPLY) {
        val = l * r;
        value = new Number(val);
    } else if (opertype == DIV) {
        val = l / r;
        value = new Number(val);
    } else if (opertype == MOD) {
        val = l % r;
        value = new Number(val);
    }
    return value;
}

int evaluatePostfix(std::vector<Lexem *> postfix) {
    int answer;
    std::stack<Lexem *> st;
    Lexem *left_elem, *right_elem, *value;
    for (auto s: postfix) {
        if (s -> getType() == NUM_TYPE || s -> getType() == VAR_TYPE) {
            st.push(s);
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
    answer = value -> getValue();
    return answer;
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

