
#include "src/scanner.h"
#include "src/parser.h"
#include "src/execute.h"
#include "src/helper.hpp"

using namespace std;

int main(int argc, char *args[])
{
	if(argc != 2){
		std::cout<<"Please provide input file to compile or execute."<<endl; 
		exit(EXIT_FAILURE);
	}

	scanner c = scanner(args[1]);
	// c.print();
	
	parser p = parser(c.token_list, c.j);
		
	//p.code_print();
    //p.stack->print();
	
   execute e = execute(p.code, p.stack);
   code_tk t = *(code_tk*)(e.code + e.ip);


typedef int (execute::*Function) ();

// needs to be in order same as enum
   Function exe[] = {
	   &execute::push,
	   &execute::pushi,
	   &execute::pushc,
	   &execute::pushf,
	   &execute::pop,
	   &execute::add,
	   &execute::sub,
	   &execute::mul,
	   &execute::div,
	   &execute::NOT,
	   &execute::AND,
	   &execute::jmp,
	   &execute::eof,
	   &execute::pos,
	   &execute::neg,
	   &execute::print,
	   &execute::printc,
	   &execute::OR,
	   &execute::greater,
	   &execute::greater_eql,
	   &execute::less,
	   &execute::less_eql,
	   &execute::eql,
	   &execute::pushb,
	   &execute::jfalse,
	   &execute::jtrue,
	   &execute::dup,
	   &execute::remove,
	   &execute::restore,
	   &execute::geti,
	   &execute::getc,
	   &execute::getf,
	   &execute::getb,
	   &execute::puti,
	   &execute::putc,
	   &execute::putf,
	   &execute::putb
   };
   int v = 0;
   while (t != op_eof){
	   //std::cout << code_tk_string[t] << endl;
	   t = *(code_tk*)(e.code + e.ip);
	   v = (e.*exe[t])();
	   if (v == 0) exit(EXIT_FAILURE);
   }

	return 0;
}
