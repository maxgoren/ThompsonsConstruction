# ThompsonsConstruction
Thompsons Construction Algorithm for building NFA from regular expressions

Steps:
  - Insert explicit concat operator symbol into string
  - convert string to token stream
  - convert token stream from Infix expression to Postfix
  - Build AST from postfix expression 
  - construct NFA from post-order traversal of AST
     

## Simulating the NFA
Once the NFA has been constructed it can be used to perform pattern matching on text strings.
There are two main ways of simulating the NFA:
  - backtracking depth first search
  - Thompson's algorithm (powerset construction)

Backtracking DFS is conceptually simple, but potentially slow on some inputs
Powerset construction is smecksy and fast on all inputs.

