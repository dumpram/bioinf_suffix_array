#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <d_critical.h>

void SA_DS(int* s, int N, int alphabetSize, int d);

int main()
{
FILE *fp;
FILE *ffp;
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

    if(fclose(fp)!=0)
    {
        printf("\nNisam dobro sam zatvorio stream test.txt\n");
    }

int alphabetSize=256;
int d=2;

    SA_DS(s, N, alphabetSize, d);

    printf("\n\nRjesenje: ");
    for(i=0;i<=N;i++)
    {
        printf("%d ",s[i]);
    }

//write the output in file "out.txt"
        ffp = fopen("out.txt", "w");
        for(i=0;i<=N;i++)
        {
            fprintf(ffp,"%d%s",s[i],"\n");
        }
        if(fclose(fp)!=0)
        {
            printf("\nNisam dobro sam zatvorio stream out.txt\n");
        }

return 0;
}


void SA_DS(int* s, int N, int alphabetSize, int d){
// start of algorithm
bool LMS_characters[N+1];
bool LS_type[N+1];
int i=0;

    find_lms_characters(s, N, LMS_characters, LS_type);
    printf("\nLS=       ");
    for(i=0;i<=N;i++)
    {
        printf("%d",LS_type[i]);
    }
    printf("\n");

    printf("LMS_ch=   ");
    for(i=0;i<=N;i++,alphabetSize=256)
    {
        printf("%d",LMS_characters[i]);
    }

bool d_critical_ch[N+1];
int n1;
int P1[(N+1)/2];

    n1=find_d_critical_characters(LMS_characters, N, d, d_critical_ch, P1);

int P_1[n1+1];
    memcpy(P_1,P1,sizeof(int)*(n1+1));

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
int j=0;

    bucket_sort_LS(P1, a, d+1, n1, N, s, LS_type);

    printf("\nSortLS:");
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

//naming P1
int name=0;
int tmp[N+1], S1[n1+1];
bool diff=false;

    for(i=0;i<=N;i++)
    {
        tmp[i]=-1;
    }

    S1[0]=name;
    for(i=1; i<=n1; i++)
    {
        diff=false;
        for(j=0;j<d+2;j++)
        {
            if(Sw[P1[i-1]+j] != Sw[P1[i]+j])
            {
                diff=true;
                S1[i]=++name;
                break;
            }
        }
        if(!diff)
        {
            S1[i]=name;
        }
    }

    for(i=0;i<=n1;i++)
    {
        tmp[P1[i]]=S1[i];
    }

    for(i=0,j=0;i<=N;i++)
    {
        if(tmp[i]!=-1) S1[j++]=tmp[i];
    }

    printf("S1:");
    for(i=0;i<=n1;i++)
    {
        printf(" %d", S1[i]);
    }
    printf("\nn1=%d",n1);

    if(name<n1)
    {
        printf("\n\nidemo u rekurziju :)");
        SA_DS(S1, n1, alphabetSize, d);
    }

        printf("\n\nsad idemo u induciranje\n");
        //inducing Step1
        int SA1[n1+1];
        int SA[N+1];

        memcpy(SA1,S1,sizeof(int)*(n1+1));
        printf("SA1: ");
        for(i=0;i<=n1;i++)
        {
            printf("%d ", SA1[i]);
        }

        for(i=0;i<=N;i++)
        {
            SA[i]=-1;
        }
        printf("\nSA: ");
        for(i=0;i<=N;i++)
        {
            printf("%d ", SA[i]);
        }

int bucket_end[alphabetSize], bucket_start[alphabetSize];
int bucket_end3[alphabetSize];
int temp=0, sum=0;
        memset(bucket_end, 0, alphabetSize*sizeof(int));
        for(i=0;i<=N;i++)
        {
            bucket_end[s[i]]++;
        }
        for(i=0;i<=alphabetSize;i++)
        {
            temp=bucket_end[i];
            bucket_start[i]=sum;
            sum += temp;
            bucket_end[i]=bucket_end3[i]=sum-1;
        }

        for(i=n1;i>=0;i--)
        {
            if(LMS_characters[P_1[SA1[i]]]==true)
            {
                SA[bucket_end[s[P_1[SA1[i]]]]--]=P_1[SA1[i]];
            }
        }
        printf("\nSA: \nStep 1: ");
        for(j=0;j<=N;j++)
        {
            printf("%d ", SA[j]);
        }

        //inducing step2
        for(i=0;i<=N;i++)
        {
            if(SA[i]>0)
            {
                if(LS_type[SA[i]-1]==false)
                {
                    SA[bucket_start[s[SA[i]-1]]++]=SA[i]-1;
                }
            }
        }

        printf("\nStep 2: ");
        for(j=0;j<=N;j++)
        {
            printf("%d ", SA[j]);
        }
        //inducing step3
        for(i=N;i>=0;i--)
        {
            if(SA[i]>0)
            {
                if(LS_type[SA[i]-1]==true)
                {
                    SA[bucket_end3[s[SA[i]-1]]--]=SA[i]-1;
                }
            }
        }
        printf("\nStep 3: ");
        for(j=0;j<=N;j++)
        {
            printf("%d ", SA[j]);
        }
        memcpy(s,SA,sizeof(int)*(N+1));
}

