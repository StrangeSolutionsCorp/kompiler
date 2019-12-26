#pragma once
#include "lex.h"

template <typename T>
class Table
{
public:
	vector <T> table;
	int count;

	Table();

	int get_count() { return count; };

	Table <T>& operator +=(T& obg);
	T& operator [](int num);
	bool operator ==(string& obg);

};

template<typename T>
inline Table<T>::Table()
{
	count = 0;
}

template<typename T>
inline Table<T>& Table<T>::operator+=(T& obg)
{
	table.push_back(obg);
	count++;

	return *this;
}

template<typename T>
inline bool Table<T>::operator==(string& obg)
{
	for (auto var : table)
	{
		if (obg == var.get_name())
			return true;
	}

	return false;
}

template<typename T>
inline T& Table<T>::operator[](int num)
{
	return table[num];
}
