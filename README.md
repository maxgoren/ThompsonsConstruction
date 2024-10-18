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
  - subset construction

So far only backtracking DFS is supported.
