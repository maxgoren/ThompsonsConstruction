#ifndef parser_hpp
#define parser_hpp
#include <iostream>
#include "shuntingyard.hpp"
#include "stack.hpp"
using namespace std;

enum nodetype {
    opnode, charnode
};

struct astnode {
    nodetype type;
    char symbol;
    astnode* left;
    astnode* right;
    astnode(nodetype t, char s, astnode* ll, astnode* rr) : type(t), symbol(s), left(ll), right(rr) { }
};

int d = 0;
void traverse(astnode* h) {
    ++d;
    if (h != nullptr) {
        traverse(h->left);
        for (int i = 0; i < d; i++) cout<<"  ";
        cout<<h->symbol<<endl;
        traverse(h->right);
    }
    --d;
}

class Parser {
    private:
        bool isOp(char c) {
            switch (c) {
                case '|':
                case '*':
                case '+':
                case '?':
                case '@':
                    return true;
                default:
                    break;
            }
            return false;
        }
        astnode* makeTree(string postfix) {
            Stack<astnode*> sf;
            for (char c : postfix) {
                cout<<"Processing: "<<c<<" ";
                if (!isOp(c)) {
                    cout<<"Alphabet."<<endl;
                    sf.push(new astnode(charnode, c, nullptr, nullptr));
                } else {
                    cout<<"Operator."<<endl;
                    astnode* right = sf.empty() ? nullptr:sf.pop();
                    astnode* left = sf.empty() ? nullptr:sf.pop();
                    sf.push(new astnode(opnode, c, left, right));
                }
            }
            return sf.pop();
        }
        Shuntingyard sy;
    public:
        Parser() {

        }
        astnode* parse(string regexp) {
            cout<<"Parsing: "<<regexp<<endl;
            string postfix = sy.in2post(regexp);
            cout<<"Postfix: "<<regexp<<endl;
            return makeTree(postfix);
        }
};

#endif