//file including main function and SA_DS function for suffix array construction
//Engineer: Tin Vlasic

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <d_critical.h>

#define ALPHABET_SIZE   256
#define DEBUG           0

void SA_DS(int *s, int N, int alphabetSize, int d);

int main(int argc, char *argv[])
{
int i=0;            // variable used in loops
int d=0;            // distance between two d-critical characters in the SA-DS algorithm
int n=0;            // number of characters in the input file
int N=0;            // number of suffixes in the input file
FILE *fp=NULL;      // pointer to the input file object
FILE *ffp=NULL;     // pointer to the output file object

    if(argc<4)
    {
        printf("\nProvide 3 arguments: d, input file, output file\n");
        return -1;
    }
    d=atoi(argv[1]);    // distance is the first of the arguments

//  opening input file and saving characters into an array s
    if((fp = fopen(argv[2], "r+"))==0)
    {
        printf("Could not open the input file %s", argv[2]);
        return -1;
    }
    if(fseek(fp,0,SEEK_END) != 0)
    {
        printf("Could not make to the end of the input file %s", argv[2]);
        return -1;
    }
    n = ftell(fp); // tells the number of characters in the input file
    if(DEBUG)
    {
        printf("\nNumber of characters in the input file: %d\n", n);
    }
int *s = (int*) malloc(sizeof(int)*(n+1));  // array consisting of n characters
char c='\0'; // variable in which characters from input file is written in

    if(fseek(fp,0,SEEK_SET) != 0)
    {
        printf("\nCould not make to the beginning of the input file to start saving characters into the array s\n");
        return -1;
    }

	bool isStartOfLine = true;
	bool waitNewLine = false;
    for(i=0; i<n; i++)
    {
        if( fread(&c, sizeof(char), 1, fp) != 1)
        {
            break; //EOF
        }
		if(c == '>' && isStartOfLine) {
			isStartOfLine = false;
			waitNewLine = true;
		}
        if(c != '\n' && !waitNewLine) s[i]=(int)c;  // if there is no new line save character in array s
        else if (c == '\n') {
			i--; 
			waitNewLine = false;
			isStartOfLine = true;
		} else {
			i--;
		}                   // else: don't save new line as a character
    }
    s[i]='0';                       // put $ sign on the end of the array s
    N = i;                          // store the number of characters in s including $
    s = (int*) realloc(s,sizeof(int)*(N+1));
    //printf("N=%d",i);
    if(DEBUG)
    {
        printf("\nNumber of characters in the array S:%d\n", N+1);
    }
    if(fclose(fp)!=0)
    {
        printf("\nCould not close the stream %s\n", argv[2]);
        return -1;
    }

// here is where the algorithm starts
// we provide to the SA_DS function the array of characters s, the number of characters N,
// the number of possible alphabet characters and distance between d-critical characters
    if(DEBUG)
    {
        printf("I've been here");
    }
    SA_DS(s, N, ALPHABET_SIZE, d);
    if(DEBUG)
    {
        printf("\n\nRjesenje:");
        for(i=0;i<=N;i++)
        {
            printf("%d ",s[i]);
        }
    }
// the suffix array is in s
// the section where we write the suffix array in the output file
        ffp = fopen(argv[3], "w");
        for(i=0;i<=N;i++)
        {
            fprintf(ffp,"%d%s",s[i],"\n");  // each index is in the new line
        }
        if(fclose(ffp)!=0)
        {
            printf("\nCould not close the stream %s\n",argv[3]);
            return -1;
        }
        free(s);

return 0;
}


void SA_DS(int *s, int N, int alphabetSize, int d){
    if(DEBUG)
    {
        printf("\nEntered in SA-DS\n");
    }
int i=0;                                                    // variable used in loops
int n1=0;                                                   // the number of d-critical numbers
int *P1=(int*) malloc(sizeof(int)*((N+1)/2));               // the array of pointers on d-critical characters
bool *LS_type=(bool*) malloc(sizeof(char)*(N+1));           // array of L and S types for s[i]
    define_LS_type(s, N, LS_type);                          // function that defines L and S types for characters in s
    if(DEBUG)
    {
        printf("\n\nLS:");
        for(i=0;i<=N;i++)
        {
            printf("%d",LS_type[i]);
        }
    }
    n1=define_d_crit_ch(LS_type, N, d, P1); //defining d-critical characters
int *P_1=(int*) malloc(sizeof(int)*(n1+1));      // copy of P1
int *a=(int*) malloc(sizeof(int)*(n1+1));        // buffer for sorting P1
int *b=(int*) malloc(sizeof(int)*(n1+1));        // buffer for sorting P1
int j=0;            // variable used in loops

    memcpy(P_1,P1,sizeof(int)*(n1+1));  // save the original P1 in P_1
    if (DEBUG)
    {
        printf("\nP1:");
        for(i=0;i<=n1;i++)
        {
            printf("%d ",P1[i]);
        }
    }

// first we are doing bucket sort with two buckets
// in one goes L-types and in second S-types
    bucket_sort_LS(P1, a, d+1, n1, N, s, LS_type);
    if(DEBUG)
    {
        printf("\n\nSortLS:");
        for(j=0;j<=n1;j++)
        {
            printf(" %d", a[j]);
        }
        printf("\n\n");
    }
// now we are doing bucket sorting with number of alphabet characters in d+2 steps

	int *c = (int *) malloc(sizeof(int) *alphabetSize);
    for(i=0; i<d+2;i++)
    {
        if(i % 2 == 0)
        {
            bucket_sort(a, b, s, c, n1, N, alphabetSize, d+1-i);
            if(DEBUG)
            {
                memcpy(P1,b,sizeof(int)*(n1+1));
            }
        }
        else
        {
            bucket_sort(b, a, s, c, n1, N, alphabetSize, d+1-i);
            if(DEBUG)
            {
                memcpy(P1,a,sizeof(int)*(n1+1));
            }
        }
        if(DEBUG)
        {
            printf("Sort%d:",i);
            for(j=0;j<=n1;j++)
            {
                printf(" %d", P1[j]);
            }
            printf("\n");
        }
    }

	free(c);
    if(i % 2 == 0)
    {
        memcpy(P1,a,sizeof(int)*(n1+1));    // copy pointers on d-critical characters from right buffer
    }
    else
    {
        memcpy(P1,b,sizeof(int)*(n1+1));    // copy pointers on d-critical characters from right buffer
    }
    free(a);
    free(b);
// naming P1
int name=0;                                         // current name pointer P1
int *tmp=(int*) malloc(sizeof(int)*(N+1));           // temporary buffer
int *S1=(int*) malloc(sizeof(int)*(n1+1));           // names of P1
bool diff=false;
    for(i=0;i<=N;i++)
    {
        tmp[i]=-1;  // whole tmp buffer full of -1
    }
    S1[0]=name;     // first one is 0 for now
    for(i=1; i<=n1; i++)
    {
        diff=false;
        for(j=0;j<d+2;j++)
        {
            if((2*s[P1[i-1]+j]+LS_type[P1[i-1]+j]) != (2*s[P1[i]+j]+LS_type[P1[i]+j]))
            {
                diff=true;
                S1[i]=++name;   //if there is difference between d-critical substrings increase name
                break;
            }
        }
        if(!diff)
        {
            S1[i]=name; // else name is the same and we will have to go recursive call
        }
    }
    for(i=0;i<=n1;i++)
    {
        tmp[P1[i]]=S1[i];
    }

    for(i=0,j=0;i<=N;i++)
    {
        if(tmp[i]!=-1) S1[j++]=tmp[i];  // sorting S1
    }
    if(DEBUG)
    {
        printf("S1:");
        for(i=0;i<=n1;i++)
        {
            printf(" %d", S1[i]);
        }
        printf("\nn1=%d",n1);
    }
    free(tmp);

	int *SA1=(int*) malloc(sizeof(int)*(n1+1));  // buffer
	int *SA=(int*) malloc(sizeof(int)*(N+1));    // suffix array
// recursive call if there is two same names in S1
    if(name < n1)
    {
        SA_DS(S1, n1, name + 1, d);
		memcpy(SA1,S1,sizeof(int)*(n1+1)); //copy S1 in SA1
    } else {
		for (i = 0; i <= n1; i++) {
			SA1[S1[i]] = i;
		}
	}
//inducing Step1


        //
        if (DEBUG)
        {
            printf("\n\nSA1: ");
            for(i=0;i<=n1;i++)
            {
                printf("%d ", SA1[i]);
            }
        }
        for(i=0;i<=N;i++)
        {
            SA[i]=-1;   //put -1 in every SA position
        }
        if (DEBUG)
        {
            printf("\nSA: ");
            for(i=0;i<=N;i++)
            {
                printf("%d ", SA[i]);
            }
        }
int *bucket_end=(int*) malloc(sizeof(int)*alphabetSize);   // end of buckets for step 1
int *bucket_start=(int*) malloc(sizeof(int)*alphabetSize); // start of buckets for step 2
int *bucket_end3=(int*) malloc(sizeof(int)*alphabetSize);  // end of buckets for step 3
int temp=0;                     // temporary variable
int sum=0;                      // sum of appearances for each character

        memset(bucket_end, 0, alphabetSize*sizeof(int));
//        memset(bucket_end3, 0, alphabetSize*sizeof(int));
//        memset(bucket_start, 0, alphabetSize*sizeof(int));
        for(i=0;i<=N;i++)
        {
            bucket_end[s[i]]++;
        }
        for(i=0;i<alphabetSize;i++)
        {
            temp=bucket_end[i];
            bucket_start[i]=sum;    //finding bucket starts
            sum += temp;
            bucket_end[i]=sum-1;    //finding bucket ends
            bucket_end3[i]=sum-1;
        }
        for(i=n1;i>=0;i--)
        {
            if(LS_type[P_1[SA1[i]]]==true && LS_type[P_1[SA1[i]]-1]==false)
            {
                SA[bucket_end[s[P_1[SA1[i]]]]--]=P_1[SA1[i]];
            }
        }
        if(DEBUG)
        {
            printf("\nSA: \nStep 1: ");
            for(j=0;j<=N;j++)
            {
                printf("%d ", SA[j]);
            }
        }
        free(P_1);
        free(bucket_end);
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
        if(DEBUG)
        {
            printf("\nStep 2: ");
            for(j=0;j<=N;j++)
            {
                printf("%d ", SA[j]);
            }
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
        if(DEBUG)
        {
            printf("\nStep 3: ");
            for(j=0;j<=N;j++)
            {
                printf("%d ", SA[j]);
            }
        }

        memcpy(s,SA,sizeof(int)*(N+1));     //return suffix array in s
        free(SA);
        free(SA1);
        free(S1);
        free(P1);
        free(LS_type);
}

