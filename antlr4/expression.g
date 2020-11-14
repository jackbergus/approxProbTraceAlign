grammar expression;

expression : '(' expression ')'                        #paren
           |<assoc=right> expression '+' expression    #plus
           |<assoc=right> expression '-' expression    #minus
           |<assoc=right> expression '\\' expression    #div
           |<assoc=right> expression '*' expression    #times
           |<assoc=right> expression '^' expression    #pow
           | 'sqrt' expression                         #sqrt
           | 'abs'  expression                         #abs
           | 'zip'  STRING STRING NUMBER VECTOR VECTOR #zip
           | 'fold' STRING        NUMBER VECTOR        #fold
           ;

INTNUMBER : ('0'..'9')+ ;
NUMBER : INTNUMBER ('.' INTNUMBER)?;
VECTOR : 'v' INTNUMBER;
STRING : '"' (~[\\"] | '\\' [\\"])* '"';
SPACE : [ \t\n]+ -> skip;