#include <stdio.h>
#include <stdlib.h>

int main()
{
FILE *fp;

    if(!(fp = fopen("test.txt", "r+")))
    {
        printf("Nemrem otprt file");
    }

    fclose(fp);

return 0;

}
