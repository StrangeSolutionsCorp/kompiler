#include "lex.h"


Lexeme::Lexeme(string& NAME, int TYPE)
{
	name = NAME;
	type = TYPE;
}

Lexeme::Lexeme(const Lexeme& obg)
{
	name = obg.name;
	type = obg.type;
}