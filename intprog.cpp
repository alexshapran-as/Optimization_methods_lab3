#include <iostream>
#include "simplex.h"
#include "intprog.h"
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <iomanip>

extern double x1;
extern double x2;
extern double x3;
extern double f;
extern double x1_2;
extern double x2_2;
extern double x3_2;
extern double f_2;
extern std::vector<double> answers;

double int_solution()
{
	if (int(x1) != double(int(x1*1000))/1000) // Проверка, являтся ли число целым
		return x1;
	else if (int(x2) != double(int(x2*1000))/1000)
		return x2;
	else if (int(x3) != double(int(x3*1000))/1000)
		return x3;
	return -1; 
}

double int_solution2()
{
	if (int(x1_2) != double(int(x1_2*1000))/1000)
		return x1_2;
	else if (int(x2_2) != double(int(x2_2*1000))/1000)
		return x2_2;
	else if (double(int(x3_2)) != double(int(x3_2*1000))/1000)
		return x3_2;
	return -1; 
}

bool is_integer(double arg)
{
	return int(arg) == double(int(arg*1000))/1000;
}

void set_new_data()
{
	using std::cout;
	using std::endl;
	using std::vector;
	using std::string;

	std::ifstream fin("lab3.txt");
	if (!fin.is_open())
	{
		cout << endl << "Ошибка открытия файла.";
		exit(EXIT_FAILURE);
	}
	int _x = int(int_solution()); // _x = [x*]
	vector<double> temp;
	double d_temp = {0.0};
	string s_temp;
	fin >> s_temp;
	for (unsigned int i = {0}; i < 17 + count + 3*count; ++i)
	{
		if (fin >> d_temp)
		{
			temp.push_back(d_temp);
		}
		else
		{	
			fin.clear();
			while (fin.get() != '\n');
		}
	}
	fin.close();
	temp.insert(temp.begin()+7+count,_x); // Задаем новые данные для составления Симлекс-таблицы
	if (int_solution() == x1)
	{
		temp.push_back(1);
		temp.push_back(0);
		temp.push_back(0);
	}
	if (int_solution() == x2)
	{
		temp.push_back(0);
		temp.push_back(1);
		temp.push_back(0);
	}
	if (int_solution() == x3)
	{
		temp.push_back(0);
		temp.push_back(0);
		temp.push_back(1);
	}
	std::ofstream fout("lab3.txt");
	if (!fout.is_open())
	{
		cout << endl << "Ошибка открытия файла.";
		exit(EXIT_FAILURE);
	}
	fout << s_temp << endl; // Переносим новые данные в файл
	for (unsigned int i = {0}; i < 3; ++i)
	{
		fout << temp[i] << endl;
	}
	fout << "q" << endl;
	for (unsigned int i = {3}; i < 7 + count; ++i)
	{
		fout << temp[i] << endl;
	}
	fout << "q" << endl;
	for (unsigned int i = {7+count}; i < 19 + count + 3*count; ++i)
	{
		fout << temp[i] << endl;
	}
	fout.close();
	
	count++; // Количество дополнительных условий, образовавшихся из-за нецелого решения
}

void set_new_data2()
{
	using std::cout;
	using std::endl;
	using std::vector;
	using std::string;

	std::ifstream fin("lab3_2.txt");
	if (!fin.is_open())
	{
		cout << endl << "Ошибка открытия файла.";
		exit(EXIT_FAILURE);
	}
	int _x = -(int(int_solution2()) + 1); // _x = [x*] + 1 (знак - чтобы получить нер-во со знаком <=)
	vector<double> temp;
	double d_temp = {0.0};
	string s_temp;
	fin >> s_temp;
	for (unsigned int i = {0}; i < 17 + count2 + 3*count2; ++i)
	{
		if (fin >> d_temp)
		{
			temp.push_back(d_temp);
		}
		else
		{	
			fin.clear();
			while (fin.get() != '\n');
		}
	}
	fin.close();
	temp.insert(temp.begin()+6+count2,_x);
	if (int_solution2() == x1_2)
	{
		temp.push_back(-1); // Меняем знак, чтобы получить нер-во со знаком <=
		temp.push_back(0);
		temp.push_back(0);
	}
	if (int_solution2() == x2_2)
	{
		temp[5+count2] = -(_x+1);
		unsigned int i = {5};
		temp[temp.size()-3] = 1;
		temp[temp.size()-2] = 0;
		temp[temp.size()-1] = 0;
		temp.push_back(0);
		temp.push_back(-1);
		temp.push_back(0);
	}
	if (int_solution2() == x3_2)
	{
		temp[5+count2] = -(_x+1);
		unsigned int i = {9};
		temp[temp.size()-3] = 0;
		temp[temp.size()-2] = 1;
		temp[temp.size()-1] = 0;
		temp.push_back(0);
		temp.push_back(0);
		temp.push_back(-1);
	}

	std::ofstream fout("lab3_2.txt");
	if (!fout.is_open())
	{
		cout << endl << "Ошибка открытия файла.";
		exit(EXIT_FAILURE);
	}
	fout << s_temp << endl;
	for (unsigned int i = {0}; i < 3; ++i)
	{
		fout << temp[i] << endl;
	}
	fout << "q" << endl;
	for (unsigned int i = {3}; i < 7 + count2; ++i)
	{
		fout << temp[i] << endl;
	}
	fout << "q" << endl;
	for (unsigned int i = {7+count2}; i < 19 + count2 + 3*count2; ++i)
	{
		fout << temp[i] << endl;
	}
	fout.close();
	
	count2++;
}

void tree()
{
	using std::cout;
	using std::endl;

	cout << endl << std::setw(24)  << "Двоичное дерево решений" << endl << endl;

	cout << std::setw(24) << "F = " << answers[0] << endl 
	<< std::setw(25) << "X1 = " << answers[1] << endl << std::setw(25) 
	<< "X2 = " << answers[2] << endl 
	<< std::setw(25) << "X3 = " << answers[3] << endl << std::setw(20) 
	<< "/" << std::setw(11) << "\\";

	cout << endl << std::setw(14) << "F = " << answers[4] << std::setw(13) 
	<< "F = " << answers[16] << endl << std::setw(15) 
	<< "X1 = " << answers[5] << std::setw(15) << "X1 = " << answers[17] << endl 
	<< std::setw(15) << "X2 = " << answers[6]  << std::setw(15) << "X2 = " << answers[18] 
	<< endl << std::setw(15) << "X3 = " << answers[7] << std::setw(15) << "X3 = " << answers[19];

	for (unsigned int i = {0}; i < 2; ++i)
	{
		if (is_integer(answers[17+4*i]) &&  is_integer(answers[18+4*i]) && is_integer(answers[19+4*i]))
		{
			cout << endl << std::setw(10-3*i) << "/" << std::setw(10-i) << "\\";
			cout << endl << std::setw(10-3*i) << "F = " << answers[8+4*i];
			if (answers[20+4*i] < 0)
				cout << std::setw(15-3*i) << "     Нет решений";
			else
				cout << std::setw(10-3*i) << "F = " << answers[20+4*i];
			cout << endl << std::setw(11-3*i) << "X1 = " << answers[9+4*i];
			if (answers[21+4*i] < 0)
				cout << std::setw(15-3*i) <<  " ";
			else
				cout << std::setw(11-3*i) <<  "X1 = " << answers[21+4*i];
			cout << endl << std::setw(11-3*i) << "X2 = " << answers[10+4*i];
			if (answers[22+4*i] < 0)  
				cout << std::setw(15-3*i) << " ";
			else
				cout << std::setw(11-3*i) <<  "X2 = " << answers[22+4*i];
			cout << endl << std::setw(11-3*i) << "X3 = " << answers[11+4*i];
			if (answers[23+4*i] < 0) 
				cout << std::setw(15-3*i) << " "; 
			else
				cout << std::setw(11-3*i) <<  "X3 = " << answers[23+4*i];
		}
	}
	
	cout << endl << endl << "Оптимальное решение исходной задачи ЦЛП:" << endl;
	double biggest = {0.0};
	for (unsigned int i = {0}; i < 6; ++i)
		biggest = (answers[4+4*i] > biggest) && is_integer(answers[4+4*i]) ? answers[4+4*i] : biggest;

	for (unsigned int i = {0}; i < answers.size(); ++i)
	{
		if (answers[i] == biggest)
		{
			cout << "F = " << biggest << endl << "X1 = " << answers[i+1] << endl 
			<< "X2 = " << answers[i+2] << endl << "X3 = " << answers[i+3];
		}
	}
}