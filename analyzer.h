#pragma once

#include "lex.h"
#include "id.h"
#include "const.h"
#include "keywords.h"
#include "delim.h"
#include "table.h"

using namespace std;



struct Func {
	string id;
	string func_parms[1000];
	int i = 0;
	void add_parm(string a) {
		i++;
		func_parms[i-1] = a;
	}
};

class Element_Sy
{
public:
	Element_Sy* parent;
	Element_Sy** sons;
	int sons_c, def_sonc_c;
	Lexeme l;
	Element_Sy()
	{
		parent = nullptr;
		sons = (Element_Sy * *)malloc(sizeof(Element_Sy*) * 1);
		sons_c = 0;
	}
	void add_son(Lexeme A)
	{
		sons = (Element_Sy**)realloc(sons, (sons_c+1)*sizeof(Element_Sy*));
		sons[sons_c] = new Element_Sy;
		sons[sons_c]->parent = this;
		sons[sons_c]->l = A;
		sons_c++;
	}
	void show() 
	{
		cout << l.get_name() << endl;
		for (int j = 0; j < sons_c; j++)
		{
			this->sons[j]->show();
		}
		cout << endl;
	}
};

class Tree
{
public:
	Element_Sy* start;
	Element_Sy* tmp;
	Element_Sy* ret_point;
	int depth;

	Tree() {
		start = NULL;
		tmp = NULL;
		ret_point = NULL;
		depth = 0;
	}

	void move_up()
	{
		tmp = tmp->parent;
	}
	void move_down(int a)
	{
		tmp = tmp->sons[a];
	}
	void returno()
	{
		tmp = ret_point;
	}
	void show()
	{
		start->show();
	}
};


class Scaner
{
	Func func_table;
	ifstream in_prog; 

	Tree tree;

	Keywords table_of_keyWords;
	Delimiters table_of_delimiters;

	Table <Lexeme> stream_of_token;
	Table <Identifier> table_of_identifier;
	Table <Constant> table_of_constant;
	char keeper_last_char;
public:
	Tree T;
	Scaner();
	~Scaner();

	string get_lex();
	bool delete_comments(char c1, char c2);
	void to_scan();

	void print_stream_of_lex();
	void print_another_table();

	void sintaxys();
};


