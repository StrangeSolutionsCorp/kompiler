#pragma once
#pragma once
#include "lex.h"

class Delimiters
{
	vector <string> delimiters;
public:
	Delimiters();



	vector <string>& get_vect_key_word() { return delimiters; };
	bool operator ==(string& obg);
};