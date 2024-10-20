#ifndef tokenize_hpp
#define tokenize_hpp
#include <iostream>
#include <vector>
using namespace std;


enum RegExSymbol {
    RE_CHAR, RE_LPAREN, RE_RPAREN, RE_LSQUARE, RE_RSQUARE, 
    RE_STAR, RE_PLUS, RE_QUESTION, RE_CONCAT, RE_OR, RE_SPECIFIEDSET, 
    RE_SPECIFIEDRANGE, RE_QUANTIFIER, RE_NONE
};

vector<string> reSymStr = { 
    "TK_CHAR", "TK_LPAREN", "TK_RPAREN", "RE_LSQUARE", "RE_RSQUARE", 
    "RE_STAR", "RE_PLUS", "RE_QUESTION", "RE_CONCAT", "RE_OR", "RE_SPECIFIEDSET", "RE_SPECIFIEDRANGE", "RE_QUANTIFIER", "TK_NONE"
};

struct Token {
    RegExSymbol symbol;
    string charachters;
    Token(RegExSymbol s = RE_NONE, string ch = "nil") : symbol(s), charachters(ch) { }

};

class Tokenizer {
    private:
        bool isChar(char c) {
            return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '.';
        }
        void setToken(Token& nt, char ch, RegExSymbol sym) {
            string buff;
            buff.push_back(ch);
            nt.symbol = sym;
            nt.charachters = buff;
        }
        void setSpecified(Token& nt, string re, int& idx) {
            string buff;
            idx++;
            bool isRange = false;
            while (idx < re.length() && re[idx] != ']') {
                if (re[idx] == '-') 
                    isRange = true;
                buff.push_back(re[idx++]);
            }
            nt.charachters = buff;
            nt.symbol = isRange ? RE_SPECIFIEDRANGE:RE_SPECIFIEDSET;
        }
        void setQuantifier(Token& nt, string re, int& idx) {
            string buff;
            cout<<re[idx]<<"?\n";
            while (idx < re.length() && isdigit(re[idx])) {
                buff.push_back(re[idx++]);
                cout<<re[idx]<<"?\n";
            }
            nt.charachters = buff;
            nt.symbol = RE_QUANTIFIER;
        }
    public:
        vector<Token> tokenize(string re) {
            vector<Token> tokens;
            for (int i = 0; i < re.size(); i++) {
                Token nt;
                if (isdigit(re[i]) || isChar(re[i])) {
                    setToken(nt, re[i], RE_CHAR);
                } else {
                    switch (re[i]) {
                        case '@': { setToken(nt, re[i], RE_CONCAT); } break;
                        case '|': { setToken(nt, re[i], RE_OR); } break;
                        case '+': { setToken(nt, re[i], RE_PLUS); } break;
                        case '*': { setToken(nt, re[i], RE_STAR); } break;
                        case '?': { setToken(nt, re[i], RE_QUESTION); } break;
                        case '(': { setToken(nt, re[i], RE_LPAREN); } break;
                        case ')': { setToken(nt, re[i], RE_RPAREN); } break;
                        case '[': { setSpecified(nt, re, i); } break;
                        case ']': break;
                        case '{': { i++; setQuantifier(nt, re, i); } break;
                        case '}': break;
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