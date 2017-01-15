#include <d_critical.h>

void define_LS_type(int *s, int N, bool *LS)
{
int i=0;
    LS[N] = true; // the last one, $, is always a S-type
        for(i=N-1; i>=0; i--)
        {
            if(s[i]>s[i+1])
            {
                LS[i] = false;  // false is L-type
            }
            else if(s[i]<s[i+1])
            {
                LS[i] = true;   // true is S-type
            }
            else
            {
                LS[i] = LS[i+1];    // if two neighbor characters are the same
            }
        }


}

int define_d_crit_ch(bool *LS, int N, int d, int *P1)
{
int i=0;
int j=0;
int p=0;                                    //number of d-critical characters
bool *d_ch=(bool*) malloc(sizeof(bool)*(N+1)); //array of d-critical characters

    d_ch[0]=false;  // first character must not be d-critical
    d_ch[N]=true;   // last character is d-critical
    for(i=1;i<N;i++)
    {
        //first criterion
        if(LS[i]==true && LS[i-1]==false)
        {
            d_ch[i]=true;
            P1[p++]=i;      // if character is LMS it is d-critical too
        }
        else if((i-d)<=0)
        {
            d_ch[i]=false;   // if current index is negative or zero there is no d-critical characters
        }
        else
        {
            //second criterion
            if(d_ch[i-d]==true && !(LS[i]==false && LS[i+1]==true))
            {
                d_ch[i]=true;           // if i-d is d-critical
                for(j=i-d+1;j<=i-1;j++)
                {
                    if(d_ch[j]==true)   //and if there is no d-critical in between i-d and i
                    {
                        d_ch[i]=false;
                        break;
                    }
                }
                if(d_ch[i]==true)
                {
                    P1[p++]=i;      //i-th character is d-critical
                }
            }
            else
            {
                d_ch[i]=false;
            }
        }
    }
P1[p]=N;    // the last one is always d-critical
free(d_ch);
return p;   // return the number of the d-critical characters
}

void bucket_sort_LS(int *a, int *b, int d, int n1, int N, int *s, bool *t)
{
int c[2]={0,n1};                //c consisting start and end indexes of buckets
int i=0;
int j=0;

    for(i=0; i<=n1; i++)
    {
        j=a[i] + d;
        if(j>N) j=N;            // if j exceeds array we assume it is last character $
        if(t[j]==1)
        {
            b[c[1]--] = a[i];   // if the last character of d-critical
        }                       // substring is S-type put P1[i] to S-bucket
        else
        {
            b[c[0]++] = a[i];   // else it is L-type and put it in the L-bucket
        }
    }
}

void bucket_sort(int *a, int *b, int *s, int *c, int n1, int N, int alphabetSize, int d)
{
int i=0;
int j=0;
int sum=0;                  // assistant variable
int tmp=0;                  // assistant variable
    memset(c, 0, alphabetSize*sizeof(int));
    for(i=0;i<=n1;i++)
    {
        j=a[i] + d;
        if(j>N) j=N;
        c[s[j]]++;          // we calculate number of appearance for each letter
    }
    for(i=0; i<alphabetSize; i++)
    {
        tmp=c[i];
        c[i]=sum;           // start of bucket for each character
        sum += tmp;
    }

    for(i=0,j=0;i<=n1;i++)
    {
        j=a[i] + d;
        if(j>N) j=N;
        b[c[s[j]]++] = a[i];    // sorting P1
    }
}



