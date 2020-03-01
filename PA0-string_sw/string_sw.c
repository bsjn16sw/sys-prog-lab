//-----------------------------------------------------------
// 
// SWE2007: Software Experiment II (Fall 2017)
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
}
long atol2 (const char *str)
{
}

/* Conversions numeric formats to string */
char *int2str (char *dest, int num)
{
}

/* String manipulation */
char *strcpy (char *dst, const char *src)
{
}
char *strncpy (char *dst, const char *src, size_t count)
{
}
char *strcat (char *dst, const char *src)
{
}
char *strncat (char *dst, const char *src, size_t count)
{
}
char *strdup (const char *str)
{
}

/* String examination */
size_t strlen (const char *str)
{
}
int strcmp (const char *lhs, const char *rhs)
{
}
int strncmp (const char *lhs, const char *rhs, size_t count)
{
}
char *strchr (const char *str, int ch)
{
}
char *strrchr (const char *str, int ch)
{
}
char *strpbrk (const char *str, const char *accept)
{
}
char *strstr (const char *str, const char *substr)
{
}
char *strtok (char *str, const char *delim)
{
}
char *strtok_r (char *str, const char *delim, char **saveptr)
{
}

/* Character array manipulation */
void *memcpy (void *dest, const void *str, size_t n)
{
}
void *memset (void *dest, int ch, size_t count)
{
}
