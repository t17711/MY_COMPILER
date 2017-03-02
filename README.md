


**The grammar for this compiler is

**Program start
  \<start-prog\>   →  TK_-  BEGIN\<var-decl\>\<statements\>TK_- END .halt

**Variable declaration
 \<var-decl\>  → TK_- Begin\<decl\>TK_- END

 \<decl\>  → \<type\>\<namelist\>TK_- SEMICOLON   |\<decl\> 

 \<type\>  → TK_- NT_- TYPE | TTK_- CHAR_- TYPE |TK_- BOOL-TYPE

 \<namelist\> →TK_- ID   TK_- COMMA  | TK_- D  TK_- SEMICOLON

**Statements
 \<statements\>  → TK_BEGIN    \<statements types\>(pop)TK_END
 \<statements types\>  → \<assignment\>| \<for\>|\<while\>|\<statements type\>

**Assignment**
 \<assignment\>  →\<calculator\>|\<procedure\>|\<assignment\>|TK-SEMICOLON

**Calculator**
 \<calculator\>  →\<add sub\> \<expression-mul-div\>

 \<add sub\>  → empty | TK_- PLUS\<expression-mul-div\>(op_- plus)\<add sub\>| TK_- MINUS \<expression-mul-div\>(op_-minus)\<add sub\>| TK_- OR\<expression-mul-div\>(op_- or)\<add sub\>

 \<expression-mul-div\>  →\<value\>\<mul-div\>

 \<mul-div\>  → empty |TK_- MUL\<value\>(op_- mul)\<mul-div\>  |TK_- DIV\<value\>(op_- div)\<mul-div\>|TK_- EQUAL_COMP\<value\>(op_- eql) \<mul-div\>| TK_- GREATER_EQL\<value\>(op_- greater_eql) \<mul-div\>TK_- REATER\<value\>(op_- greater) \<mul-div\>TK_- LESS\<value\>(op_- less) \<mul-div\>|TK_- LESS_EQUAL\<value\>(op_- less_eql) \<mul-div\>| TK_- AND\<value\>(op_- and) \<mul-div\>

 \<value\>  → empty| TK_- INT(op-pushi)| TK_- D (op-push) | TK_- PLUS \<value\>(op-pos)| TK_- MINUS\<value\>(op-neg) | TK_- OPEN\<calculator\>TK_- CLOSE

**Do while**
 \<do_- while -statemenent\> →  TK-DO\<statements\>TK_- WHILE\<expressions\>(op_- jtrue)  TK_- SEMICOLON 

**While**
 \<while-statemenent\> → TK_- WHILE\<expressions\>(op_- jfalse)\<statements\>(op_- jmp) 

**If**
 \<if-statemenent\> → TK_- IF\<expressions\>(op_- jfalse)\<statements\>\<else-statement\>

 \<else-statemenent\> →  empty | (op_- jmp)TK_- ELSE \<statements| \<else-statement\>

**Switch Case**
 \<switch-statement\> → TK_- SWITCH\<value\> \<case-statement\>

 \<case-statement\> → empty| TK_- CASE \<value\> TK_- COLON (op_- dup)(op_- eql)(op_- jfalse) \<statements\>(op_- jmp) |\<case_- statement\>| 

**\<default\>**
 \<default\>  → \<statements\>
