//-----------------------------------------------------------
// 
// SWE2007: Software Experiment II (Fall 2017)
//
// Fill out the skeleton code for PA #0
// September 17, 2017 For first version
// February 28, 2020 For revised version
// Su-bin Jeon
//
// Skeleton code for PA #0
// September 6, 2017
//
// Ha-yun Lee, Jong-won Park
// Embedded Software Laboratory
// Sungkyunkwan University
//
//-----------------------------------------------------------

#include "string_sw.h"
#include <stdlib.h>

#define isspace(ch) ((ch) == ' ' || (ch) == '\t' || (ch) == '\n' || \
					 (ch) == '\v' || (ch) == '\f' || (ch) == '\r')

/* Conversions string to numeric formats */
int atoi2 (const char *str)
{
    int sign = 0, value = 0;

    if(str == NULL) return 0;

    while(isspace(*str))    str++;

    if(*str == '-'){
        sign = 1;
        str++;
    }
    else if(*str == '+'){
        sign = 0;
        str++;
    }
    else if(!('0' <= *str && *str <= '9')){
        return 0;
    }

    while('0' <= *str && *str <= '9'){
        value = 10 * value + *str - '0';
        str++;
    }

    if(sign)    value = -value;

    return value;
}

long atol2 (const char *str)
{
    long sign = 0, value = 0;

    if(str == NULL) return 0;

    while(isspace(*str))    str++;

    if(*str == '-'){
        sign = 1;
        str++;
    }
    else if(*str == '+'){
        sign = 0;
        str++;
    }
    else if(!('0' <= *str && *str <= '9')){
        return 0;
    }

    while('0' <= *str && *str <= '9'){
        value = 10 * value + *str - '0';
        str++;
    }

    if(sign)    value = -value;

    return value;
}

/* Conversions numeric formats to string */
char *int2str (char *dest, int num)
{
    int sign = 0, i = 0, j = 0;
    char *temp = (char*)malloc(sizeof(char) * 12);

    if(dest == NULL){
        dest = (char*)malloc(sizeof(char) * 12);
        if(dest == NULL)    return NULL;
    }

    if(num == -2147483648){
        dest = strncpy(dest, "-2147483648", 11);
        return dest;
    }

    if(num < 0){
        sign = 1;
        num = -num;
    }

    do {
        temp[i] = (num % 10) + '0';
        num /= 10;
        i++;
    } while(num > 0);


    if(sign){
        dest[0] = '-';
        j++;
    }

    i--;
    while(i >= 0){
        dest[j] = temp[i];
        j++;
        i--;
    }
    dest[j] = '\0';

    return dest;
}

/* String manipulation */
char *strcpy (char *dst, const char *src)
{
    size_t len = strlen(src), i;

    for(i=0; i<=len; i++)
        dst[i] = src[i];

    return dst;
}
char *strncpy (char *dst, const char *src, size_t count)
{
    size_t i;

    for(i=0; i<count; i++)
        dst[i] = src[i];
    dst[i] = '\0';

    return dst;
}

char *strcat (char *dst, const char *src)
{
    size_t d_len, s_len, i;

    d_len = strlen(dst);
    s_len = strlen(src);

    for(i=0; i<=s_len; i++)
        dst[d_len + i] = src[i];

    return dst;
}

char *strncat (char *dst, const char *src, size_t count)
{
    size_t d_len, i;

    d_len = strlen(dst);

    for(i=0; i<count; i++)
        dst[d_len + i] = src[i];
    dst[d_len + i] = '\0';

    return dst;
}

char *strdup (const char *str)
{
    char* dst;
    size_t len, i;

    len = strlen(str);

    dst = (char*)malloc(sizeof(char) * (len + 1));
    if(dst == NULL) return NULL;

    for(i=0; i<=len; i++)
        dst[i] = str[i];

    return dst;
}

/* String examination */
size_t strlen (const char *str)
{
    size_t len = 0;

    while(str[len] != '\0')
        len++;

    return len;
}

int strcmp (const char *lhs, const char *rhs)
{
    size_t i = 0;

    while(1){
        if(lhs[i] != rhs[i])
            return lhs[i] - rhs[i];

        if(lhs[i] == '\0' && rhs[i] == '\0')
            return 0;

        i++;
    }
}

int strncmp (const char *lhs, const char *rhs, size_t count)
{
    size_t i = 0;

    while(1){
        if(lhs[i] != rhs[i])
            return lhs[i] - rhs[i];

        if(i + 1 == count)
            return 0;

        i++;
    }
}

char *strchr (const char *str, int ch)
{
    size_t i = 0;

    while(1){
        if(str[i] == '\0')
            return NULL;

        if(str[i] == ch)
            return (char*)str + i;

        i++;
    }
}

char *strrchr (const char *str, int ch)
{
    size_t i = strlen(str) - 1;

    while(1){
        if(i < 0)
            return NULL;

        if(str[i] == ch)
            return (char*)str + i;

        i--;
    }
}

char *strpbrk (const char *str, const char *accept)
{
    size_t s_len, a_len, i, j;

    s_len = strlen(str);
    a_len = strlen(accept);

    for(i=0; i<s_len; i++){
        for(j=0; j<a_len; j++){
            if(str[i] == accept[j])
                return (char*)str + i;
        }
    }

    return NULL;
}

char *strstr (const char *str, const char *substr)
{
    size_t s_len, b_len, i, j = 0, k = 0;

    s_len = strlen(str);
    b_len = strlen(substr);

    for(i=0; i<s_len; i++){
        if(s_len - i < b_len)
            return NULL;

        while(str[i + k] == substr[j + k] && k < b_len)
            k++;

        if(k == b_len)
            return (char*)str + i;
        else
            j = 0; k = 0;
    }
    
    return NULL;
}

char *strtok (char *str, const char *delim)
{
    static char* ret = NULL;
    static char* ptr_str = NULL;
    char* ptr_delim = NULL;

    if(str != NULL) ptr_str = str;
    else{
        // Tokenize for str of previous strtok().
        // So ptr_str is not updated at this point.
    }

    if(ptr_str == NULL || *ptr_str == '\0')
        return NULL;

    if(delim == NULL || *delim == '\0')
        return NULL;

    // Find and remove any char in delim from head of str.
    for(; *ptr_str != '\0'; ptr_str++){
        for(ptr_delim = (char*)delim; *ptr_delim != '\0'; ptr_delim++){
            if(*ptr_str == *ptr_delim)
                break;
        }
        if(*ptr_delim == '\0')
            break;
    }

    ret = ptr_str;
    ptr_str++;
    ptr_delim = (char*)delim;

    for(; *ptr_str != '\0'; ptr_str++){
        for(ptr_delim = (char*)delim; *ptr_delim != '\0'; ptr_delim++){
            if(*ptr_str == *ptr_delim)
                break;
        }
        // If below condition is true, that means any char
        // in delim is located in current position of str.
        // So put '\0' on that position and move where
        // ptr_str is pointing to the next position
        // for preparing next strtok() call.
        if(*ptr_delim != '\0'){
            *ptr_str++ = '\0';
            break;
        }
    }

    return ret;
}

char *strtok_r (char *str, const char *delim, char **saveptr)
{
    // Only difference of strtok_r from strtok is, there
    // is saveptr that saves the point for next tokening.
    // So we don't have to declare ptr_str as static variable.

    static char* ret = NULL;
    char* ptr_str = NULL;
    char* ptr_delim = NULL;

    if(str != NULL) ptr_str = str;
    else    ptr_str = *saveptr;

    if(ptr_str == NULL || *ptr_str == '\0')
        return NULL;

    if(delim == NULL || *delim == '\0')
        return NULL;

    for(; *ptr_str != '\0'; ptr_str++){
        for(ptr_delim = (char*)delim; *ptr_delim != '\0'; ptr_delim++){
            if(*ptr_str == *ptr_delim)
                break;
        }
        if(*ptr_delim == '\0')
            break;
    }

    ret = ptr_str;
    ptr_str++;
    ptr_delim = (char*)delim;

    for(; *ptr_str != '\0'; ptr_str++){
        for(ptr_delim = (char*)delim; *ptr_delim != '\0'; ptr_delim++){
            if(*ptr_str == *ptr_delim)
                break;
        }

        if(*ptr_delim != '\0'){
            *ptr_str++ = '\0';
            *saveptr = ptr_str;
            break;
        }
    }

    return ret;
}

/* Character array manipulation */
void *memcpy (void *dest, const void *str, size_t n)
{
    char* d = dest;
    char* s = (char*) str;

    while(n--)  *d++ = *s++;

    return dest;
}

void *memset (void *dest, int ch, size_t count)
{
    char* d = dest;

    while(count--)  *d++ = ch;

    return dest;
}
