#include <stdio.h>
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
bool LMS_characters[N];
bool LS_type[N];

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

bool d_critical_ch[N];
int n1;
int P1[N/2];
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

int Sw[N];

    calculate_Sw(s, Sw, N, LS_type);
    printf("\nSw=  ");
    for(i=0;i<=N;i++)
    {
        printf("%d ",Sw[i]);
    }

//Bucket LS
int a[n1], b[n1];

    bucket_sort_LS(P1, a, d+1, n1, s, LS_type);

//bucket_sort


return 0;

}
