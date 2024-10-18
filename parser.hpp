#ifndef parser_hpp
#define parser_hpp
#include <iostream>
#include "tokenize.hpp"
#include "stack.hpp"
using namespace std;

class RegularExpression {
    public:
        virtual Token getSymbol() = 0;
        virtual RegularExpression* getLeft() = 0;
        virtual RegularExpression* getRight() = 0;
}; 

class ExpressionLiteral : public RegularExpression {
    private:
        Token symbol;
    public:
        ExpressionLiteral(Token sym) { symbol = sym; }
        RegularExpression* getLeft() {
            return nullptr;
        }
        RegularExpression* getRight() {
            return nullptr;
        }
        Token getSymbol() {
            return symbol;
        }
};

class ExpressionOperator : public RegularExpression {
    private:
        RegularExpression* left;
        RegularExpression* right;
        Token symbol;
    public:
        ExpressionOperator(Token c, RegularExpression* ll, RegularExpression* rr) {
            symbol = c;
            left = ll;
            right = rr;
        }
        Token getSymbol() {
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
        cout<<h->getSymbol().charachters<<endl;
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

bool isOp(Token c) {
    switch (c.symbol) {
            case TK_STAR:
        case TK_PLUS:
        case TK_QUESTION: 
        case TK_CONCAT:
        case TK_OR: return true;
        default:
            break;
    }
    return false;
}

class Parser {
    private:
        RegularExpression* makeTree(vector<Token> postfix) {
            Stack<RegularExpression*> sf;
            for (Token c : postfix) {
                cout<<"Processing: "<<c.charachters<<" ";
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
        int precedence(Token c) {
            switch (c.symbol) {
                case TK_STAR:
                case TK_PLUS:
                case TK_QUESTION: return 50;
                case TK_CONCAT: return 30;
                case TK_OR: return 20;
                default:
                    break;
            }
            return 10;
        }
        bool leftAssociative(Token c) {
            switch (c.symbol) {
                 case TK_STAR:
                case TK_PLUS:
                case TK_QUESTION: 
                case TK_CONCAT:
                case TK_OR: return true;
                default:
                    break;
            }
            return false;
        }
        string addConcatOp(string str) {
            string fixed;
            bool inset = false;
            for (int i = 0; i < str.length(); i++) {
                fixed.push_back(str[i]);
                if (str[i] == '(' || str[i] == '|')
                    continue;
                if (str[i] == '[') {
                    inset = true;
                    continue;
                }
                if (str[i] == ']') {
                    inset = false;
                }
                if (i+1 < str.length() && inset == false) {
                    char p = str[i+1];
                    if (p == '|' || p == '*' || p == '+' || p == ')' || p == '?')
                        continue;
                    fixed.push_back('@');
                }
            }
            return fixed;
        }
        vector<Token> in2post(vector<Token> str) {
            Stack<Token> ops;
            vector<Token> postfix;
            for (int i = 0; i < str.size(); i++) {
                if (str[i].symbol == TK_LPAREN) {
                    ops.push(str[i]);
                } else if (isOp(str[i])) {
                        if (precedence(str[i]) < precedence(ops.top()) || (precedence(str[i]) == precedence(ops.top()) && leftAssociative(str[i]))) {
                            Token c = ops.pop();
                            postfix.push_back(c);
                            ops.push(str[i]);
                        } else {
                            ops.push(str[i]);
                        }
                } else if (str[i].symbol == TK_RPAREN) {
                    while (!ops.empty()) {
                        Token c = ops.pop();
                        if (c.symbol == TK_LPAREN)
                            break;
                        else postfix.push_back(c);
                    }
                } else {
                    postfix.push_back(str[i]);
                }
            }
            while (!ops.empty()) {
                Token c = ops.pop();
                if (c.symbol != TK_LPAREN)
                    postfix.push_back(c);
            }
            return postfix;
        }
    public:
        Parser() {

        }
        RegularExpression* parse(string regexp) {
            Tokenizer tz;
            cout<<"Parsing: "<<regexp<<endl;
            regexp = addConcatOp(regexp);
            auto tokens =  tz.tokeize(regexp);
            int i = 0;
            for (auto m : tokens) {
                cout<<i++<<": "<<m.charachters<<endl;
            }
            vector<Token> postfix = in2post(tokens);
            cout<<"Postfix: ";
            for (auto m : postfix) {
                cout<<m.charachters<<" - "<<symStr[m.symbol]<<endl;
            }
            cout<<endl;
            return makeTree(postfix);
        }
};

#endif