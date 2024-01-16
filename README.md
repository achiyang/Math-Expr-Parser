# Math-Expr-Parser

## BNF 형식에 따라 정의한 Expression
```
<expression> ::= <term> | <term><addsub><expression>
<term> ::= <factor> | <factor><muldiv><term>
<factor> ::= <number> | '('<expression>')'
<number> ::= <digit> | <number><digit>
<digit> ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
<addsub> ::= '+' | '-'
<muldiv> ::= '*' | '/'
```
