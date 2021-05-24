#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>

enum LexemType {
    NUM_TYPE,
    OPER_TYPE,
    VAR_TYPE,
    FUNC_TYPE
};

enum OPERATOR {
    DEF, RET, ENDDEF,
    IF, THEN,
    ELSE, ENDIF,
    WHILE, ENDWHILE,
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

         //********************
class Lexem {
    LexemType type;
public:
    Lexem();
    virtual ~Lexem() {};
    virtual void print() {};
    virtual int getType() {}; 
    virtual int getValue() {};
    virtual void setValue(int newValue) {};
    virtual bool inLabTable() {};
    virtual std::string getName() {};
    virtual OPERATOR getOpertype() {};
    virtual int getPriority() {};
};

        //*********************
class Number: public Lexem {
    int value;
public:
    Number();
    ~Number();
    Number(int newValue);
    int getValue();
    int getType();
    void print();
};

        //*********************
class Oper: public Lexem {
    OPERATOR opertype;
public:
    Oper();
    ~Oper();
    Oper(int n);
    int getPriority();
    int getType();
    OPERATOR getOpertype();
    void print();
};

                //********************
class Variable: public Lexem {
    int value;
    std::string name;
public:
    Variable();
    Variable(std::string newName);
    void setValue(int newValue);     
    int getValue();
    std::string getName();
    void print();
    int getType();
    bool inLabTable();
};

                //********************
class Goto: public Oper {
    int row;
    int oper;
public:
    enum { UNDEFINED = -INT32_MAX };
    Goto();
    Goto(int newOper): Oper(newOper) {
        row = UNDEFINED;
        Goto::oper == newOper;
    }
    void setRow(int newRow);
    void setRow(const std::string &labelname);
    int getRow();
    void print();
};

            //********************
class Function: public Goto {
    std::string name;
public:
    Function();
    Function(std::string newName, int newRow);
    int getType();
    void setName(std::string newName);
    std::string getName();
    void setRow(int newRow);
    int getRow();
    void print();
};
            //********************
Lexem *getNumber(std::string codeline, int *i);

Lexem *getVariable(std::string codeline, int *i);

Lexem *getOperation(std::string codeline, int *index);

void printVector(std::vector<Lexem*> &vector);

void print(std::vector<std::vector<Lexem *>> lines);

void printMap();

std::vector<Lexem *> parseLexem(std::string codeline);

void initLabels(std::vector<Lexem *> &infix, int row);

void initJumps(std::vector<std::vector <Lexem *>> &infixLines);

void joinGotoAndLabel(Lexem *lexemvar, std::vector<Lexem *> &postfix);

std::vector<Lexem *> buildPostfix(std::vector<Lexem *> infix);

Lexem *calculate(int opertype, Lexem *left, Lexem *right);

int evaluatePostfix(std::vector<std::vector<Lexem *>> postfixLines, int row, int *answer);

#endif
