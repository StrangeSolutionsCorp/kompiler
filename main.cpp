#include "analyzer.h"

int main()
{
	Scaner A;

	Element_Sy B;

	try
	{
		A.to_scan();
		A.print_stream_of_lex();
		A.print_another_table();
		A.sintaxys();
	}
	catch (const char[])
	{
		cout << "error" << endl;
	}
	system("pause");

	return 0;
}