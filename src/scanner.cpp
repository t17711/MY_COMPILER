#include "scanner.h"

using namespace std;

void
scanner::inc(){
	i++;
	if(!buffer[i]) throw "CODE ENDS BEFORE EOF";
}

// constructor that reads the file converts file to upper case char array
scanner::scanner(char name[]){
		ifstream pfile (name, ios::binary); // rb is read as binary

		if (!pfile) {
			throw "FILE READ ERROR: File opening failed";
		}

		// read the file into string
		std::string input((std::istreambuf_iterator<char>(pfile)), std::istreambuf_iterator<char>());
		pfile.close();
		
		// buffer = const_cast<char*> (input.c_str());
		

	    int char_len = input.length();
		buffer = new char[char_len];


		token_list = new token*[char_len];

		int buf = 0;
		// convert all non-string to uppercase
		// copy to buffer
		int i=0;
		while(i<char_len){

			// dont touch string because string case are important
			if(input[i]=='"'){
				buffer[buf++]=input[i++]; // copy "
				while(i<char_len && input[i]!='"') buffer[buf++]=input[i++]; // copy inside " "
				if(i==char_len) break;
				buffer[buf++]=input[i++]; // copy "
			}
			else if(input[i]=='\''){
				buffer[buf++]=input[i++]; // ' char begin
				buffer[buf++]=input[i++]; // char or "\"

				// if char doesnot end in next 2 step then error
				if(input[i]!='\'' && input[i+1]!='\'') {
					throw printf("CHAR ERROR: %c %s", input[i], "instead of END of CHAR");
				}

				// if char ends in i+1 step
				if(input[i+1]=='\''){
					// now copy rest
					buffer[buf++]=input[i++]; // special char
				}
				
				buffer[buf++]=input[i++]; // ' char end
			
			}
			else{
				buffer[buf++]=toupper(input[i++]);

			}
		}

		buffer[buf] = EOF;

		//while (t<result) cout << buffer[t++];

		// create a list of tokens
		get_key_table();

		// operator table
		get_operator_table();
		
		// now get token from data
		get_token();
	}

// deletes allocated arrays
scanner::~scanner(){
		delete [] buffer;
		free(token_list);
}

/**
 * Check if EOF occurs and if ir does throw error
 */
void
scanner::check_eof(string s){
	if(buffer[i]==EOF){
		throw printf("FILE END ERROR: file ends at %s \n", s);
	}
}

/**
 * Main scanner
 * This looks at individual characters and checks if they are possibly id, or operators or whitespace then sends them to correct procedure
 */
void
scanner::get_token(){
		// this is start of a token
	char curr;
	int a=0;
	i=0;

	while(buffer[i]){

		// check if char are legal';
		curr = buffer[i];
		// cout<<curr;
		if (curr == EOF)  {
			get_eof(); // this should exit the loop put eof on token list
			return;
		}
		else if (curr <= 32){/// skip spaces
			inc();
			continue;
		}
		else if ('A' <= curr && curr <= 'Z'){
			identifier();
			continue;
		}
		else if ('a' <= curr && curr <= 'z'){
			throw printf("LOWERCASE CHARACTER ERROR: %c\n", curr);
		}
		else if ('0' <= curr && curr <= '9'){
			digit_token();
			continue;
		}
		else {
			operator_token();
			continue;
		}

		// else break 
		// but if it breks means token was wrong
		// since we only exit on EOF
		throw printf("INVALID CHARACTER: %c", curr);
	}

}


// prints tokens
void
scanner::print(){
		int t = 0;
		while (token_list[t]->name != TK_EOF) {
			token_list[t]->print();
			t++;
		}
		token_list[t]->print();
		std::cout << std::endl << std::endl;
	}

// sent here for last token
void
scanner::get_eof(){
	//printf("get_eof");
	token_list[j] = tk(TK_EOF);	
}

// creates dictionary of keywords
void
scanner::get_key_table(){
	key_table = unordered_map<string, token_name> ({
		{"CHAR",	TK_CHAR_DEF},
		{"FOR",		TK_FOR},
		{"INT",		TK_INT_DEF},
		{"FLOAT",	TK_FLOAT_DEF},
		{"STRING",	TK_STRING_DEF},
		{"WHILE",	TK_WHILE},
		{"BOOL",	TK_BOOL_DEF},
		{"RETURN",	TK_RETURN},
		{"PRINT",	TK_PRINT},
		{"OR",		TK_OR},
		{"AND",		TK_AND},
		{"DO",		TK_DO},
		{"TRUE",	TK_TRUE},
		{"FALSE",	TK_FALSE},
		{"NOT",		TK_NOT},
		{"IF",		TK_IF},
		{"ELSE",	TK_ELSE},
		{"SWITCH",	TK_SWITCH},
		{"CASE",	TK_CASE},
		{"DEFAULT",	TK_DEFAULT},
		{"PROCEDURE",TK_PROCEDURE_DEF}
	});

}

void
scanner::get_operator_table(){
	operator_table = unordered_map<char, token_name> ({
		{'{', 	TK_BEGIN},
		{'}',	TK_END},
		{'(',	TK_OPEN},
		{')',	TK_CLOSE},
		{'[',	TK_SQUARE_OPEN},
		{']',	TK_SQUARE_CLOSE},
		{'+', 	TK_PLUS},
		{'-',	TK_MINUS},
		{'*',	TK_MUL},
		{'/', 	TK_DIV},
		{'=',	TK_EQUAL},
		{'>', 	TK_GREATER},
		{'<', 	TK_LESS},
		{'?',	TK_QUES},
		{';',	TK_SEMICOLON},
		{',',	TK_COMMA},
		{':', 	TK_COLON},
		{'\'',	TK_QUOTE},
		{'"',	TK_DOUBLE_QUOTE}
	});
}

// checks keywoords
void
scanner::check_keyword(token* tk){
		token_name t = key_table[tk->id];
		if (t != TK) {
			tk->name = t;
			tk->id="";
		}
}

token* 
scanner::check_operator(char curr){	
	// if operator not valid then exit
	if(operator_table.find(curr)==operator_table.end()){
		throw printf("OPERATOR ERROR: Invalid Operator: %s", curr);
	}

	return tk(operator_table[curr]);
}

/**
 * if a char is read first then it is id, check if id is keyword
 * keyword can include numbers and _ bun cannot start with them.
 * if any illegal char ios found the id upto that char is individual id 
*/ 
void
scanner::identifier(){
	// i made it so that we enter this function if there is valid for identifier
	// error("ID END");
	

	char curr;
	token_list[j] = tk(TK_ID); // init token

	IDENTIFIER_LOOKUP:

	// if eof occurs here throw error and end
	check_eof("Identifier "+ token_list[j]->id);

	curr = buffer[i];

	/**
	* if current character is valid identifier continue
	* if no whitespace continue reading
	* Since identifier is called only if curr is char we can use following
	*/
	if (('A' <= curr && curr <= 'Z') ||
				curr == '_' ||
				(curr >= '0' && curr <= '9')){
		// valif names are alphabet nmbers and underscore, only alphabet starts
		token_list[j]->id += curr;
		inc();
		goto IDENTIFIER_LOOKUP;
	}

	// if not valid char it means id is complete 
	else{
	    
	    // if non id character after an char then we say the value is identifier
		check_keyword(token_list[j]); // if it was identifier token name is chnged and id is set emapty
		j++;
		return;
	}

}

// this is to check different types of operator
void 
scanner::operator_token(){
	// error("OPERATOR END");

	//	cout << "operator_token\n";

	char curr;
	curr = buffer[i++];

	if(curr!='}') check_eof("Identifier "+ curr);

	// create whatever the token is
	token_list[j]= check_operator(curr);
	
	// sometimes tokens are multi char or they start string or char so do it here
	switch(token_list[j]->name){
		// '/' may be div but it also starts comments
		case TK_DIV: // '// or /*'
			// check for comment
			if (buffer[i] == '/'){
				inc();
				comment(0);
			}
			else if (buffer[i] == '*'){
				inc();
				comment(1);
			}
			// comment means token read is cancelled so return
			token_list[j]= NULL;
			return;

		// check for >=
		case TK_GREATER: // '>=':
			if (buffer[i] == '='){
				token_list[j]->name = TK_GREATER_EQUAL;
				inc();
			}
			break;

		// check for <=
		case TK_LESS: // '<=':
			if (buffer[i] == '='){
				token_list[j]->name = TK_LESS_EQUAL;
				inc();
			}
			break;

		// check for ==
		case TK_EQUAL: // '==':
			if (buffer[i] == '='){
				token_list[j]->name = TK_EQUAL_COMP;
				inc();
			}
			break;

		// check for char
		case TK_QUOTE: // '\'':
			//	get char
			get_char_token();
			return;

		// check for string
		case TK_DOUBLE_QUOTE: // '"':
		//	get_string
			get_string_token();
			return;		
		
		default: break;
	}
		// increment token	
	j++;	

}

/**
 * if char starts with number then go to number
 * checks for float
 * and integer and char 'E' for exponent
*/
void 
scanner::digit_token(){
	// error("DIGIT END");

	// int are int
	// decimals are 2 int
	// there is also exp portion as power of 10
	// float later
	//printf("digit_token\n");
	
	char curr;
	curr = buffer[i];

	// declare num token
	token_list[j] = tk(TK_INT);

	while ('0' <= curr && curr <= '9'){
		token_list[j]->int_value = token_list[j]->int_value * 10 + (curr - '0');
		inc();
		curr = buffer[i];
	}

	if (curr == '.'){
		inc();
		float_token();
		return;
	}
	else if (curr == 'E'){
		inc();
		exp_token();
		return;
	}

	// digit cannot be followed by character, i did not do hex here maybe later
	else if (curr >= 'A' && curr <= 'Z'){
		throw printf("Error in integer %c", curr);
		return;
	}

	// if . or e are not found and other non numbers are then they are next token
	else{
		check_eof("Number " + token_list[j]->int_value);
		token_list[j++]->name = TK_INT;  // ist take everything as int
		return;
	}
}

/**
 * if there is decimal in number than it is float
 */
void 
scanner::float_token(){
	//printf("double\n");

	token_list[j]->name = TK_FLOAT;  // change toklen to float

	char curr;
	curr = buffer[i];
	token_list[j]->exp = 0; //  0.1 decimal is 1* 10^-1

	// 1st copy int value
	token_list[j]->float_value = static_cast<float>(token_list[j]->int_value);
	token_list[j]->int_value = 0;

	float e = 0.0;
	float pos = 1.0;
	while ('0' <= curr && curr <= '9')
	{
		e = e*10+ (curr - '0');
		curr = buffer[++i];
		pos*=10;
	}

	// add exponent part too
	token_list[j]->float_value += e/pos;

	if (curr == 'E')
	{
		inc();
		exp_token();
		return;
	}
	else if (curr <= 32 || curr == ';')
	{
		j++;
		
		return;
	}
	else
	{
		printf("bad float: %c\n", curr);
		j++;
		get_eof();
		return;
	}
}

/**
 * if the float or integer number has 'E' charthen it is exponent
 */
void
scanner::exp_token(){
	//printf("double\n");

	token_list[j]->name = TK_FLOAT;  // 1st take everything as int
	char curr;
	int temp = 0;
	curr = buffer[i];

	while ('0' <= curr && curr <= '9')
	{
		temp = temp * 10 + (curr - '0');
		inc();
		curr = buffer[i];
	}
	token_list[j]->exp =  temp;
		
	if (curr <= 32 || curr == ';')
	{
		j++;
		
		return;
	}
	else
	{
		printf("bad exp: %c\n", curr);
		j++;
		get_eof();
		return;
		
	}
}

/**
 * this thisng starts from " and ends at another " and returns string value
 */
void 
scanner::get_string_token(){

	token_list[j]->name = TK_STRING;  // ist take everything as int
	char curr;
	curr = buffer[i];

	// wait until "
	while (buffer[i] && curr != '"'){
		// cout<<curr;
		if (curr == '\0'){
			throw printf("STRING ERROR: NULL char in string after %c", curr);
		}
		token_list[j]->id += curr;

		inc();
		check_eof("String scanning" +string(1,curr));
		curr = buffer[i];
	}
	inc(); j++;
}

/**
 * this starts at one ' and has one char or \n then ends with ' and returns the char
 */
void
scanner::get_char_token(){
	//std::cout << buffer[i] << endl;
	token_list[j]->name = TK_CHAR;  
	// any thing can be char

	if (buffer[i] == '\\'){
		inc();
		switch (buffer[i]){
		case 'n':
			token_list[j]->char_val = '\n';
			break;
		case 't':
			token_list[j]->char_val = '\t';
			break;
		default:
			throw printf("CHARACTER ERROR: bad string escape char %c", buffer[i]);
		}
		inc();
	}
	else{
		token_list[j]->char_val = buffer[i];
		inc();
	}
		
	if (buffer[i]!= '\''){
		throw printf("CHARACTER ERROR: char not closed", buffer[i]);
	}
	else{
		inc();
		j++;
		
	}
}

/**
 * this scans // and skips all char after that upto endline
 * also it scans/* and skips all char upto 
*/
void 
scanner::comment( int type){
	//skip until end line if //
	if (type == 0)
		while (buffer[i] != EOF && buffer[i] != '\n') i++;

	// skip until */
	else if (type == 1){
		while (buffer[i] != EOF){
			if (buffer[i] == EOF) {
				get_eof();
				return;
			}
			else if (buffer[i++] == '*' && buffer[i] == '/') break;
		}
	}
	inc();
	//no j++ since it skips the operator
	
	return;
}
