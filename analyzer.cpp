#include "analyzer.h"

Scaner::Scaner()
{
	keeper_last_char = '\0';

	in_prog.open("program.txt", ios::in);

	if (!in_prog.is_open())
	{
		cout << "Error opening file program.txt" << endl;
		system("pause");
		system("cls");
	}

}

Scaner::~Scaner()
{
	in_prog.close();
}

string Scaner::get_lex()
{
	int state = BEGIN;
	string bufer, t_str;
	char c1, c2;
	//bool flag = true;
	c1 = keeper_last_char;

	//Пропуск пробелов табуляций и переносов строки преред лексемой

	do {
		if ((c1 == ' ') || (c1 == '\t') || (c1 == '\n'))
		{
			//flag = false;
			if (!in_prog.eof())
				c1 = in_prog.get();//Получение символа char
		}
		else
		{
			break;
		}
	} while (!in_prog.eof());

	if (keeper_last_char == '\0')
	{
		if (!in_prog.eof())
			c1 = in_prog.get();//Получение символа char
	}

	if (!in_prog.eof())
		c2 = in_prog.get();//Получение символа char

	if (c1 == '=' && c2 == '=') {
		
		bufer = "==";
		//Разделитель 
		Lexeme A(bufer, DELIMITERS);
		stream_of_token += A;
		c1 = in_prog.get();
		keeper_last_char = c1;
		return bufer;
	}

	if ((c1 >= '0') && (c1 <= '9'))
		state = CONSTANT;

	while ((!in_prog.eof()) || (c1 > 0))
	{
		
		if (delete_comments(c1, c2))//Пропуск коментариев
		{
			//
			if (!in_prog.eof())
				c1 = in_prog.get();
			
			while (!in_prog.eof())
			{

				if ((c1 == ' ') || (c1 == '\t') || (c1 == '\n'))
				{
					//flag = false;
					if (!in_prog.eof())
						c1 = in_prog.get();//Получение символа char
				}
				else
				{
					break;
				}
			}
			if (!in_prog.eof())
				c2 = in_prog.get();

			if ((c1 >= '0') && (c1 <= '9'))
				state = CONSTANT;
		}


		
		t_str = c1;
		if (state == CONSTANT)
		{
			if ((c1 < '0') && (c1 > '9'))
			{
				//error
				t_str = "";
				Lexeme A(t_str, ERROR);
				stream_of_token += A;
				return t_str;
			}

		}
		else
		{
			if (!(((c1 >= 'A') && (c1 <= 'Z')) || ((c1 >= 'a') && (c1 <= 'z')) || (table_of_delimiters == t_str) || ((c1 >= '0') && (c1 <= '9'))))
			{
				//error
				t_str = "";
				Lexeme A(t_str, ERROR);
				stream_of_token += A;
				return t_str;
			}
		}
		bufer += c1;
		t_str = c2;
		
		if (table_of_delimiters == bufer)//
		{
			keeper_last_char = c2;

			//Разделитель 
			Lexeme A(bufer, DELIMITERS);
			stream_of_token += A;

			return bufer;
		}
		if ((table_of_delimiters == t_str) || (c2 == ' ') || (c2 == '\t') || (c2 == '\n'))
		{
			keeper_last_char = c2;
			//Константа или ID Или Ключевое слово
			if (state == CONSTANT)//Константа
			{

				Lexeme A(bufer, CONSTANT);
				stream_of_token += A;
				if (!(table_of_constant == bufer))
				{
					Constant B(bufer);
					table_of_constant += B;
				}
			}
			else
			{
				if (table_of_keyWords == bufer)
				{
					Lexeme A(bufer, KEYWORD);
					stream_of_token += A;
				}
				else
				{
					Lexeme A(bufer, IDENTIFIER);
					stream_of_token += A;

					if (!(table_of_identifier == bufer))
					{
						Identifier B(bufer);
						table_of_identifier += B;
					}
				}
			}

			return bufer;
		}

		c1 = c2;

		if (!in_prog.eof())
			c2 = in_prog.get();
	}

	bufer = "";
	Lexeme A(bufer, ENDF);
	stream_of_token += A;

	return bufer;
}

bool Scaner::delete_comments(char c1, char c2)
{
	if (c1 == '/')
	{
		if (c2 == '/')
		{
			char c1;
			do {
				if (!in_prog.eof())
					c1 = in_prog.get();
			} while (c1 != '\n');
			return true;

		}
		else
		{
			if (c2 == '*')
			{
				while (!in_prog.eof())
				{
					if (!in_prog.eof())
						c1 = in_prog.get();
					if (c1 == '*')
					{
						if (in_prog.eof())
							return false;
						if (!in_prog.eof())
							c2 = in_prog.get();

						if (c2 == '/')
							return true;
					}
				}
			}
		}
	}
	return false;
}

void Scaner::to_scan()
{
	string a;
	tree.start = new Element_Sy;
	tree.tmp = tree.start;
	tree.tmp->def_sonc_c = -1;
	Lexeme temp_lex;
	Lexeme last_lex;
	string lex_name;
	int flag_return, flag_sons, flag_temp;
	flag_return = flag_sons = 0;

	int skob_flag, round_count, square_count, figure_count, main_flag;
	skob_flag = round_count = square_count = figure_count = 0;
	main_flag = 0;
	int sons_cur;
	sons_cur = 0;
	int dot_flag;
	dot_flag = 0;
	int if_flag;
	if_flag = 0;
	int func_flag;
	func_flag = 0;
	int func_add_flag;
	func_add_flag = 0;
	int func_found_flag;
	func_found_flag = 0;
	int return_flag;
	return_flag = 0;
	int i;
	i = 0;

	string* funcs;
	funcs = new string;

	while ((a = get_lex()) != "") {

		temp_lex = stream_of_token.table[i];

		if (main_flag == 1 && !((temp_lex.get_name() == "(") || (temp_lex.get_name() == ")") || (temp_lex.get_name() == "{")))
		{
			throw "Error";
		}
		switch (temp_lex.get_type())
		{
		case DELIMITERS:
		{
			if ((last_lex.get_type() == temp_lex.get_type())&&(skob_flag == 0)&&(temp_lex.get_name()!="}"))
				throw "Error";

			if (temp_lex.get_name() == "=")
			{
				if (last_lex.get_name() == ";")
					throw "Error";
				tree.tmp = tree.tmp->sons[flag_sons-1];
				tree.tmp->add_son(tree.tmp->l);
				tree.tmp->l = temp_lex;
				flag_return++;
				last_lex = temp_lex;
				flag_sons = 1;
				tree.tmp->def_sonc_c = 2;
				skob_flag = 0;
				dot_flag = 1;
			}
			else if (temp_lex.get_name() == "+")
			{
				if (last_lex.get_type() == KEYWORD)
					throw "Error";
				if (dot_flag == 0)
					throw "Error";
				tree.tmp = tree.tmp->sons[flag_sons-1];
				tree.tmp->add_son(tree.tmp->l);
				tree.tmp->l = temp_lex;
				flag_return++;
				last_lex = temp_lex;
				flag_sons = 1;
				tree.tmp->def_sonc_c = 2;
				skob_flag = 0;
			}
			else if (temp_lex.get_name() == "==")
			{
				if (last_lex.get_type() == KEYWORD)
					throw "Error";
				tree.tmp = tree.tmp->sons[flag_sons - 1];
				tree.tmp->add_son(tree.tmp->l);
				tree.tmp->l = temp_lex;
				flag_return++;
				last_lex = temp_lex;
				flag_sons = 1;
				tree.tmp->def_sonc_c = 2;
				skob_flag = 0;
			}
			else
			{
				if (temp_lex.get_name() == "(")
				{
					round_count++;
					if (skob_flag != 1)
						throw "Error";
					break;
				}
				if (temp_lex.get_name() == ")")
				{
					if (if_flag == 1)
					{
						if (last_lex.get_name() == "if")
							throw "Error";
					}
					if (func_flag == 1)
					{

						flag_temp = flag_return;
						for (int j = 0; j < flag_temp; j++)
						{
							tree.move_up();
							flag_return--;
						}
						flag_sons = tree.tmp->sons_c;
						lex_name = "null";
						last_lex.set_name(lex_name);
						dot_flag = 0;
						last_lex = temp_lex;
					}
					func_add_flag = 0;
					func_found_flag = 0;
					round_count--;
					if (round_count < 0)
						throw "Error";
					break;
				}
				if (temp_lex.get_name() == "[")
				{
					if (skob_flag != 1)
						throw "Error";
					square_count++;
					break;
				}
				if (temp_lex.get_name() == "]")
				{
					square_count--;
					if (square_count < 0)
						throw "Error";
					break;
				}
				if (temp_lex.get_name() == "{")
				{
					if (skob_flag != 1)
						throw "Error";
					figure_count++;
					main_flag = 0;
					break;
				}
				if (temp_lex.get_name() == "}")
				{
					figure_count--;
					if (figure_count < 0)
						throw "Error";
					break;
				}

				if (tree.tmp->def_sonc_c >=1)
					if (flag_sons < tree.tmp->def_sonc_c)
						if (func_found_flag == 0)
							throw "Error";
				flag_temp = flag_return;
				if (func_found_flag == 1)
				{
					flag_temp--;
					flag_return--;
				}

				for (int j = 0; j < flag_temp; j++)
				{
					tree.move_up();
					flag_return--;
				}
				flag_sons = tree.tmp->sons_c;
				lex_name = "null";
				last_lex.set_name(lex_name);
				dot_flag = 0;
				last_lex = temp_lex;
				if (func_found_flag == 1)
				{
					flag_temp++;
					flag_return++;
				}
			}
			break;
		}
		case KEYWORD:
		{
			if (func_found_flag == 1)
				throw "Error";
			if (temp_lex.get_name() == "main")
			{
				main_flag = 1;
				skob_flag = 1;
				tree.tmp->add_son(temp_lex);
				flag_sons++;
				tree.tmp->sons[flag_sons - 1]->def_sonc_c = 0;
			}

			if (temp_lex.get_name() == "integer")
			{
				tree.tmp->add_son(temp_lex);
				flag_sons++;
				tree.tmp->sons[flag_sons - 1]->def_sonc_c = 2;
				lex_name = "integer";
				tree.tmp->sons[flag_sons - 1]->l.set_name(lex_name);
				tree.tmp = tree.tmp->sons[flag_sons - 1];
				tree.tmp->add_son(tree.tmp->l);
				tree.tmp->l = temp_lex;
				lex_name = "var";
				tree.tmp->l.set_name(lex_name);
				flag_return++;
				last_lex = temp_lex;
				flag_sons = 1;
				tree.tmp->def_sonc_c = 2;
				if (func_flag == 1)
				{
					lex_name = "integer";
					func_table.add_parm(lex_name);
				}
			}

			if (temp_lex.get_name() == "bool")
			{
				tree.tmp->add_son(temp_lex);
				flag_sons++;
				tree.tmp->sons[flag_sons - 1]->def_sonc_c = 2;
				lex_name = "bool";
				tree.tmp->sons[flag_sons - 1]->l.set_name(lex_name);
				tree.tmp = tree.tmp->sons[flag_sons - 1];
				tree.tmp->add_son(tree.tmp->l);
				tree.tmp->l = temp_lex;
				lex_name = "var";
				tree.tmp->l.set_name(lex_name);
				flag_return++;
				last_lex = temp_lex;
				flag_sons = 1;
				tree.tmp->def_sonc_c = 2;
				if (func_flag == 1)
				{
					lex_name = "bool";
					func_table.add_parm(lex_name);
				}
			}

			if (temp_lex.get_name() == "string")
			{
				tree.tmp->add_son(temp_lex);
				flag_sons++;
				tree.tmp->sons[flag_sons - 1]->def_sonc_c = 2;
				lex_name = "string";
				tree.tmp->sons[flag_sons - 1]->l.set_name(lex_name);
				tree.tmp = tree.tmp->sons[flag_sons - 1];
				tree.tmp->add_son(tree.tmp->l);
				tree.tmp->l = temp_lex;
				lex_name = "var";
				tree.tmp->l.set_name(lex_name);
				flag_return++;
				last_lex = temp_lex;
				flag_sons = 1;
				tree.tmp->def_sonc_c = 2;
				if (func_flag == 1)
				{
					lex_name = "string";
					func_table.add_parm(lex_name);
				}
			}

			if (temp_lex.get_name() == "if")
			{
				if_flag = 1;
				if (tree.tmp->def_sonc_c >= 1)
					if (flag_sons >= tree.tmp->def_sonc_c)
						throw "Error";
				tree.tmp->add_son(temp_lex);
				flag_sons++;
				tree.tmp->sons[flag_sons - 1]->def_sonc_c = 1;
				skob_flag = 1;
				last_lex = temp_lex;
			}

			if (temp_lex.get_name() == "then")
			{
				if (if_flag != 1)
					throw "Error";

				flag_temp = flag_return;
				for (int j = 0; j < flag_temp; j++)
				{
					tree.move_up();
					flag_return--;
				}
				flag_sons = tree.tmp->sons_c;
				lex_name = "null";
				last_lex.set_name(lex_name);
				dot_flag = 0;
				last_lex = temp_lex;



				if_flag++;
				if (tree.tmp->def_sonc_c >= 1)
					if (flag_sons >= tree.tmp->def_sonc_c)
						throw "Error";
				tree.tmp->add_son(temp_lex);
				flag_sons++;
				tree.tmp->sons[flag_sons - 1]->def_sonc_c = 1;
				skob_flag = 1;
				last_lex = temp_lex;
			}

			if (temp_lex.get_name() == "else")
			{
				if (if_flag != 2)
					throw "Error";
				if ((figure_count % 2) != 1)
					throw "Error";

				flag_temp = flag_return;
				for (int j = 0; j < flag_temp; j++)
				{
					tree.move_up();
					flag_return--;
				}
				flag_sons = tree.tmp->sons_c;
				lex_name = "null";
				last_lex.set_name(lex_name);
				dot_flag = 0;
				last_lex = temp_lex;


				if_flag++;
				if (tree.tmp->def_sonc_c >= 1)
					if (flag_sons >= tree.tmp->def_sonc_c)
						throw "Error";
				tree.tmp->add_son(temp_lex);
				flag_sons++;
				tree.tmp->sons[flag_sons - 1]->def_sonc_c = 1;
				skob_flag = 1;
				last_lex = temp_lex;
			}

			if (temp_lex.get_name() == "endif")
			{
				flag_temp = flag_return;
				for (int j = 0; j < flag_temp; j++)
				{
					tree.move_up();
					flag_return--;
				}
				flag_sons = tree.tmp->sons_c;
				lex_name = "null";
				last_lex.set_name(lex_name);
				dot_flag = 0;
				last_lex = temp_lex;

				if (if_flag != 3)
					throw "Error";
				if_flag++;
				if (tree.tmp->def_sonc_c >= 1)
					if (flag_sons >= tree.tmp->def_sonc_c)
						throw "Error";
				tree.tmp->add_son(temp_lex);
				flag_sons++;
				tree.tmp->sons[flag_sons - 1]->def_sonc_c = 1;
				skob_flag = 1;
				last_lex = temp_lex;
			}

			if (temp_lex.get_name() == "FUNC")
			{
				func_flag = 1;
				flag_temp = flag_return;
				for (int j = 0; j < flag_temp; j++)
				{
					tree.move_up();
					flag_return--;
				}
				flag_sons = tree.tmp->sons_c;
				lex_name = "null";
				last_lex.set_name(lex_name);
				dot_flag = 0;
				last_lex = temp_lex;


				if (tree.tmp->def_sonc_c >= 1)
					if (flag_sons >= tree.tmp->def_sonc_c)
						throw "Error";
				tree.tmp->add_son(temp_lex);
				flag_sons++;
				tree.tmp->sons[flag_sons - 1]->def_sonc_c = 3;
				skob_flag = 1;
				last_lex = temp_lex;
			}

			if (temp_lex.get_name() == "ENDF")
			{
				if (func_flag != 1)
					throw "Error";
				func_flag++;
				flag_temp = flag_return;
				for (int j = 0; j < flag_temp; j++)
				{
					tree.move_up();
					flag_return--;
				}
				flag_sons = tree.tmp->sons_c;
				lex_name = "null";
				last_lex.set_name(lex_name);
				dot_flag = 0;
				last_lex = temp_lex;


				if (tree.tmp->def_sonc_c >= 1)
					if (flag_sons >= tree.tmp->def_sonc_c)
						throw "Error";
				tree.tmp->add_son(temp_lex);
				flag_sons++;
				tree.tmp->sons[flag_sons - 1]->def_sonc_c = 0;
				skob_flag = 1;
				last_lex = temp_lex;
			}
			if (temp_lex.get_name() == "true")
			{
				if (tree.tmp->def_sonc_c >= 1)
					if (flag_sons >= tree.tmp->def_sonc_c)
						throw "Error";
				if (last_lex.get_name() == ";")
					throw "Error";
				tree.tmp->add_son(temp_lex);
				flag_sons++;
				tree.tmp->sons[flag_sons - 1]->def_sonc_c = 0;
				skob_flag = 0;
				last_lex = temp_lex;
				break;
			}

			if (temp_lex.get_name() == "return")
			{
				tree.tmp = tree.tmp->sons[flag_sons - 1];
				tree.tmp->add_son(tree.tmp->l);
				tree.tmp->l = temp_lex;
				flag_return++;
				last_lex = temp_lex;
				flag_sons = 1;
				tree.tmp->def_sonc_c = -1;
				skob_flag = 0;
				return_flag = 1;
			}

			break;
		}
		case IDENTIFIER:
		{
			if (return_flag == 1)
			{
				tree.tmp->add_son(temp_lex);
				flag_sons++;
				tree.tmp->sons[flag_sons - 1]->def_sonc_c = -1;
				if (func_flag != 1)
					skob_flag = 0;
				return_flag = 0;
				break;
			}

			if (func_found_flag == 1)
			{
				tree.tmp = tree.tmp->sons[flag_sons - 1];
				flag_return++;
				lex_name = temp_lex.get_name();
				tree.tmp->l.set_name(lex_name);
				last_lex = temp_lex;
				flag_sons = 1;
				tree.tmp->def_sonc_c = -1;
				skob_flag = 0;
				break;
			}
			if (tree.tmp->def_sonc_c >= 1)
				if (flag_sons >= tree.tmp->def_sonc_c)
					throw "Error";
			tree.tmp->add_son(temp_lex);
			flag_sons++;
			tree.tmp->sons[flag_sons - 1]->def_sonc_c = -1;
			if (func_flag != 1)
				skob_flag = 0;

			if ((func_flag == 1)&&(last_lex.get_name() == "FUNC"))
			{
				func_add_flag = 1;
				func_table.id = temp_lex.get_name();
			}
			else if (temp_lex.get_name() == func_table.id)
			{
				func_found_flag = 1;
				skob_flag = 1;
			}
			last_lex = temp_lex;
			break;
		}
		case CONSTANT:
		{
			if (tree.tmp->def_sonc_c >= 1)
				if (flag_sons >= tree.tmp->def_sonc_c)
					throw "Error";
			if (last_lex.get_name() == ";")
				throw "Error";
			tree.tmp->add_son(temp_lex);
			flag_sons++;
			tree.tmp->sons[flag_sons - 1]->def_sonc_c = 0;
			skob_flag = 0;
			last_lex = temp_lex;
			break;
		}
		default:
			if (tree.tmp->def_sonc_c >= 1)
				if (flag_sons >= tree.tmp->def_sonc_c)
				throw "Error";
			tree.tmp->add_son(temp_lex);
			flag_sons++;
			tree.tmp->sons[flag_sons-1]->def_sonc_c = -1;
			skob_flag = 0;
			last_lex = temp_lex;
			break;
		}
		
		i++;
		cout << temp_lex.get_name() << endl;
	} 
	cout << "sons" << endl;
	tree.show();
	if (square_count > 0 || round_count > 0 || figure_count > 0)
		throw "Error";
}

void Scaner::print_stream_of_lex()
{
	string type;
	cout << setw(10) << "№" << setw(20) << "Name of token" << setw(20) << " Type" << endl;
	for (int i = 0; i < stream_of_token.get_count() - 1; i++)
	{
		switch (stream_of_token[i].get_type())
		{
		case KEYWORD:
			type = "KEYWORD";
			break;
		case DELIMITERS:
			type = "DIVIDER";
			break;
		case IDENTIFIER:
			type = "IDENTIFIER";
			break;
		case CONSTANT:
			type = "CONSTANT";
			break;
		case ERROR:
			//type = ERROR;
			cout << "ОШИБКА, невозможно обрабоать лексему " << endl;
			system("pause");
			exit(0);
			break;
		}
		cout << setw(10) << i << setw(20) << stream_of_token[i].get_name() << setw(20) << type << endl;
	}
}

void Scaner::print_another_table()
{
	int i = 0;
	cout << endl << endl;
	
	cout << setw(10) << "№" << setw(20) << "Name of KeyWord" << endl;
	for (auto a : table_of_keyWords.get_vect_key_word())
	{
		cout << setw(10) << i << setw(20) << a << endl;
		i++;
	}

	cout << endl << endl;
	
	cout << setw(10) << "№" << setw(20) << "Name of Delimiters" << endl;
	for (auto a : table_of_delimiters.get_vect_key_word())
	{
		cout << setw(10) << i << setw(20) << a << endl;
		i++;
	}

	cout << endl << endl;
	
	cout << setw(10) << "№" << setw(20) << "Value of constant" << endl;
	for (i = 0; i < table_of_constant.get_count(); i++)
		cout << setw(10) << i << setw(20) << table_of_constant[i].get_name() << endl;

	cout << endl << endl;
	
	cout << setw(10) << "№" << setw(20) << "Name of Identifier" << endl;
	for (i = 0; i < table_of_identifier.get_count(); i++)
		cout << setw(10) << i << setw(20) << table_of_identifier[i].get_name() << endl;
	cout << endl << endl;
}

void Scaner::sintaxys()
{
	cout << "A это не так работает))\n";

}