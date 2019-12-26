#include "delim.h"

Delimiters::Delimiters()
{

	string str;
	try
	{
		ifstream in_KW;
		in_KW.open("delimiters.txt", ios::in);

		if (!in_KW.is_open())
			throw in_KW;

		while (!in_KW.eof())
		{
			in_KW >> str;
			if (!str.empty())
				delimiters.push_back(str);

		}
		in_KW.close();
	}
	catch (ifstream& i)
	{
		cout << "Error opening file delimiters.txt" << endl;
		system("pause");
		system("cls");
	}

}

bool Delimiters::operator==(string& obg)
{
	for (auto var : delimiters)
	{
		if (obg == var)
			return true;
	}

	return false;
}