#pragma once
#include "lex.h"
class Constant
{
	string name;
public:
	Constant(string& NAME) { name = NAME; };
	Constant(const Constant& obg) { name = obg.name; };

	void set_name(string& N) { name = N; };
	string& get_name() { return name; };
}; 