#ifndef nfa_hpp
#define nfa_hpp
#include <iostream>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "stack.hpp"
using namespace std;

class Edge {
    public:
        virtual char getLabel() = 0;
        virtual bool matches(char c) = 0;
        virtual bool isEpsilon() = 0;
        virtual ~Edge() { }
};

class CharEdge : public Edge {
    private:
        char label;
    public:
        CharEdge(char c) {
            label = c;
        }
        ~CharEdge() {

        }
        bool isEpsilon() {
            return false;
        }
        bool matches(char c) {
            return label == c;
        }
        char getLabel() {
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
        char getLabel() {
            return '&';
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
            tmp.push_back(t.edge->getLabel());
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
        bool match(string text) {
            cout<<"Attempting to match: "<<text<<endl;
            cout<<"Start state: "<<start<<endl;
            cout<<"Accept state: "<<accept<<endl;
            unordered_set<Transition> epsHistory;
            Stack<Node> sf;
            sf.push(Node(0, start, epsHistory));
            while (!sf.empty()) {
                int strPos = sf.top().strPos;
                State currState = sf.top().state;
                epsHistory = sf.top().epsHistory;
                sf.pop();
                char input = text[strPos];
                cout<<"State: "<<currState<<", Input: "<<input<<endl;
                if (accept == currState) {
                    cout<<"Found Accept State."<<endl;
                    return true;
                }

                for (Transition t : states[currState]) {
                    cout<<"    "<<t.from<<" -> "<<t.to;
                    if ((t.edge->matches(input) || t.edge->matches('.')) || t.edge->isEpsilon()) {
                        if (t.edge->isEpsilon()) { 
                            if (epsHistory.find(t) != epsHistory.end()) {
                                cout<<"\nAlready on Stack.\n"<<endl;
                                continue;
                            }
                            cout<<"\n     + Taking Epsilon transition."<<endl;
                            epsHistory.insert(t);
                            sf.push(Node(strPos, t.to, epsHistory));
                        } else {
                            cout<<"\n     + Match on " <<t.edge->getLabel()<<endl;
                            epsHistory.clear();
                            sf.push(Node(strPos + 1, t.to, epsHistory));
                        }
                    } else {
                        cout<<", Nothing for us here, mate."<<endl;
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
};


#endif