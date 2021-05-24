
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
    Lexem() {}
    virtual void print() {}
    virtual int getType() {}
    virtual int getValue() {}
    virtual void setValue(int newValue) {}
    virtual bool inLabTable() {}
    virtual std::string getName() {}
    virtual int getOpertype() {}
    virtual int getPriority() {}
};

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
    std::cout << "[" << value << "]";
}

int Number::getType() {
    return NUM_TYPE;
}
        
        //*********************
enum OPERATOR {
    GOTO, ASSIGN, COLON,
    LBRACKET, RBRACKET,
    OR,
    AND,
    BITOR,
    XOR,
    BITAND,
    EQ, NEQ,
    SHL, SHR,
    LEQ, LT, GEQ, GT,
    PLUS, MINUS,
    MULT, DIV, MOD
};

enum {
    UNDEFINED = -INT32_MAX
};

int PRIORITY[] = {
    0, 0, 0,
    1, 1,
    2,
    3,
    4,
    5,
    6,
    7, 7,
    8, 8,
    9, 9, 9, 9,
    10, 10,
    11, 11, 11
};

std::string OPERTEXT[] = {
    "goto", ":=", ":",
    "(", ")",
    "or",
    "and",
    "|",
    "^",
    "&",
    "==", "!=",
    "<<", ">>",
    "<=", "<", ">=", ">",
    "+", "-",
    "*", "/", "%",
};

class Oper: public Lexem {
    OPERATOR opertype;
public:
    Oper() {}
    Oper(std::string oper);
    int getPriority();
    int getType();
    int getOpertype();
    void print();
};

Oper::Oper(std::string oper) { 
    int n = sizeof(OPERTEXT) / sizeof(std::string);
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
    std::cout << "[" << OPERTEXT[(int) opertype] << "]";
}

                //********************
std::map <std::string, int> VarTable;
std::map <std::string, int> LabTable;

class Variable: public Lexem {
    int value;
    std::string name;
public:
    Variable() {}
    Variable(std::string newName);
    void setValue(int newValue);     
    int getValue();
    std::string getName();
    void print();
    int getType();
    bool inLabTable();
};

Variable::Variable(std::string newName) {
    name = newName;
}

void Variable::setValue(int newValue) {
    value = newValue;
    VarTable[name] = value;
}

std::string Variable::getName() {
    return name;
}

int Variable::getValue() {
    return VarTable[name];
}

void Variable::print() {
    std::cout << "[" << name << "]";
}

int Variable::getType() {
    return VAR_TYPE;
}

bool Variable::inLabTable() {
    if (LabTable.find(name) != LabTable.end()) {
        return true;
    }
    return false;
}

                //********************
class Goto: public Oper {
    int row;
    int oper;
public:
    Goto() {}
    Goto(int newOper);
    void setRow(int newRow);
    void setRow(const std::string &labelname);
    int getRow();
    void print();
};

Goto::Goto(int newOper) {
    row = UNDEFINED;
    oper = newOper;
}

void Goto::setRow(int newRow) {
    row = newRow;
}

void Goto::setRow(const std::string &labelname) {
    row = LabTable[labelname];
}

int Goto::getRow() {
    return row;
}

void Goto::print() {
    std::cout << "[" << OPERTEXT[oper] << " -> " << "row = " << row << "]";
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

Lexem *getOperation(std::string codeline, int *index) {
    Lexem *operation;
    int n = sizeof(OPERTEXT) / sizeof(std::string);
    for (int oper = 0; oper < n; oper++) {
        int len = OPERTEXT[oper].size();
        if (OPERTEXT[oper] == codeline.substr(*index, len)) {
            *index += OPERTEXT[oper].size();
            if(oper == GOTO) {
                operation = new Goto(oper);
            } else {
                operation = new Oper(OPERTEXT[oper]);
            }
            return operation;
        }
    }
    return nullptr;
}

void printVector(std::vector<Lexem*> &vector) {
    for (auto lexem: vector) {
        if (lexem == nullptr) {
            continue;
        }
        lexem->print();
        std::cout << ' ';
    }
    std::cout << std::endl;
}

void print(std::vector<std::vector<Lexem *>> lines) {
    for (int i = 0; i < (int)lines.size(); i++) {
        std::cout << i << ": ";
        printVector(lines[i]);
    }
}

void printMap() {
    std::cout << "Таблица переменных" << std::endl;
    for (std::map<std::string,int>::iterator it = VarTable.begin(); it != VarTable.end(); it++) {
        std::cout << it->first << " = " << it->second << std::endl;
    }
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
        } else {
            lexem = getOperation(codeline, &i);
            if (lexem == nullptr) {
                lexem = getVariable(codeline, &i);
            }
        }
        infix.push_back(lexem);
    }
    //printVector(infix);
    return infix;
}

void initLabels(std::vector<Lexem *> &infix, int row) {
    for (int i = 1; i < (int)infix.size(); i++) {
        if (infix[i - 1]->getType() == VAR_TYPE && infix[i]->getType() == OPER_TYPE) {
            if (infix[i]->getOpertype() == COLON) {
                LabTable[infix[i - 1]->getName()] = row;
                delete infix[i - 1];
                delete infix[i];
                infix[i - 1] = nullptr;
                infix[i] = nullptr;
                i++;
            }
        }
    }
}


void joinGotoAndLabel(Lexem *lexemvar, std::vector<Lexem *> &postfix) {
    if (postfix.back()->getOpertype() == GOTO) {
        Goto *lexemgoto = (Goto *)postfix.back();
        lexemgoto->setRow(lexemvar->getName());
    }
}

std::vector<Lexem *> buildPostfix(std::vector<Lexem *> infix) {
    std::vector<Lexem *> postfix;
    std::stack<Lexem *> st;
    for(auto s: infix) {
        if (s == nullptr) {
            continue;
        }
        if (s->getType() == NUM_TYPE) {
            postfix.push_back(s);
        } else if (s->getType() == VAR_TYPE) {
            if (s->inLabTable() == true) {
                joinGotoAndLabel(s, postfix);
            } else {
                postfix.push_back(s);
            }
        } else if (s->getOpertype() == GOTO) {
            while(!st.empty()) {
                 postfix.push_back(st.top());
                 st.pop();
            }
            postfix.push_back(s);
        } else if (s->getOpertype() == LBRACKET) {       
            st.push(s);                                      
        } else if (s->getOpertype() == RBRACKET) {  
            while (true) {
                if (st.top()->getOpertype() == LBRACKET)
                    break;
                postfix.push_back(st.top());
                st.pop();
            }
            st.pop();
        } else if (!st.empty()){
            while(!st.empty() && st.top()->getPriority() >= s->getPriority()) {
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
    //printVector(postfix);
    return postfix;
}

Lexem *calculate(int opertype, Lexem *left, Lexem *right) {
    int val = 0, l = 0, r = 0;
    Lexem *value;
    if (opertype == ASSIGN) {               
        val = right->getValue();
        left->setValue(val);
        value = new Number(val);
        return value;
    }
    l = left->getValue();
    r = right->getValue();
    if (opertype == OR) {
        val = bool(l || r);
        value = new Number(val);
    } else if(opertype == AND) {
        val = bool(l && r);
        value = new Number(val);
    } else if (opertype == BITOR) {
        val = l | r;
        value = new Number(val);
    } else if (opertype == XOR) {
        val = l ^ r;
        value = new Number(val);
    } else if (opertype == BITAND) {
        val = l & r;
        value = new Number(val);
    } else if (opertype == EQ) {
        val = bool(l == r);
        value = new Number(val);
    } else if (opertype == NEQ) {
        val = bool(l != r);
        value = new Number(val);
    } else if (opertype == LEQ) {
        val = bool(l <= r);
        value = new Number(val);
    } else if (opertype == LT) {
        val = bool(l < r);
        value = new Number(val);
    } else if (opertype == GEQ) {
        val = bool(l >= r);
        value = new Number(val);
    } else if (opertype == GT) {
        val = bool(l > r);
        value = new Number(val);
    } else if (opertype == SHL) {
        val = l << r;
        value = new Number(val);
    } else if (opertype == SHR) {
        val = l >> r;
        value = new Number(val);
    } else if (opertype == PLUS) {
        val = l + r;
        value = new Number(val);
    } else if (opertype == MINUS) {
        val = l - r;
        value = new Number(val);
    } else if (opertype == MULT) {
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

int evaluatePostfix(std::vector<Lexem *> postfix, int row, int *answer) {
    std::stack<Lexem *> st;
    Lexem *left_elem, *right_elem, *value;
    for (auto s: postfix) {
        if (s->getType() == NUM_TYPE || s->getType() == VAR_TYPE) {
            st.push(s);
        } else {
            if (s->getOpertype() == GOTO) {
                Goto *lexemgoto = (Goto *)s;
                return lexemgoto->getRow();
            }
            right_elem = st.top();
            st.pop();
            left_elem = st.top();
            st.pop();
            
            value = calculate(s->getOpertype(), left_elem, right_elem);
            st.push(value);
        }
    }
    value = st.top();
    st.pop();
    (*answer) = value->getValue();
    row++;
    return row;
}

int main() {
    std::string codeline;
    std::vector< std::vector<Lexem *> > infixLines;
    std::vector< std::vector<Lexem *> > postfixLines;
    int value;
    while (std::getline(std::cin, codeline) && codeline != "exit") {
        infixLines.push_back(parseLexem(codeline));
    }
    std::cout << "infix:" << std::endl;
    print(infixLines);
    for (int row = 0; row < (int)infixLines.size(); row++) {
        initLabels(infixLines[row], row);
    }
    for (auto infix: infixLines) {
        postfixLines.push_back(buildPostfix(infix));
    }
    std::cout << "postfix:" << std::endl;
    print(postfixLines);
    int row = 0;
    while (0 <= row && row < (int)postfixLines.size()) {
        row = evaluatePostfix(postfixLines[row], row, &value);
    }
    printMap();
    std::cout << "answer: " << value << std::endl;
    return 0;
}

