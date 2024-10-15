#ifndef nfa_hpp
#define nfa_hpp
#include <iostream>
#include <unordered_map>
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
        unordered_map<State, vector<Transition>> getStates() {
            return states;
        }
        vector<Transition>& getTransitions(State state) {
            return states[state];
        }
        bool match(string text) {
            cout<<"Attempting to match: "<<text<<endl;
            cout<<"Start state: "<<start<<endl;
            cout<<"Accept state: "<<accept<<endl;
            Stack<pair<int,State>> sf;
            sf.push(make_pair(0, start));
            while (!sf.empty()) {
                int strPos = sf.top().first;
                State currState = sf.top().second;
                sf.pop();
                cout<<"State: "<<currState<<", Character: ";
                char cm = text[strPos];
                cout<<cm<<endl;
                if (accept == currState) {
                    cout<<"Found Accept State."<<endl;
                    return true;
                }
                for (Transition t : states[currState]) {
                    cout<<"    "<<t.from<<" -> "<<t.to<<endl;
                    if ((t.edge->matches(cm) || t.edge->matches('.')) || t.edge->isEpsilon()) {
                        cout<<"     + Match on " <<t.edge->getLabel()<<endl;
                        int next = t.edge->isEpsilon() ? strPos:strPos+1;
                        sf.push(make_pair(next, t.to));
                    }
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