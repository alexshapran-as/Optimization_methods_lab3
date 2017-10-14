#ifndef INTPROG_H_
#define INTPROG_H_
#include <vector>
#include <string>
double int_solution();
double int_solution2();
bool is_integer(double);
void set_new_data();
void set_new_data2();
void tree();
static unsigned int count = {0};
static unsigned int count2 = {0};
static std::map<std::string, std::vector<double>> nodes;
#endif
