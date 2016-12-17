#include <d_critical.h>

void find_lms_characters(char* s, int N, bool* LMS_ch)
{
int i;
bool LMS[N];

    LMS[N] = true;
    LMS_ch[0] = false;
        for(i=N-1; i>=0; i--)
        {
            if(s[i]>s[i+1])
            {
                LMS[i] = false;
                if (LMS[i+1]==true) LMS_ch[i+1]=true;
                else LMS_ch[i+1]=false;
            }
            else if(s[i]<s[i+1])
            {
                LMS[i] = true;
                LMS_ch[i+1]=false;
            }
            else
            {
                LMS[i] = LMS[i+1];
                LMS_ch[i+1]=false;
            }
        }
    printf("LMS=      ");
    for(i=0;i<=N;i++)
    {
        printf("%d",LMS[i]);
    }
    printf("\n");
}

void find_d_critical_characters(bool* LMS_ch, int N, int d, bool* d_ch, int* a)
{
int i, j, p=0;
int* P1=malloc(sizeof(int));

    d_ch[0]=false;
    d_ch[N]=true;
    for(i=1;i<N;i++)
    {
        //first criterion
        if(LMS_ch[i]==true)
        {
            d_ch[i]=true;
            P1[p]=i;
            p++;
            P1=(int*)realloc(P1,sizeof(int)*(p+1));
        }
        else if((i-d)<=0)
        {
            d_ch[i]=false;
        }
        else
        {
            //second criterion
            if(d_ch[i-d]==true && LMS_ch[i+1]!=true)
            {
                d_ch[i]=true;
                for(j=i-d+1;j<=i-1;j++)
                {
                    if(d_ch[j]==true)
                    {
                        d_ch[i]=false;
                        break;
                    }
                }
                if(d_ch[i]==true)
                {
                    P1[p]=i;
                    p++;
                    P1=(int*)realloc(P1,sizeof(int)*(p+1));
                }
            }
            else
            {
                d_ch[i]=false;
            }
        }
    }
P1[p]=N;
*a=P1;
a++;
*a=p;
}

