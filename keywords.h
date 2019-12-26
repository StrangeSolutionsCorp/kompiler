#pragma once
#include "lex.h"
class Keywords
{
	vector <string> keyWords;
public:
	Keywords();

	vector <string>& get_vect_key_word() { return keyWords; };
	bool operator ==(string& obg);
}; 