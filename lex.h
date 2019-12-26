#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;

enum { KEYWORD, DELIMITERS, IDENTIFIER, CONSTANT, ENDF, ERROR, BEGIN};

class Lexeme//1 Лексема
{
	string name;
	int type;//1-ID; 2- KEYWORD; 3 -DELIMITERS; 4-CONSTANT.
public:
	string typeP;
	bool inic;

	Lexeme() {
		name = "hui";
		type = 1;
		typeP = "";
		inic = false;
	};
	Lexeme(string& NAME, int TYPE);
	Lexeme(const Lexeme& obg);


	void set_name(string& N) { name = N; };
	string& get_name() { return name; };

	void set_type(int N) { type = N; };
	int get_type() { return type; };

	Lexeme& operator==(const Lexeme& ob) {
		this->name = ob.name;
		this->type = ob.type;
		return *this;
	}
};
