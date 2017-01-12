#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <d_critical.h>

void find_lms(char* s, int N, bool* LMS);

int main()
{
FILE *fp;
int n, N, i;

//opening file and saving characters into an array
    if(!(fp = fopen("test.txt", "r+")))
    {
        printf("Nemrem otprt file");
    }
    if(fseek(fp,0,SEEK_END) != 0)
    {
        printf("Ne mogu doci do kraja file-a");
    }
    n = ftell(fp);
    printf("%d\n", n);

//char s[n];
int *s = (int*) malloc(sizeof(int)*n);
char c;
int z=0;

     if(fseek(fp,0,SEEK_SET) != 0)
    {
        printf("Ne mogu doci do pocetka file-a");
    }
    for(i=0; i<n; i++)
    {
        if( fread(&c, sizeof(char), 1, fp) != 1)
        {
            z=1;
            break;
        }
        if(c != '\n') s[i]=(int)c;
        else i--;
        printf("%c", s[i]);
    }
    s[i]='$';
    N = i;
    s = (int*) realloc(s,sizeof(int)*i);
    printf("\n%d", i);
    printf("\nS=  ");
    for(i=0;i<=N;i++)
    {
        printf("%d ",s[i]);
    }
    printf("\nz=%d\n", z);
    fclose(fp);

// start of algorithm
bool LMS_characters[N+1];
bool LS_type[N+1];

    find_lms_characters(s, N, LMS_characters, LS_type);
    printf("LS=       ");
    for(i=0;i<=N;i++)
    {
        printf("%d",LS_type[i]);
    }
    printf("\n");

    printf("LMS_ch=   ");
    for(i=0;i<=N;i++)
    {
        printf("%d",LMS_characters[i]);
    }

bool d_critical_ch[N+1];
int n1;
int P1[(N+1)/2];
int d=2;

    n1=find_d_critical_characters(LMS_characters, N, d, d_critical_ch, P1);
    printf("\nd_crit_ch=");
    for(i=0;i<=N;i++)
    {
        printf("%d",d_critical_ch[i]);
    }
    printf("\nP1=  ");
    for(i=0;i<=n1;i++)
    {
        printf("%d ",P1[i]);
    }

int Sw[N+1];

    calculate_Sw(s, Sw, N, LS_type);
    printf("\nSw=  ");
    for(i=0;i<=N;i++)
    {
        printf("%d ",Sw[i]);
    }

//Bucket LS
int a[n1+1], b[n1+1];
int j=0,alphabetSize=256;

    bucket_sort_LS(P1, a, d+1, n1, N, s, LS_type);

    printf("SortLS:");
    for(j=0;j<=n1;j++)
    {
        printf(" %d", a[j]);
    }
    printf("\n\n");

//bucket_sort

    for(i=0; i<d+2;i++)
    {
        if(i % 2 == 0)
        {
            bucket_sort(a, b, s, n1, N, alphabetSize, d+1-i);
            memcpy(P1,b,sizeof(int)*(n1+1));

        }
        else
        {
            bucket_sort(b, a, s, n1, N, alphabetSize, d+1-i);
            memcpy(P1,a,sizeof(int)*(n1+1));
        }
        printf("Sort%d:",i);
        for(j=0;j<=n1;j++)
        {
            printf(" %d", P1[j]);
        }
        printf("\n");
    }

return 0;

}
