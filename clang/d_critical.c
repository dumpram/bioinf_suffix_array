#include <d_critical.h>

void find_lms_characters(int* s, int N, bool* LMS_ch, bool* LS)
{
int i;

    LS[N] = true;
    LMS_ch[0] = false;
        for(i=N-1; i>=0; i--)
        {
            if(s[i]>s[i+1])
            {
                LS[i] = false;
                if (LS[i+1]==true) LMS_ch[i+1]=true;
                else LMS_ch[i+1]=false;
            }
            else if(s[i]<s[i+1])
            {
                LS[i] = true;
                LMS_ch[i+1]=false;
            }
            else
            {
                LS[i] = LS[i+1];
                LMS_ch[i+1]=false;
            }
        }
}

int find_d_critical_characters(bool* LMS_ch, int N, int d, bool* d_ch, int* P1)
{
int i, j, p=0;

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
        }
        else if((i-d)<=0)
        {
            d_ch[i]=false; //for the beginning of s
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
                }
            }
            else
            {
                d_ch[i]=false;
            }
        }
    }
P1[p]=N;
return p;
}


void calculate_Sw(int* s, int* Sw, int N, bool* t)
{
int i;

    for (i=0;i<=N+1;i++)
    {
        Sw[i]=2*s[i]+(int)t[i];
    }

}


void bucket_sort_LS(int* a, int* b, int d, int n1, int* s, bool* t)
{
int c[2]={0,n1};
int i, j=0;

    for(i=0; i<=n1; i++)
    {
        j=a[i] + d;
        if(j>n1) j=n1;
        if(t[j]==1)
        {
            b[c[1]--] = a[i];
        }
        else
        {
            b[c[0]++] = a[i];
        }
    }
}
