#ifndef shuntingyard_hpp
#define shuntingyard_hpp
#include <iostream>
#include <vector>
#include "stack.hpp"
using namespace std;

enum Symbol {
    TK_LITERAL, TK_LPAREN, TK_RPAREN, TK_OR, TK_STAR, TK_CAT, TK_NONE
};

struct Token {
    char glif;
    Symbol symbol;
    Token(char c = ' ', Symbol s = TK_NONE) {
        glif = c;
        symbol = s;
    }
};

class Shuntingyard {
    private:
        Stack<char> ops;
        string postfix;
        int precedence(char c) {
            switch (c) {
                case '*': return 50;
                case '+': return 50;
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
        vector<Token> tokenize(string s) {
            vector<Token> tokens;
            return tokens;
        }
    public:
        Shuntingyard() {

        }
        string in2post(string& str) {
            str = addConcatOp(str);
            cout<<"Inserting explicit concat operators: "<<str<<endl;
            vector<Token> tokenized = tokenize(str);
            postfix.clear();
            for (int i = 0; i < str.length(); i++) {
                if (str[i] == '(') {
                    ops.push(str[i]);
                } else if (str[i] == '|' || str[i] == '*' || str[i] == '+' || str[i] == '@' || str[i] == '?') {
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
};


#endif