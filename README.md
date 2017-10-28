Compiler project

# Instructions

## STACK ##
- Docker
- Ubuntu image: ubuntu xenial
- GCC 5.4


## RUNNING ##
- Install docker
- Then use the docker-compose to build and start the container. If not when running run.sh it will automatically do that
- Then to compile execute ./run.sh -m which starts docker if its not started and calls make on code
- To run execute ./run.sh -r "code name" it will pass file name to main. Code file needs to be in docker container, so just save it along with this program. Here i put them in test
- To go into docker container to do ./run.sh -e
- to clean ./run.sh -c

## Language ##

My program has 3 part:

1. Variable declaration
2. Statements
3. procedures

### So body looks like: ###

```
{ <- This is start of program

{
	Variable declaration
}

{
	Statements
}

	Procedures => name(){}

} <- This is end of program
```

1. Declaration

```
For declaration I have types
a. For Int, char, bool, float and procedures I declare like:
Int x, y, z;
Procedure a, b, c;
b. For array I declare for bool , Int , char and float only:
```

```
Int [10];
Bool [50];
```

2. Statements
a. Procedure call

```
For declared procedure just do
A ();
```

b. Assignment

i. Primitives like Int bool are assigned like
```

Compiler project

```
X = 10; y = ‘c’;
```
```
ii. Array is assigned like
```
```
X [10] = 20;
```
```
Array index is always integer. I could not do for other in time.
```
```
c. If condition
For if I do
The condition can be any expression that results in bool like x>0 && y + 12 > 50
If condition {
Statements;
}
d. Do while
Do {
Statements.
}
While conditions
e. While
While condition {
Statements.
}
f. For loop
For (assignment; condition; increment) {
Statement.
}
Assignment for already declared integer like I = 0;
Condition can be any expression that results in bool, or I get error
Increment is any assignment like I = i+10;
```
3. Procedure

```
I just have id and statements like
```
```
X {
```
```
Statements.
```
```
}
```
```
I save the address of code of assignment in symbol tab. Before call I save address after
call in stack. I jump to address in symbol tab which is the statements in this procedure. After
statement there is address to the code after procedure call as last item remaining in stack which
I use to go back.
```
# Language

I used enum variables as token name. Each token is a class that has toke name, field for id, int, float, and
bool. Then I compare those token names for parser.


Compiler project


##Program start

  \<start-prog\>   →  TK_-  BEGIN\<var-decl\>\<statements\>TK_- END .halt

**Variable declaration**

 \<var-decl\>  → TK_- Begin\<decl\>TK_- END

 \<decl\>  → \<type\>\<namelist\>TK_- SEMICOLON   |


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\<decl\> 

 \<type\>  → TK_- NT_- TYPE |


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; TTK_- CHAR_- TYPE |


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;TK_- BOOL-TYPE

 \<namelist\> →TK_- ID   TK_- COMMA  |


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; TK_- D  TK_- SEMICOLON

**Statements**

 \<statements\>  → TK_BEGIN    \<statements types\>(pop)TK_END
 \<statements types\>  → \<assignment\>|


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; \<for\>|


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\<while\>|


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\<statements type\>

**Assignment**

 \<assignment\>  →\<calculator\>|


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\<procedure\>|


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			\<assignment\>|


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;TK-SEMICOLON

**Calculator**

 \<calculator\>  →\<add sub\> \<expression-mul-div\>

 \<add sub\>  → empty |


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; TK_- PLUS\<expression-mul-div\>(op_- plus)\<add sub\>|


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; TK_- MINUS \<expression-mul-div\>(op_-minus)\<add sub\>|


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; TK_- OR\<expression-mul-div\>(op_- or)\<add sub\>

 \<expression-mul-div\>  →\<value\>\<mul-div\>

 \<mul-div\>  → empty |


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;TK_- MUL\<value\>(op_- mul)\<mul-div\>  |


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;TK_- DIV\<value\>(op_- div)\<mul-div\>|


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;TK_- EQUAL_COMP\<value\>(op_- eql) \<mul-div\>|


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; TK_- GREATER_EQL\<value\>(op_- greater_eql) \<mul-div\>TK_- REATER\<value\>(op_- greater) \<mul-div\>TK_- LESS\<value\>(op_- less) \<mul-div\>|


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;TK_- LESS_EQUAL\<value\>(op_- less_eql) \<mul-div\>|


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; TK_- AND\<value\>(op_- and) \<mul-div\>

 \<value\>  → empty|


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; TK_- INT(op-pushi)|


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; TK_- D (op-push) |


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; TK_- PLUS \<value\>(op-pos)|


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; TK_- MINUS\<value\>(op-neg) |


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; TK_- OPEN\<calculator\>TK_- CLOSE

**Do while**

 \<do_- while -statemenent\> →  TK-DO\<statements\>TK_- WHILE\<expressions\>(op_- jtrue)  TK_- SEMICOLON 

**While**

 \<while-statemenent\> → TK_- WHILE\<expressions\>(op_- jfalse)\<statements\>(op_- jmp) 

**If**

 \<if-statemenent\> → TK_- IF\<expressions\>(op_- jfalse)\<statements\>\<else-statement\>

 \<else-statemenent\> →  empty |


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; (op_- jmp)TK_- ELSE \<statements|


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; \<else-statement\>

**Switch Case**

 \<switch-statement\> → TK_- SWITCH\<value\> \<case-statement\>

 \<case-statement\> → empty|


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; TK_- CASE \<value\> TK_- COLON (op_- dup)(op_- eql)(op_- jfalse) \<statements\>(op_- jmp) |


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\<case_- statement\>|


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 

**\<default\>**

 \<default\>  → \<statements\>
