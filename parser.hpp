#ifndef parser_hpp
#define parser_hpp
#include <iostream>
#include "shuntingyard.hpp"
#include "stack.hpp"
using namespace std;

bool isOp(char c) {
    switch (c) {
        case '|':
        case '*':
        case '?':
        case '+':
        case '@':
            return true;
        default:
            break;
    }
    return false;
}


class RegularExpression {
    public:
        virtual char getSymbol() = 0;
        virtual RegularExpression* getLeft() = 0;
        virtual RegularExpression* getRight() = 0;
}; 

class ExpressionLiteral : public RegularExpression {
    private:
        char symbol;
    public:
        ExpressionLiteral(char sym) { symbol = sym; }
        RegularExpression* getLeft() {
            return nullptr;
        }
        RegularExpression* getRight() {
            return nullptr;
        }
        char getSymbol() {
            return symbol;
        }
};

class ExpressionOperator : public RegularExpression {
    private:
        RegularExpression* left;
        RegularExpression* right;
        char symbol;
    public:
        ExpressionOperator(char c, RegularExpression* ll, RegularExpression* rr) {
            symbol = c;
            left = ll;
            right = rr;
        }
        char getSymbol() {
            return symbol;
        }
        RegularExpression* getLeft() {
            return left;
        }
        RegularExpression* getRight() {
            return right;
        }
};

void traverse(RegularExpression* h, int d) {
    if (h != nullptr) {
        traverse(h->getLeft(), d+1);
        for (int i = 0; i < d; i++) cout<<"  ";
        cout<<h->getSymbol()<<endl;
        traverse(h->getRight(), d+1);
    }
}

class Parser {
    private:
        RegularExpression* makeTree(string postfix) {
            Stack<RegularExpression*> sf;
            for (char c : postfix) {
                cout<<"Processing: "<<c<<" ";
                if (!isOp(c)) {
                    cout<<"Alphabet."<<endl;
                    sf.push(new ExpressionLiteral(c));
                } else {
                    cout<<"Operator."<<endl;
                    auto right = sf.empty() ? nullptr:sf.pop();
                    auto left = sf.empty() ? nullptr:sf.pop();
                    sf.push(new ExpressionOperator(c, left, right));
                }
            }
            return sf.pop();
        }
        Shuntingyard sy;
    public:
        Parser() {

        }
        RegularExpression* parse(string regexp) {
            cout<<"Parsing: "<<regexp<<endl;
            string postfix = sy.in2post(regexp);
            cout<<"Postfix: "<<regexp<<endl;
            return makeTree(postfix);
        }
};

#endif