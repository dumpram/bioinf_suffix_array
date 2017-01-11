#ifndef D_CRITICAL_H_INCLUDED
#define D_CRITICAL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


void find_lms_characters(int* s, int N, bool* LMS_ch, bool* LS);
void find_d_critical_characters(bool* LMS_ch, int N, int d, bool* d_ch, int* a);
void bucket_sort(int* P1, int d, int n1, int* S1);
void calculate_Sw(int* s, int* Sw, int N, bool* t);

#endif // D_CRITICAL_H_INCLUDED
