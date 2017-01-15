#ifndef D_CRITICAL_H_INCLUDED
#define D_CRITICAL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void define_LS_type(int *s, int N, bool *LS);
int define_d_crit_ch(bool *LS, int N, int d, int *P1);
void bucket_sort_LS(int *a, int *b, int d, int n1, int N, int *s, bool *t);
void bucket_sort(int *a, int *b, int *s, int n1, int N, int alphabetSize, int d);

#endif // D_CRITICAL_H_INCLUDED
