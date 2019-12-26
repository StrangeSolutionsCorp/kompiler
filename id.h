#pragma once
#include "lex.h"

class Identifier
{
	string name;
public:
	Identifier(string& NAME) { name = NAME; };
	Identifier(const Identifier& obg) { name = obg.name; };

	void set_name(string& N) { name = N; };
	string& get_name() { return name; };

}; 
