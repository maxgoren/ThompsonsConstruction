#ifndef parser_hpp
#define parser_hpp
#include <iostream>
#include "stack.hpp"
using namespace std;

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

bool isOp(char c) {
    switch (c) {
        case '|': case '@': case '?': case '+': case '*':
            return true;
        default:
            break;
    }
    return false;
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
        int precedence(char c) {
            switch (c) {
                case '*':
                case '+':
                case '?': return 50;
                case '@': return 30;
                case '|': return 20;
                default:
                    break;
            }
            return 10;
        }
        bool leftAssociative(char c) {
            switch (c) {
                case '*': 
                case '+':
                case '?':
                case '|': 
                case '@':
                    return true;
                default:
                    break;
            }
            return false;
        }
        string addConcatOp(string str) {
            string fixed;
            for (int i = 0; i < str.length(); i++) {
                fixed.push_back(str[i]);
                if (str[i] == '(' || str[i] == '|')
                    continue;
                if (i+1 < str.length()) {
                    char p = str[i+1];
                    if (p == '|' || p == '*' || p == '+' || p == ')' || p == '?')
                        continue;
                    fixed.push_back('@');
                }
            }
            return fixed;
        }
        string in2post(string& str) {
            Stack<char> ops;
            string postfix;
            str = addConcatOp(str);
            cout<<"Inserting explicit concat operators: "<<str<<endl;
            for (int i = 0; i < str.length(); i++) {
                if (str[i] == '(') {
                    ops.push(str[i]);
                } else if (isOp(str[i])) {
                        if (precedence(str[i]) < precedence(ops.top()) || (precedence(str[i]) == precedence(ops.top()) && leftAssociative(str[i]))) {
                            char c = ops.pop();
                            postfix.push_back(c);
                            ops.push(str[i]);
                        } else {
                            ops.push(str[i]);
                        }
                } else if (str[i] == ')') {
                    while (!ops.empty()) {
                        char c = ops.pop();
                        if (c == '(')
                            break;
                        else postfix.push_back(c);
                    }
                } else {
                    postfix.push_back(str[i]);
                }
            }
            while (!ops.empty()) {
                char c = ops.pop();
                if (c != '(')
                    postfix.push_back(c);
            }
            return postfix;
        }
    public:
        Parser() {

        }
        RegularExpression* parse(string regexp) {
            cout<<"Parsing: "<<regexp<<endl;
            string postfix = in2post(regexp);
            cout<<"Postfix: "<<postfix<<endl;
            return makeTree(postfix);
        }
};

#endif