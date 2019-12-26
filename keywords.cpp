#include "keywords.h"

Keywords::Keywords()
{
	//Чтение ключевых слов 
	string str;
	try
	{
		ifstream in_KW;
		in_KW.open("keywords.txt", ios::in);

		if (!in_KW.is_open())
			throw in_KW;

		while (!in_KW.eof())
		{
			in_KW >> str;
			if (!str.empty())
				keyWords.push_back(str);

		}
		in_KW.close();
	}
	catch (ifstream& i)
	{
		cout << "Error opening file keywords.txt" << endl;
		system("pause");
		system("cls");
	}

}

bool Keywords::operator==(string& obg)
{
	for (auto var : keyWords)
	{
		if (obg == var)
			return true;
	}

	return false;
}