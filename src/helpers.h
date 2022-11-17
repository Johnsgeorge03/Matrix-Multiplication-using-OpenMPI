#ifndef HELPERS_H
#define HELPERS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <omp.h>
void gen_rand_mat(double*, int, int, int);

bool is_correct(double*, double*, int);

void print_mat(double*, int, int);

void serial_multiply(double*, double*, double*, int, int, int);

void write_mat(double*, char, int, int);
#endif
