#include "auxilary.h"

char *str_dynamic_copy(const char *s) 
{
    char *result = NULL;
    size_t len = 0;

    if (s)
    {
        len = strlen(s);
        result = malloc((len + 1) * sizeof(char));
        if (result)
            memcpy(result, s, (len + 1) * sizeof(char));
    }

    return result;
}

int str_iequal(const char *s1, const char *s2)
{
    unsigned char c1, c2;
    
    if (!s1 && !s2) return 0;
    if (!s1 || !s2) return 1;
    
    while (*s1 && *s2) 
    {
        c1 = (unsigned char)*s1++;
        c2 = (unsigned char)*s2++;
        
        if (c1 >= 'A' && c1 <= 'Z') c1 += 32;
        if (c2 >= 'A' && c2 <= 'Z') c2 += 32;
        
        if (c1 >= 192 && c1 <= 223) c1 += 32; 
        if (c2 >= 192 && c2 <= 223) c2 += 32; 
        
        if (c1 != c2) 
            return (int)(c1 - c2);
    }
    
    return (*s1 == '\0') - (*s2 == '\0');
}

