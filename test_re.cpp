#include <iostream>
#include "compiler.hpp"
void test() {
    NFACompiler compiler;
    NFA nfa = compiler.compile("(a+b|ac)d");
    string tests[] = { "aaaaabd", "aaaaac", "abd", "bd"};
    for (int i = 0; i < 4; i++) {
        cout<<"---------------------"<<endl;
        cout<<nfa.match(tests[i])<<endl;
    }
}

void test2() {
    NFACompiler compiler;
    NFA nfa = compiler.compile("r(a|e|i|o|u)n+ing.*");
    cout<<nfa.match("rin")<<endl;
    cout<<nfa.match("roman")<<endl;
    cout<<nfa.match("runningmates")<<endl;
}

void test3() {
    NFACompiler compiler;
    NFA nfa = compiler.compile(".*a(b|cd)*ef.*");
    cout<<nfa.match("aabbcdeffaefe")<<endl;
}

int main() {

    test3();
    return 0;
}