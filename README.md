# ThompsonsConstruction
Thompsons Construction Algorithm for building NFA from regular expressions

Steps:
  - Insert explicit concat operator symbol into string
  - convert Infix expression to Postfix
  - Build AST from postfix expression string
  - construct NFA from post-order traversal of AST
     

## Simulating the NFA
Once the NFA has been constructed it can be used to perform pattern matching on text strings.
There are two main ways of simulating the NFA:
  - backtracking depth first search
  - powerset construction

Backtracking DFS is conceptually simple, but potentially slow on some inputs
Powerset construction is smecksy and fast on all inputs.
