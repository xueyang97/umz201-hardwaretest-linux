#include "libstring.h"

int findstr(const char *str, const char *substr)
{
    int count = 0,i,j,check;
    int len = strlen(str);
    int sublen = strlen(substr);
    for(i = 0; i < len; i++)
    {
        check = 1;
        for(j = 0; j + i < len && j < sublen; j++)
        {
            if(str[i+j] != substr[j])
            {
                check = 0;
                break;
            }
        }
        if(check == 1)
        {
            count++;
            i = i + sublen;
        }
    }

    return count;
}

