grammar regex;

regex : '(' regex ')'                          #paren
      |  regex ( NUMBER )? '+'           #plus
      |  regex ( NUMBER )? '*'           #star
      |  regex ( NUMBER )? '?'           #optional
      |  STRING                          #string
      |  regex ( NUMBER )? regex        #consequent
      |  regex '|' (NUMBER)? regex     #binaryAlternative
      ;


INTNUMBER : ('0'..'9')+ ;
NUMBER : INTNUMBER ('.' INTNUMBER)?;
STRING : '"' (~[\\"] | '\\' [\\"])* '"';
WORD : ~[ \t\r\n"*]+;
NEWLINE : '\r'? '\n' | '\r';
SPACE : [ \t]+ -> skip;