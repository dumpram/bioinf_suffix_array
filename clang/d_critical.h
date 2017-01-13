#ifndef D_CRITICAL_H_INCLUDED
#define D_CRITICAL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void find_lms_characters(int* s, int N, bool* LMS_ch, bool* LS);
int find_d_critical_characters(bool* LMS_ch, int N, int d, bool* d_ch, int* P1);
void bucket_sort_LS(int* a, int* b, int d, int n1, int N, int* s, bool* t);
void calculate_Sw(int* s, int* Sw, int N, bool* t);
void bucket_sort(int* a, int* b, int* s, int n1, int N, int alphabetSize, int d);

#endif // D_CRITICAL_H_INCLUDED
