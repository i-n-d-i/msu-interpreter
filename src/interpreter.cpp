#include <interpreter.hpp>

int PRIORITY[] = {
    -1, -1, -1,
    -1, -1,
    -1, -1,
    -1, -1,
    -1, -1, -1,
    0, 0,
    1,
    2,
    3,
    4,
    5, 
    6, 6,
    7, 7,
    8, 8, 8, 8,
    9, 9, 
    10, 10, 10
};

std::string OPERTEXT[] = {
    "def", "ret", "enddef",
    "if", "then",
    "else", "endif",
    "while", "endwhile",
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
    "*", "/", "%"
};

         //********************
Lexem::Lexem() {
}

        //*********************
Number::Number() {
}

Number::~Number() {
}

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
Oper::Oper() {
}

Oper::~Oper() {
}

Oper::Oper(int n) {
    opertype = (OPERATOR)n;
}

OPERATOR Oper::getOpertype() {
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
std::map <std::string, int> FunTable;
std::stack <int> RetAddr;

Variable::Variable() {
}     

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
Goto::Goto() {
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
    std::cout << "[<row " << row << ">" << OPERTEXT[oper] << "]";
}

            //********************
Function::Function() {
}

Function::Function(std::string newName, int newRow) {
    name = newName;
    ((Goto *)this)->setRow(newRow);
}

int Function::getType() {
    return FUNC_TYPE;
}

void Function::setName(std::string newName) {
    name = newName;
}

std::string Function::getName() {
    return name;
}

void Function::setRow(int newRow) {
    ((Goto *)this)->setRow(newRow);
}

int Function::getRow() {
    if (FunTable.find(name) != FunTable.end()) {
        return FunTable[name];
    } else {
        return -1;
    }
}

void Function::print() {
    std::cout << "[" << name << " -> " << ((Goto *)this)->getRow() << "]";
}

            //********************
Lexem *getNumber(std::string codeline, int *i) {
    Lexem *number;
    int value = 0;
    int len = codeline.size();
    while(*i < len && isdigit(codeline[*i])) {
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
    int len = codeline.size();
    while(*i < len && isalpha(codeline[*i])) {
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
    int n = sizeof(OPERTEXT) / sizeof(std::string);
    for (int oper = 0; oper < n; oper++) {
        int len = OPERTEXT[oper].size();
        if (OPERTEXT[oper] == codeline.substr(*index, len)) {
            *index += OPERTEXT[oper].size();
            if(oper == GOTO || oper == IF || oper == ELSE || oper == WHILE || oper == ENDWHILE || oper == RET) {
                return new Goto(oper);
            } else {
                return new Oper(oper);
            }
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
    int len = codeline.size();
    while (i < len) {
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
        if (infix[i - 1] == nullptr) {
            continue;
        }
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

void initJumps(std::vector<std::vector <Lexem *>> &infixLines) {
    std::stack <Goto *> stackIfElse;
    for (int row = 0; row < (int)infixLines.size(); row++) {
        for (int i = 0; i < (int)infixLines[row].size(); i++) {
            if (infixLines[row][i] == nullptr) {
                continue;
            }
            if (infixLines[row][i]->getType() == OPER_TYPE) {
                Oper *lexemoper = (Oper *)infixLines[row][i];
                if (lexemoper->getOpertype() == IF) {
                    stackIfElse.push((Goto *)lexemoper);
                } else if (lexemoper->getOpertype() == ELSE) {
                    stackIfElse.top()->setRow(row + 1);
                    stackIfElse.pop();
                    stackIfElse.push((Goto *)lexemoper);
                } else if (lexemoper->getOpertype() == ENDIF) {
                    stackIfElse.top()->setRow(row + 1);
                    stackIfElse.pop();
                }
            }
        }
    }
    std::stack <Goto *> stackWhile;
    for (int row = 0; row < (int)infixLines.size(); row++) {
        for (int i = 0; i < (int)infixLines[row].size(); i++) {
            if (infixLines[row][i] == nullptr) {
                continue;
            }
            if (infixLines[row][i]->getType() == OPER_TYPE) {
                Oper *lexemoper = (Oper *)infixLines[row][i];
                if (lexemoper->getOpertype() == WHILE) {
                    Goto *lexemgoto = (Goto *)lexemoper;
                    lexemgoto->setRow(row);
                    stackWhile.push(lexemgoto);
                } else if (lexemoper->getOpertype() == ENDWHILE) {
                    Goto *lexemgoto = (Goto *)lexemoper;
                    lexemgoto->setRow(stackWhile.top()->getRow());
                    stackWhile.top()->setRow(row + 1);
                    stackWhile.pop();
                }
            }
        }
    }
    std::stack <int> stackFunction;
    for (int row = 0; row < (int)infixLines.size(); row++) {
        for (int i = 0; i < (int)infixLines[row].size(); i++) {
            if (infixLines[row][i] == nullptr) {
                continue;
            } else if (infixLines[row][i]->getType() == OPER_TYPE) {
                if (infixLines[row][i]->getOpertype() == DEF && infixLines[row][i + 1]->getType() == VAR_TYPE) {
                    std::string FuncName = ((Variable*)infixLines[row][i + 1])->getName();
                    FunTable[FuncName] = row + 1;
                    delete infixLines[row][i];
                    delete infixLines[row][i + 1];
                    infixLines[row][i] = nullptr;
                    infixLines[row][i + 1] = nullptr;
                    i += 2;
                    stackFunction.push(row);
                } else if (infixLines[row][i]->getOpertype() == ENDDEF) {
                    delete infixLines[row][i];
                    infixLines[row][i] = nullptr;
                    if (!stackFunction.empty()) {
                        stackFunction.pop();
                    }
                }
            } else if (infixLines[row][i]->getType() == VAR_TYPE) {
                Variable *var = (Variable *)infixLines[row][i];
                if (FunTable.find(var->getName()) != FunTable.end()) {
                    Function *func = new Function(var->getName(), row);
                    delete infixLines[row][i];
                    infixLines[row][i] = func;
                }
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
        Oper *q = (Oper *)s; 
        if (s == nullptr) {
            continue;
        }
        if (s->getType() == NUM_TYPE) {
            postfix.push_back(s);
        } else if (s->getType() == FUNC_TYPE) {
            st.push(s);
        } else if (s->getType() == VAR_TYPE) {
            if (s->inLabTable() == true) {
                joinGotoAndLabel(s, postfix);
            } else {
                postfix.push_back(s);
            }
        } else if (q->getOpertype() == GOTO) {
            while(!st.empty()) {
                postfix.push_back(st.top());
                st.pop();
            }
            postfix.push_back(s);
            continue;
        } else if (q->getOpertype() == THEN) {
            continue;
        } else if (q->getOpertype() == ENDIF || q->getOpertype() == ENDDEF) {
            continue;
        } else if (q->getOpertype() == LBRACKET) {       
            st.push(s);                                      
        } else if (q->getOpertype() == RBRACKET) {  
            while (true) {
                if (st.top()->getOpertype() == LBRACKET)
                    break;
                postfix.push_back(st.top());
                st.pop();
            }
            if (!st.empty()) {
                st.pop();
            }
            if (!st.empty() && st.top()->getType() == FUNC_TYPE) {
                postfix.push_back(st.top());
                st.pop();
            }
            continue;
        } else if (!st.empty()) {
            while(!st.empty() && st.top()->getType() != FUNC_TYPE && st.top()->getPriority() >= q->getPriority()) {
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

int evaluatePostfix(std::vector<std::vector<Lexem *>> postfixLines, int row, int *answer) {
    std::vector<Lexem *> postfix = postfixLines[row];
    std::stack<Lexem *> st;
    Lexem *left_elem, *right_elem, *value = nullptr;
    for (auto s: postfix) {
        if (s == nullptr) {
            continue;
        }
        if (s->getType() == NUM_TYPE || s->getType() == VAR_TYPE) {
            st.push(s);
        } else if (s->getType() == FUNC_TYPE) {
            int LocRow = ((Function *)s)->getRow();
            int RetVal;
            while (postfix[LocRow] != nullptr) {
                LocRow = evaluatePostfix(postfixLines, LocRow, &RetVal);
            }
            value = new Number(RetVal);
            st.push(value);
        } else if (s->getType() == OPER_TYPE) {
            Oper *q = (Oper *)s;
            if (q->getOpertype() == GOTO || 
                q->getOpertype() == ELSE || 
                q->getOpertype() == ENDWHILE) {
                Goto *lexemgoto = (Goto *)q;
                return lexemgoto->getRow();
            } else if (q->getOpertype() == IF || q->getOpertype() == WHILE) {
                Goto *lexemgoto = (Goto *)q;
                Lexem *rvalue = st.top();
                st.pop();
                if (!(rvalue->getValue())) {
                    return lexemgoto->getRow();
                }
                continue;
            }
            right_elem = st.top();
            st.pop();
            left_elem = st.top();
            st.pop();
            value = calculate(q->getOpertype(), left_elem, right_elem);
            st.push(value);
        }
    }
    if (!st.empty()) {
        value = st.top();
        st.pop();
    }
    if (value != nullptr) {
        (*answer) = value->getValue();
    }
    row++;
    return row;
}
 
