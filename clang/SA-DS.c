#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <d_critical.h>

void find_lms(char* s, int N, bool* LMS);

int main()
{
FILE *fp;
int n, m, i;

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
char *s = (char*) malloc(n);
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
        if(c != '\n') s[i]=c;
        else i--;
        printf("%c", s[i]);
    }
    s[i]='$';
    s[i+1]='\0';
    m = i;
    s = (char*) realloc(s,i);
    printf("\n%d", i);
    printf("\n%s", s);
    printf("\nz=%d\n", z);
    fclose(fp);

// start of algorithm
bool LMS_characters[m];

    find_lms_characters(s,m,LMS_characters);
    printf("LMS_ch=   ");
    for(i=0;i<=m;i++)
    {
        printf("%d",LMS_characters[i]);
    }

bool d_critical_ch[m];
int* a[2];
int* P1;
int p_cnt;
int d=2;

    find_d_critical_characters(LMS_characters, m, d, d_critical_ch, a);
    P1=a[0];
    p_cnt=a[1];
    printf("\nd_crit_ch=");
    for(i=0;i<=m;i++)
    {
        printf("%d",d_critical_ch[i]);
    }
    printf("\nP1=  ");
    for(i=0;i<=p_cnt;i++)
    {
        printf("%d ",P1[i]);
    }

return 0;

}
