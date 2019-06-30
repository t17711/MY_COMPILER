#pragma once
#include <iostream>
#include <fstream>      // std::ifstream
#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>
#include "token.h"
using namespace std;

class scanner{
public:
	char *buffer;
	token **token_list;// = new token*[result];
	size_t result;
	int i=0;
	int j=0;
	unordered_map<string, token_name> key_table;
	unordered_map<char, token_name> operator_table;
	int key_n;

public:
	scanner(char[]);
	~scanner();
	void print();
	void get_eof();
	void get_key_table();
	void get_operator_table();
	void check_keyword(token* tk);
	token* check_operator(char);
	void get_token();
	void identifier();
	void comment(int type);
	void get_char_token();
	void digit_token();
	void get_string_token();
	void operator_token();
	void float_token();
	void exp_token();
	void check_eof(string);
	void inc();
};