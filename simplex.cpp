#include <iostream>
#include <fstream>
#include "simplex.h"
#include "intprog.h"
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <iomanip>
#include <windows.h>

extern double x1;
extern double x2;
extern double x3;
extern double f;
extern double x1_2;
extern double x2_2;
extern double x3_2;
extern double f_2;
extern std::vector<double> answers;

double** set_data(std::vector<double> &c, std::vector<double> &b, double** a, unsigned int &n, unsigned int &m, unsigned int node)
{
	using std::cout;
	using std::endl;
	using std::vector;

	std::ifstream fin;
	double temp = {0.0};
	if (node == 0)
		fin.open("lab3.txt");
	else if (node == 1)
		fin.open("lab3_2.txt");
	if (!fin.is_open())
	{
		cout << endl << "Ошибка открытия файла.";
		exit(EXIT_FAILURE);
	}
	fin >> min_max;
	if (min_max != "min" && min_max != "max")
	{
		cout << endl << "Неправильно указано, к чему стремится ЦФ F.";
		exit(EXIT_FAILURE);
	}
	while (fin >> temp)
	{
		if (min_max == "min")
			c.push_back(temp);
		else if (min_max == "max")
			c.push_back(-temp);
		n++;
	}
	fin.clear();
	while (fin.get() != '\n');
	while (fin >> temp)
	{
		b.push_back(temp);
		m++;
	}
	fin.clear();
	while (fin.get() != '\n');

	if (m > 2*n)
	{
		cout << endl << "Число линейно независимых уравнений больше числа переменных."
		" Такая СЛАУ несовместна.";
		exit(EXIT_FAILURE);
	}

	a = new double*[m];
	for (unsigned int i = {0}; i < m; ++i)
		a[i] = new double [n];
	for (unsigned int i = {0}; i < m; ++i)
	{
		for (unsigned int j = {0}; j < n; ++j)
		{
			if (!(fin >> a[i][j]))
			{
				cout << endl << "Не удалось считать данные для вектора A = [a1,..,an]!";
				exit(EXIT_FAILURE);
			}
		}
	}
	fin.close();
	show_task(c,b,a);
	return a;
}

void show_task(std::vector<double> c, std::vector<double> b, double** a)
{
	using std::cout;
	using std::cin;
	using std::endl;

	cout << endl << "Постановка задачи ЦЛП. Требуется найти решение следующей задачи:" 
	<< endl << std::setw(26) << "F = cx -> " <<  min_max << endl 
	<< std::setw(25) << "Ax <= b" << endl << std::setw(25) << "x >= 0, x ";
	SetConsoleCP(866);
	SetConsoleOutputCP(866);
	cout << char(243)  << " Z";
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);
	cout << endl << "Каноническая форма исходной задачи ЛП:" << endl;
	if (min_max == "min")
		cout << std::setw(17) << "F = ";
	else
		cout << std::setw(17) << "-F = ";
	for (unsigned int i = {0}; i < c.size(); ++i)
	{
		if (c[i] < 0)
			cout << c[i] << "x" << i+1;
		else
			cout << "+" << c[i] << "x" << i+1;
	}
	cout << " -> ";
	if (min_max == "min")
		cout <<  min_max << endl << endl; 
	else
		cout <<  "min" << endl << endl; 
	unsigned int k = c.size() + 1;
	for (unsigned int i = {0}; i < b.size(); ++i)
	{
		cout << "                 ";
		for (unsigned int j = {0}; j < c.size(); ++j)
		{
			if (a[i][j] != 0)
			{
				if (a[i][j] != 1 && a[i][j] != -1)
				{
					if (a[i][j] < 0 || j == 0)
						cout << a[i][j] << "x" << j+1;
					else
						if (a[i][j-1] != 0)
							cout << "+" << a[i][j] << "x" << j+1;
						else 
							cout << a[i][j] << "x" << j+1;
				}
				else
				{
					if (a[i][j] < 0)
						cout << "-x" << j+1;
					else if (j != 0)
						cout << "+" << "x" << j+1;
					else 
						cout << "x" << j+1;
				}
		
			}		
		}
		cout << "+x" << k++ << " = " << b[i] << endl;
	}
	for (unsigned int j = {0}; j < 2; j++)
	{
		cout << endl << "             ";
		for (unsigned int i = {0}; i < c.size()+b.size(); ++i)
		{
			if (i != c.size()+b.size()-1)
				cout << "x" << i+1 << ", ";
			else
				cout << "x" << i+1;
		}
		if (j == 0)
		{
			SetConsoleCP(866);
			SetConsoleOutputCP(866);
			cout << " " << char(243)  << " Z";
			SetConsoleCP(65001);
			SetConsoleOutputCP(65001);
		}
		else if (j == 1)
		{
			cout << " >= 0" << endl;
		}
	}
	
	cout << endl << "Алгоритм преобразования симплекс-таблицы (жордановы исключения):";
}

double * simplex_method(std::vector<double> c, std::vector<double> b, double** a, unsigned int node)
{
	using std::cout;
	using std::endl;
	using std::vector;
	using std::string;

	count_of_call = 0;
	vector<string> x(b.size() + c.size() + 1);

	double ** arr = new double*[b.size()+1];  // Двумерный массив - симплекс матрица
	for (unsigned int i = {0}; i < b.size()+1; ++i)
		arr[i] = new double[c.size()+1];
	for (unsigned int i = {0}; i < b.size(); ++i)
		arr[i][0] = b[i]; // Заполняем первый столбец свободных членов
	arr[b.size()][0] = {0}; // Свободный элемент в функции F
	cout << endl;

	for (unsigned int i = {0}; i < b.size(); ++i)
	{
		unsigned int k = {1};
		for (unsigned int j = {0}; j < c.size(); ++j)
		{
			arr[i][k] = a[i][j]; // Заполняем все остальные элементы, начиная со второго столбца, без последней строки
			k++;
		}
	} 

	for (unsigned int i = {0}; i < b.size(); ++i)
			delete [] a[i];
	delete [] a;

	unsigned int k = {1};
	for (unsigned  int i = {0}; i < c.size(); ++i)
	{
		arr[b.size()][k] = -c[i]; // Заполняем последнюю строку со второго столбца коэфф-ми при x в ф-ии F
		k++;
	}

	x = print(arr, b.size(), c.size());

	while (not_reference(arr,b.size()))
	{
		cout << endl << "Недопустимое решение. Производим замену базиса," 
		" чтобы получить опорное решение.";
		unsigned int i = {0};
		while (arr[i][0] > 0 && i < b.size() + 1)
			i++;
		unsigned int j = {1}; // Разрешающий столбец
		unsigned int k = {0};
		for (; j < c.size() + 1; ++j)
		{
			if (arr[i][j] < 0)
			{
				k++;
				break;
			}
		}
		if (k == 0)
		{
			cout << endl << "Задача не имеет допустимых решений!";
			answers.push_back(-1); // -1 указывает на то, что решений нет
			answers.push_back(-1);
			answers.push_back(-1);
			answers.push_back(-1);
			return nullptr;
		}
		vector<double> temp;
		for (unsigned int i = {0}; i < b.size(); ++i)
		{
			if ((arr[i][0] != 0 || arr[i][j] > 0) && arr[i][j] != 0)
				temp.push_back(arr[i][0] / arr[i][j]); // Заполняем временный вектор temp отношениями свободных членов к элементам разрешающего столбца
		}
		std::sort(temp.begin(),temp.end()); // Сортируем эти отношения
		unsigned int m = {0};
		while (temp[m] < 0)
			m++; // Ищем номер первого положительного элемента в отсортированном векторе temp 
		i = {0}; // Разрешающая строка
		for (; temp[m] != arr[i][0] / arr[i][j]; ++i);
		for (unsigned int m = {0}; m < b.size() + 1; ++m)
		{
			for (unsigned int n = {0}; n < c.size() + 1; ++n)
			{
				if (m != i && n != j) // Преобразование эл-та, который не стоит ни в разрешающей строке, ни в разрешающем столбце
				{
					arr[m][n] = arr[m][n] - (arr[m][j] * arr[i][n]) / (arr[i][j]);
				}
			}
		}
		for (unsigned int m = {0}; m < b.size() + 1; ++m)
		{
			for (unsigned int n = {0}; n < c.size() + 1; ++n)
			{
				if (m != i && n == j) // Преобразование эл-та, который стоит в разрешающем столбце, но не в разрешающей строке
				{
					if (arr[m][n] != 0)
						arr[m][n] = -arr[m][n] / arr[i][j];
				}
				if (m == i && n != j) // Преобразование эл-тва, который стоит в разрешающей строке, но не в разрешающем столбце
				{
					arr[m][n] = arr[m][n] / arr[i][j];
				}
			}
		}
		arr[i][j] = 1.0 / arr[i][j]; // Преобразование разрешающего эл-та
		ri = i; 
		rj = j;
		x = print(arr, b.size(), c.size());
	}

	cout << endl << "Найдено опорное решение. Проводим проверку на оптимальность.";
	while (not_optimal(arr,b.size(),c.size()))
	{
		cout << endl << "Неоптимальное решение. Производим замену базиса.";
		unsigned int j = {0}; // Новый разрешающий столбец
		for (; j < c.size() + 1; ++j)
		{
			if (arr[b.size()][j] > 0) // Ищем разрешающий столбец
				break;
		}
		vector<double> temp;
		for (unsigned int i = {0}; i < b.size(); ++i)
		{
			if (arr[i][0] != 0 || arr[i][j] > 0)
				temp.push_back(arr[i][0] / arr[i][j]); 
		}
		std::sort(temp.begin(),temp.end());
		unsigned int m = {0};
		while (temp[m] < 0) // Аналогично ищем номер первого положительного отношения
			m++;
		unsigned int i = {0}; // Новая разрешающая строка
		if (arr[ri][rj] == 0)
		{
			cout << endl << "Деление на 0 недопустимо!";
			exit(EXIT_FAILURE);
		}
		for (; temp[m] != arr[i][0] / arr[i][j]; ++i);
		for (unsigned int m = {0}; m < b.size() + 1; ++m)
		{
			for (unsigned int n = {0}; n < c.size() + 1; ++n)
			{
				if (m != i && n != j)
				{
					arr[m][n] = arr[m][n] - (arr[m][j] * arr[i][n]) / (arr[i][j]);
				}
			}
		}
		for (unsigned int m = {0}; m < b.size() + 1; ++m)
		{
			for (unsigned int n = {0}; n < c.size() + 1; ++n)
			{
				if (m != i && n == j)
				{
					if (arr[m][n] != 0)
						arr[m][n] = -arr[m][n] / arr[i][j];
				}
				if (m == i && n != j)
				{
					arr[m][n] = arr[m][n] / arr[i][j];
				}
			}
		}
		arr[i][j] = 1.0 / arr[i][j];
		ri = i;
		rj = j;
		x = print(arr, b.size(), c.size()); // Вектор строк, в котором первыми идут свободные переменные, а затем идут базисные переменные
	}
	cout << endl << "Найдено оптимальное решение!" << endl;
	for (unsigned int i = {0}; i < c.size(); ++i)
		cout << x[i] << " = 0; ";
	unsigned int l = {0};
	for (unsigned int j = c.size(); j < b.size()+c.size(); ++j)
	{
		cout << endl << x[j] << " = " << arr[l][0] << ";";
		if (x[j] == "X1")
		{
			if (node == 0)
				x1 = arr[l][0];
			else if (node == 1)
				x1_2 = arr[l][0];
		}
		else if (x[j] == "X2")
		{
			if (node == 0)
				x2 = arr[l][0];
			else if (node == 1)
				x2_2 = arr[l][0];
		}
		else if (x[j] == "X3")
		{
			if (node == 0)
				x3 = arr[l][0];
			else if (node == 1)
				x3_2 = arr[l][0];
		}
		l++;
	}

	cout << endl;
	if (min_max == "min") 
	{
		cout << "F = " << arr[b.size()][0]; 
		f = arr[b.size()][0]; 
	}
	else if (min_max == "max")
	{
		cout << "F = " << -arr[b.size()][0]; // Инвертируем знак F, так как ищем максимум
		f = -arr[b.size()][0]; 
	}

	answers.push_back(f);
	if (node == 0)
	{
		answers.push_back(x1);
		answers.push_back(x2);
		answers.push_back(x3);
	}
	else if (node == 1)
	{
		answers.push_back(x1_2);
		answers.push_back(x2_2);
		answers.push_back(x3_2);
	}
	

	for (unsigned int i = {0}; i < b.size() + 1; ++i)
		delete [] arr[i];
	delete [] arr;
}

bool not_reference(double ** arr, unsigned int m)
{
	unsigned int accept = {0};
	for (unsigned int i = {0}; i < m; ++i)
	{
		if (arr[i][0] < 0) // Проверка на допустимое решение
			accept++;
	}
	if (accept > 0)
		return true;
	else
		return false;
}

bool not_optimal(double ** arr, unsigned int m, unsigned int n)
{
	unsigned int check = {0};
	for (unsigned int j = {1}; j < n + 1; ++j)
	{
		if (arr[m][j] > 0) // Проверка на оптимальное решение
			check++;
	}
	if (check > 0)
		return true;
	else 
		return false;
}

std::vector<std::string> print(double ** arr, unsigned int m, unsigned int n)
{
	using std::cout;
	using std::endl;
	using std::vector;
	using std::string;

	cout << endl << "Симплекс-таблица:" << endl << std::setw(9) << "Si0";
	static vector<string> x(m+n+1); // Вектор Xi и F
	if (!x.empty())
	{
		x.resize(m+n+1+count);
	}
	//cout << "count_of_call = " << count_of_call;
	if (count_of_call == 0)
	{
		for (unsigned int i = {0}; i < x.size()-1; ++i)
		{
			char temp[24]; 
			itoa(i+1,temp,10); // Переводим номер Икса в строку
			x[i] = "X";
			x[i] += temp;
		}
		x[x.size()-1] = "F ";
	}
	if (count_of_call > 0)
	{
		for (unsigned int i = {0}; i < n; ++i)
		{
			if (i == rj - 1)
			{
				for (unsigned int j = {0}; j < m+n; ++j)
				{
					if (j == ri)
					{
						string temp = x[i];
						x[i] = x[j+n];
						x[j+n] = temp; // Меняем X-ы, стоящие в разрешающей строке и столбце
					}
				}
			}
		}
	}
	count_of_call++;
	
	unsigned int k = {0};
	for (; k < n; ++k)
	{
		cout << std::setw(7) << x[k];
	}
	cout << endl;
	for (unsigned int i = {0}; i < m+1; ++i)
	{
		cout << x[k++];
		for (unsigned int j = {0}; j < n+1; ++j)
		{
			cout << std::setw(7) << std::fixed << std::setprecision(2) << arr[i][j];
		}
		cout << endl;
	}
	return x;
}
