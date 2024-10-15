# ThompsonsConstruction
Thompsons Construction Algorithm for building NFA from regular expressions

Steps:
  - Insert explicit concat operator symbol into string
  - convert Infix expression to Postfix
  - Build AST from postfix expression string
  - construct NFA from post-order traversal of AST
     
