#ifndef SIMPLEX_H_
#define SIMPLEX_H_
#include <vector>
#include <string>
#include <iostream>
void show_task(std::vector<double> c, std::vector<double> b, double** a);
double** set_data(std::vector<double> &c, std::vector<double> &b, double** a, unsigned int &n, unsigned int &m, unsigned int node);
double * simplex_method(std::vector<double> c, std::vector<double> b, double ** a, unsigned int node);
bool not_reference(double ** arr, unsigned int m);
bool not_optimal(double ** arr, unsigned int m, unsigned int n);
std::vector<std::string> print(double ** arr, unsigned int m, unsigned int n);
static unsigned int ri = {0}; // Разрешающая строка
static unsigned int rj = {0}; // Разрешающий столбец
static std::string min_max; // Ищем min/max
static unsigned int count_of_call = {0};
#endif