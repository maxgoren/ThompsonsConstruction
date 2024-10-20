#include <iostream>
#include "compiler.hpp"
#include "patternmatcher.hpp"

void test() {
    NFACompiler compiler;
    NFA nfa = compiler.compile("(a*b|ac)d");
    PowerSetPatternMatcher pm(nfa);
    string tests[] = { "aaaaabd", "aaaaac", "abd", "bd"};
    for (int i = 0; i < 4; i++) {
        cout<<"---------------------"<<endl;
        cout<<pm.match(tests[i])<<endl;
    }
}

void test2() {
    NFACompiler compiler;
    NFA nfa = compiler.compile("r(a|e|i|o|u)n+ing.*");
    PowerSetPatternMatcher pm(nfa);
    cout<<pm.match("running")<<endl;
    cout<<"------------------"<<endl;
    cout<<pm.match("rwning")<<endl;
    cout<<"------------------"<<endl;
    cout<<pm.match("riningmates")<<endl;
}

void test3() {
    NFACompiler compiler;
    NFA nfa = compiler.compile("a(b|cd)*ef");
    PowerSetPatternMatcher pm(nfa);
    cout<<pm.match("abbcdef")<<endl;
}

void test4() {
    NFACompiler compiler;
    NFA nfa = compiler.compile("r[E-Ve-v]n+ing.*");
    PowerSetPatternMatcher pm(nfa);
    cout<<pm.match("running")<<endl;
    cout<<"------------------"<<endl;
    cout<<pm.match("rwning")<<endl;
    cout<<"------------------"<<endl;
    cout<<pm.match("rIningmates")<<endl;
}

void testCata() {
    NFACompiler compiler;
    NFA nfa = compiler.compile("a*a*a*a*a*a*a*a*a*a*a*a*aac");
    PowerSetPatternMatcher pm(nfa);
    cout<<pm.match("aaaaaaaaaaaaaaaaaaaab")<<endl;
}

void test5() {
    NFACompiler compiler;
    NFA nfa = compiler.compile("(a|b){3}bd");
    PowerSetPatternMatcher pm(nfa);
    cout<<pm.match("bbbad")<<endl;
}

int main() {
   /* testCata();
    test();
    test4();
    test2();
    test3();  */
    test5();
    return 0;
}