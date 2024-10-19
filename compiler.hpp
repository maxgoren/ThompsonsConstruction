#ifndef compiler_hpp
#define compiler_hpp
#include <iostream>
#include "parser.hpp"
#include "nfa.hpp"
using namespace std;

class NFACompiler {
    private:
        int l;
        Stack<NFA> nfaStack;
        int makeStateLabel() {
            return l++;
        }
        void initNextNFA(NFA& nnfa) {
            int nstart = makeStateLabel();
            nnfa.makeState(nstart);
            nnfa.setStart(nstart);
            int nend = makeStateLabel();
            nnfa.makeState(nend);
            nnfa.setAccept(nend);
        }
        void copyTransitions(NFA& nnfa, NFA onfa) {
            for (auto state : onfa.getStates()) {
                nnfa.makeState(state.first);
                for (auto trans : onfa.getTransitions(state.first)) {
                    nnfa.addTransition(trans);
                }
            }
        }
        NFA singleTransitionNFA(Edge* edge) {
            NFA nfa;
            initNextNFA(nfa);
            nfa.addTransition(Transition(nfa.getStart(), nfa.getAccept(), edge)); 
            return nfa;
        }

        NFA emptyExpr() {
            return singleTransitionNFA(new EpsilonEdge());
        }
        NFA atomicNFA(Token c) {
            return singleTransitionNFA(new CharEdge(c));
        }
        NFA concatNFA(NFA first, NFA second) {
            NFA nnfa;
            copyTransitions(nnfa, first);
            copyTransitions(nnfa, second);
            nnfa.setStart(first.getStart());
            nnfa.setAccept(second.getAccept());
            nnfa.addTransition(Transition(first.getAccept(), second.getStart(), new EpsilonEdge()));
            return nnfa;
        }
        NFA kleeneNFA(NFA torepeat, bool mustMatch) {
            NFA nnfa;
            initNextNFA(nnfa);
            copyTransitions(nnfa, torepeat);
            nnfa.addTransition(Transition(torepeat.getAccept(), nnfa.getStart(), new EpsilonEdge()));
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
            copyTransitions(nnfa, first);
            copyTransitions(nnfa, second);
            //Add new E-transitions from new start state to first and second NFAs
            nnfa.addTransition(Transition(nnfa.getStart(), first.getStart(), new EpsilonEdge()));
            nnfa.addTransition(Transition(nnfa.getStart(), second.getStart(), new EpsilonEdge()));
            //Add new E-transitions from first and second accept state to new accept state.
            nnfa.addTransition(Transition(first.getAccept(), nnfa.getAccept(), new EpsilonEdge()));
            nnfa.addTransition(Transition(second.getAccept(), nnfa.getAccept(), new EpsilonEdge()));
            return nnfa;
        }
        NFA zeroOrOnce(NFA onfa) {
            NFA nnfa;
            initNextNFA(nnfa);
            copyTransitions(nnfa, onfa);
            //wire in match choice
            nnfa.addTransition(Transition(nnfa.getStart(), onfa.getStart(), new EpsilonEdge()));
            nnfa.addTransition(Transition(onfa.getAccept(), nnfa.getAccept(), new EpsilonEdge()));
            //wire in epsilon choice.
            nnfa.addTransition(Transition(nnfa.getStart(), nnfa.getAccept(), new EpsilonEdge()));
            return nnfa;
        }
        NFA buildOperatorNFA(RegularExpression* ast) {
            switch (ast->getSymbol().charachters[0]) {
                case '@': {
                    NFA b = nfaStack.pop();
                    NFA a = nfaStack.pop();
                    return concatNFA(a, b);
                }
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
                case '?': {
                    NFA a = nfaStack.pop();
                     return zeroOrOnce(a);
                }
                default:
                    break;
            }
            return NFA();
        }
        void gen_nfa(RegularExpression* ast) {
            if (ast != nullptr) {
                gen_nfa(ast->getLeft());
                gen_nfa(ast->getRight());
                if (!isOp(ast->getSymbol())) {
                    nfaStack.push(atomicNFA(ast->getSymbol()));
                } else {
                    nfaStack.push(buildOperatorNFA(ast));
                }
            }
        }
        
    public:
        NFACompiler() {
            l = 0;
        }
        NFA compile(string pattern) {
            Parser parser;
            string re = "(" + pattern + ")";
            auto ast = parser.parse(re);
            traverse(ast, 1);
            gen_nfa(ast);
            return nfaStack.pop();
        }
};

#endif