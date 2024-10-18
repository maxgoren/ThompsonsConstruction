#ifndef tokenize_hpp
#define tokenize_hpp
#include <iostream>
#include <vector>
using namespace std;


enum Symbol {
    TK_CHAR, TK_LPAREN, TK_RPAREN, TK_LSQUARE, TK_RSQUARE, 
    TK_STAR, TK_PLUS, TK_QUESTION, TK_CONCAT, TK_OR, TK_SPECIFIEDSET, TK_NONE
};

vector<string> symStr = { 
    "TK_CHAR", "TK_LPAREN", "TK_RPAREN", "TK_LSQUARE", "TK_RSQUARE", 
    "TK_STAR", "TK_PLUS", "TK_QUESTION", "TK_CONCAT", "TK_OR", "TK_SPECIFIEDSET", "TK_NONE"
};

struct Token {
    Symbol symbol;
    string charachters;
    Token(Symbol s = TK_NONE, string ch = "nil") : symbol(s), charachters(ch) { }

};

class Tokenizer {
    private:
        bool isChar(char c) {
            return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
        }
    public:
        Tokenizer() {

        }
        void setToken(Token& nt, char ch, Symbol sym) {
            string buff;
            buff.push_back(ch);
            nt.symbol = sym;
            nt.charachters = buff;
        }
        void setSpecified(Token& nt, string re, int& idx) {
            string buff;
            idx++;
            while (idx < re.length() && re[idx] != ']')
                buff.push_back(re[idx++]);
            nt.charachters = buff;
            nt.symbol = TK_SPECIFIEDSET;
        }
        vector<Token> tokeize(string re) {
            vector<Token> tokens;
            for (int i = 0; i < re.size(); i++) {
                Token nt;
                if (isdigit(re[i]) || isChar(re[i])) {
                    setToken(nt, re[i], TK_CHAR);
                } else {
                    switch (re[i]) {
                        case '@': { setToken(nt, re[i], TK_CONCAT); } break;
                        case '|': { setToken(nt, re[i], TK_OR); } break;
                        case '+': { setToken(nt, re[i], TK_PLUS); } break;
                        case '*': { setToken(nt, re[i], TK_STAR); } break;
                        case '?': { setToken(nt, re[i], TK_QUESTION); } break;
                        case '(': { setToken(nt, re[i], TK_LPAREN); } break;
                        case ')': { setToken(nt, re[i], TK_RPAREN); } break;
                        case '[': { setSpecified(nt, re, i); } break;
                        case ']': { } break;
                        default:
                            break;
                    }
                }
                tokens.push_back(nt);
            }
            return tokens;
        }
};

#endif