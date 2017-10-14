#include <iostream>
#include "simplex.h"
#include "intprog.h"
#include <vector>
#include <string>

double x1 = {0.0};
double x2 = {0.0};
double x3 = {0.0};
double f = {0.0};
double x1_2 = {0.0};
double x2_2 = {0.0};
double x3_2 = {0.0};
double f_2 = {0.0};
std::vector<double> answers; // Промежуточные ответы x1, x2, x3

int main()
{
	using std::cout;
	using std::cin;
	using std::endl;
	using std::vector;
	system("chcp 65001");

	unsigned int node = {0}; // Переменная для определения по какой ветви дерева идет расчет 
	vector<double> c;
	vector<double> b;
	double** a;
	unsigned int n = {0}; // количество столбцов матрицы A
	unsigned int m = {0}; // Количество строк матрицы A
	simplex_method(c,b,set_data(c,b,a,n,m,node),node); // Решаем задачу ЛП с помощью симплекс метода 

	while (int_solution() != -1)
	{
		set_new_data();
		m = n = 0;
		c.clear();
		b.clear();
		simplex_method(c,b,set_data(c,b,a,n,m,node),node);
	}
	node = 1;
	unsigned int i = {1};
	x1_2 = answers[i];
	x2_2 = answers[++i];
	x3_2 = answers[++i];
	while (int_solution2() != -1)
	{
		set_new_data2();
		m = n = 0;
		c.clear();
		b.clear();
		simplex_method(c,b,set_data(c,b,a,n,m,node),node);
		i++;
		x1_2 = answers[++i];
		x2_2 = answers[++i];
		x3_2 = answers[++i];
	}
	cout << endl;
	tree();
	cout << endl;
	system("pause");
	return 0;
}

