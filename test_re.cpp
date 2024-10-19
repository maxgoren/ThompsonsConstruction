#include <iostream>
#include "compiler.hpp"

void test() {
    NFACompiler compiler;
    NFA nfa = compiler.compile("(a*b|ac)d");
    string tests[] = { "aaaaabd", "aaaaac", "abd", "bd"};
    for (int i = 0; i < 4; i++) {
        cout<<"---------------------"<<endl;
        cout<<nfa.match(tests[i])<<endl;
    }
}

void test2() {
    NFACompiler compiler;
    NFA nfa = compiler.compile("r(a|e|i|o|u)n+ing.*");
    cout<<nfa.match("running")<<endl;
    cout<<"------------------"<<endl;
    cout<<nfa.match("rwning")<<endl;
    cout<<"------------------"<<endl;
    cout<<nfa.match("riningmates")<<endl;
}

void test3() {
    NFACompiler compiler;
    NFA nfa = compiler.compile("a(b|cd)*ef");
    cout<<nfa.match("abbcdef")<<endl;
}

void test4() {
    NFACompiler compiler;
    NFA nfa = compiler.compile("r[aeiou]n+ing.*");
    cout<<nfa.match("running")<<endl;
    cout<<"------------------"<<endl;
    cout<<nfa.match("rwning")<<endl;
    cout<<"------------------"<<endl;
    cout<<nfa.match("riningmates")<<endl;
}

void testCata() {
    NFACompiler compiler;
    NFA nfa = compiler.compile("a*a*a*a*a*a*a*a*a*a*a*a*aac");
    cout<<nfa.matchbt("aaaaaaaaaaaaaaaaaaaab")<<endl;
    cout<<nfa.match("aaaaaaaaaaaaaaaaaaaaaab")<<endl;
}

int main() {
    test4();
    test2();
    //test3();
    //testCata();
    return 0;
}