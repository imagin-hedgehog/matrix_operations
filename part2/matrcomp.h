#ifndef MATRCOMP_H
#define MATRCOMP_H

#include "mytype.h" // нужно для типа матриц

void mult(t_item* A, t_item* B, int rowA, int colB, int cArB, t_item* C);
void sum(t_item* A, t_item* B, int rowC, int colC, t_item* C);
t_item det(t_item* m, int ROWS, int COLS);
int rank(t_item* m, int ROWS, int COLS);

#endif