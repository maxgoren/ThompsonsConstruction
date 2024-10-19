#ifndef nfa_hpp
#define nfa_hpp
#include <iostream>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "stack.hpp"
#include "tokenize.hpp"
using namespace std;

class Edge {
    public:
        virtual Token getLabel() = 0;
        virtual bool matches(char c) = 0;
        virtual bool isEpsilon() = 0;
        virtual ~Edge() { }
};

class CharEdge : public Edge {
    private:
        Token label;
    public:
        CharEdge(Token c) {
            label = c;
        }
        ~CharEdge() {

        }
        bool isEpsilon() {
            return false;
        }
        bool matches(char c) {
            if (label.symbol == TK_SPECIFIEDSET) {
                for (char m : label.charachters) {
                    if (c == m)
                        return true;
                }
                return false;
            }
            return label.charachters[0] == c;
        }
        Token getLabel() {
            return label;
        }
};

class EpsilonEdge : public Edge {
    public:
        EpsilonEdge() { }
        ~EpsilonEdge() { }
        bool matches(char c) {
            return true;
        }
        bool isEpsilon() {
            return true;
        }
        Token getLabel() {
            return Token(TK_NONE, "&");
        }
};


typedef int State;

struct Transition {
    State from;
    State to;
    Edge* edge;
    Transition(State s, State t, Edge* e) {
        from = s; to = t; edge = e;
    }
    Transition(const Transition& t) {
        from = t.from;
        to = t.to;
        if (t.edge->isEpsilon()) {
            edge = new EpsilonEdge();
        } else {
            edge = new CharEdge(t.edge->getLabel());
        }
    }
    ~Transition() {
        delete edge;
    }
    Transition& operator=(const Transition& t) {
        if (this != &t) {
            from = t.from;
            to = t.to;
            if (t.edge->isEpsilon()) {
                edge = new EpsilonEdge();
            } else {
                edge = new CharEdge(t.edge->getLabel());
            }
        }
        return *this;
    }
};

bool operator==(const Transition& s, const Transition& t) {
    return s.from == t.from && s.to == t.to && s.edge == t.edge;
}

bool operator!=(const Transition& s, const Transition& t) {
    return !(s == t);
}

namespace std {
    template <> struct hash<Transition> {
        std::size_t operator()(const Transition& t) const noexcept {
            string tmp = to_string(t.from);
            tmp += t.edge->getLabel().charachters;
            tmp += to_string(t.to);
            return std::hash<string>()(tmp);
        }
    };
}

struct Node {
    int strPos;
    State state;
    unordered_set<Transition> epsHistory;
    Node(int sp, State s, unordered_set<Transition> t) : strPos(sp), state(s), epsHistory(t) { }
};


class NFA {
    private:
        State start;
        State accept;
        unordered_map<State, vector<Transition>> states;
    public:
        NFA() {
            start = 0;
            accept = 0;
        }
        NFA(const NFA& nfa) {
            start = nfa.start;
            accept = nfa.accept;
            for (auto m : nfa.states) {
                makeState(m.first);
                for (auto t : m.second) {
                    addTransition(t);
                }
            }
        }
        void makeState(State name) {
            if (states.find(name) == states.end()) {
                states.insert(make_pair(name, vector<Transition>()));
            }
        }
        void setStart(State ss) {
            start = ss;
        }
        void setAccept(State as) {
            accept = as;
        }
        State getStart() {
            return start;
        }
        State getAccept() {
            return accept;
        }
        void addTransition(Transition t) {
            states[t.from].push_back(t);
        }
        int size() {
            return states.size();
        }
        unordered_map<State, vector<Transition>>& getStates() {
            return states;
        }
        vector<Transition>& getTransitions(State state) {
            return states[state];
        }
        bool grep(string text) {
            bool found = false;
            for (int i = 0; i < text.length(); i++) {
                if (match(text.substr(i))) {
                    cout<<"Match starting from: "<<i<<endl;
                    found = true;
                }
            }
            return found;
        }
        bool matchbt(string text) {
            cout<<"Attempting to match: "<<text<<", Start state: "<<start<<", Accept state: "<<accept<<endl;
            unordered_set<Transition> epsHistory;
            Stack<Node> sf;
            sf.push(Node(0, start, epsHistory));
            int from = 0;
            while (!sf.empty()) {
                int strPos = sf.top().strPos;
                State currState = sf.top().state;
                epsHistory = sf.top().epsHistory;
                sf.pop();
                char input = text[strPos];
                cout<<"State: "<<currState<<", Input: "<<input<<endl;
                if (accept == currState) { 
                    cout<<"Found Accept State."<<endl;
                    cout<<text.substr(from, text.length()-strPos);
                    return true;
                }
                for (auto it = states[currState].rbegin(); it != states[currState].rend(); it++) {
                    Transition t = *it;
                    if ((t.edge->matches(input) || t.edge->matches('.')) || t.edge->isEpsilon()) {
                        if (t.edge->isEpsilon()) { 
                            if (epsHistory.find(t) != epsHistory.end()) {
                                cout<<"\nAlready on Stack.\n"<<endl;
                                continue;
                            }
                            epsHistory.insert(t);
                            sf.push(Node(strPos, t.to, epsHistory));
                        } else {
                            epsHistory.clear();
                            sf.push(Node(strPos + 1, t.to, epsHistory));
                        }
                        cout<<t.from<<"-("<<t.edge->getLabel().charachters<<")->"<<t.to<<endl;
                    } else {
                        cout<<"Dead end."<<endl;
                    }
                    cout<<endl;
                }
            }
            return false;
        }
        NFA& operator=(const NFA& nfa) {
            if (this != &nfa) {
                start = nfa.start;
                accept = nfa.accept;
                for (auto m : nfa.states) {
                    makeState(m.first);
                    for (auto t : m.second) {
                        addTransition(t);
                    }
                }
            }
            return *this;
        }
        //Gathers a list of states reachable from those in clist, which have transition which consumes ch
        set<State> move(set<State> clist, char ch) {
            set<State> nlist;
            cout<<ch<<": "<<endl;
            for (State s : clist) {
                for (Transition t : states[s]) {
                    if (t.edge->matches(ch) || t.edge->matches('.')) {
                        if (t.edge->isEpsilon() == false) {
                            cout<<'\t'<<t.from<<" - ("<<t.edge->getLabel().charachters<<") ->"<<t.to<<endl;
                            nlist.insert(t.to);
                        } 
                    }
                }
            }
            return nlist;
        }
        //Gathers a list of states reachable from those in clist using _only_ epsilon transitions.
        set<State> e_closure(set<State> clist) {
            set<State> nlist = clist;
            Stack<State> sf;
            for (State s : clist)
                sf.push(s);
            while (!sf.empty()) {
                State s = sf.pop();
                for (Transition t : states[s]) {
                    if (t.edge->isEpsilon()) {
                        if (nlist.find(t.to) == nlist.end()) {
                            cout<<'\t'<<t.from<<" - ("<<t.edge->getLabel().charachters<<") ->"<<t.to<<endl;
                            nlist.insert(t.to);
                            sf.push(t.to);
                        }
                    }
                }
            }
            return nlist;
        }
        bool match(string text) {
            set<State> curr, next;
            next.insert(start);
            curr = e_closure(next);
            for (int i = 0; i < text.length(); i++) {
                next = move(curr, text[i]);
                curr = e_closure(next);
            }
            return curr.find(accept) != curr.end();
        }
};


#endif