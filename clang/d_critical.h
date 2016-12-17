#ifndef D_CRITICAL_H_INCLUDED
#define D_CRITICAL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void find_lms_characters(char* s, int N, bool* LMS_ch);
void find_d_critical_characters(bool* LMS_ch, int N, int d, bool* d_ch, int* a);

#endif // D_CRITICAL_H_INCLUDED
