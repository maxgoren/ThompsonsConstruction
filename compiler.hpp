#ifndef compiler_hpp
#define compiler_hpp
#include <iostream>
#include "shuntingyard.hpp"
#include "parser.hpp"
#include "nfa.hpp"
using namespace std;


class NFACompiler {
    private:
        Stack<NFA> nfaStack;
        int l;
        int nextlabel() {
            return l++;
        }
        void initNextNFA(NFA& nnfa) {
            int nstart = nextlabel();
            nnfa.makeState(nstart);
            nnfa.setStart(nstart);
            int nend = nextlabel();
            nnfa.makeState(nend);
            nnfa.setAccept(nend);
        }
        void moveTransitions(NFA& nnfa, NFA onfa) {
            for (auto state : onfa.getStates()) {
                nnfa.makeState(state.first);
                for (auto trans : onfa.getTransitions(state.first)) {
                    nnfa.addTransition(trans);
                }
            }
        }
        NFA singleCharExpr(Edge* edge) {
            NFA nfa;
            int a = nextlabel();
            int b = nextlabel();
            nfa.makeState(a);
            nfa.makeState(b);
            nfa.setStart(a);
            nfa.setAccept(b);
            nfa.addTransition(Transition(a,b, edge));
            return nfa;
        }
        NFA emptyExpr() {
            return singleCharExpr(new EpsilonEdge());
        }
        NFA atomicNFA(char c) {
            return singleCharExpr(new CharEdge(c));
        }
        NFA concatNFA(NFA first, NFA second) {
            NFA nnfa;
            moveTransitions(nnfa, first);
            moveTransitions(nnfa, second);
            nnfa.setStart(first.getStart());
            nnfa.setAccept(second.getAccept());
            nnfa.addTransition(Transition(first.getAccept(), second.getStart(), new EpsilonEdge()));
            return nnfa;
        }
        NFA kleeneNFA(NFA torepeat, bool mustMatch) {
            NFA nnfa;
            initNextNFA(nnfa);
            torepeat.addTransition(Transition(torepeat.getAccept(), torepeat.getStart(), new EpsilonEdge()));
            moveTransitions(nnfa, torepeat);
            nnfa.addTransition(Transition(nnfa.getStart(), torepeat.getStart(), new EpsilonEdge()));
            nnfa.addTransition(Transition(torepeat.getAccept(), nnfa.getAccept(), new EpsilonEdge()));
            if (!mustMatch)
                nnfa.addTransition(Transition(nnfa.getStart(), nnfa.getAccept(), new EpsilonEdge()));
            return nnfa;
        }
        NFA alternateNFA(NFA first, NFA second) {
            //create new NFA with start and end state.
            NFA nnfa;
            initNextNFA(nnfa);
            //copy states and transitions from both NFA's to new NFA
            moveTransitions(nnfa, first);
            moveTransitions(nnfa, second);
            //Add new E-transitions from new start state to first and second NFAs
            nnfa.addTransition(Transition(nnfa.getStart(), first.getStart(), new EpsilonEdge()));
            nnfa.addTransition(Transition(nnfa.getStart(), second.getStart(), new EpsilonEdge()));
            //Add new E-transitions from first and second accept state to new accept state.
            nnfa.addTransition(Transition(first.getAccept(), nnfa.getAccept(), new EpsilonEdge()));
            nnfa.addTransition(Transition(second.getAccept(), nnfa.getAccept(), new EpsilonEdge()));
            return nnfa;
        }
        NFA buildOperatorNFA(astnode* ast) {
            switch (ast->symbol) {
                case '@': {
                    NFA b = nfaStack.pop();
                    NFA a = nfaStack.pop();
                    return concatNFA(a, b);
                }
                break;
                case '|': {
                    NFA b = nfaStack.pop();
                    NFA a = nfaStack.pop();
                    return alternateNFA(a, b);
                }
                case '*': {
                    NFA a = nfaStack.pop();
                    return kleeneNFA(a, false);
                }
                case '+': {
                    NFA a = nfaStack.pop();
                    return kleeneNFA(a, true);
                }
                break;
                default:
                    break;
            }
            return NFA();
        }
        void gen_nfa(astnode* ast) {
            int l;
            if (ast != nullptr) {
                gen_nfa(ast->left);
                gen_nfa(ast->right);
                NFA nfa;
                if (ast->type == charnode) {
                    nfa = atomicNFA(ast->symbol);
                } else {
                    nfa = buildOperatorNFA(ast);
                }
                nfaStack.push(nfa);
            }
        }
    public:
        NFACompiler() {
            l = 0;
        }
        NFA compile(string pattern) {
            Parser parser;
            string re = "(" + pattern + ")";
            astnode* ast = parser.parse(re);
            traverse(ast);
            gen_nfa(ast);
            return nfaStack.pop();
        }
};

#endif