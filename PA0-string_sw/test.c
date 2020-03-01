#include<stdio.h>
#include<limits.h>
#include"string_sw.h"

int main(){
	char str0[30] = "Hi!";
	char str1[30] = "Hello!";
	char str2[30] = "World!";
	char str3[30] = " ,! Hel,lo w, orl!d ";
	char str4[30] = " ,! Hel,lo w, orl!d ";
	char *ptr;

	// Conversions string to numeric formats
	printf("atoi2(\" +12345\") = %d\n", atoi2(" +12345"));
	printf("atoi2(\" -12345\") = %d\n\n", atoi2(" -12345"));

	printf("atol2(\" +12345\") = %ld\n", atol2(" +12345"));
	printf("atol2(\" -12345\") = %ld\n\n", atol2(" -12345"));

	// Conversions numeric formats to string
	printf("int2str(str0, 0) = %s\n", int2str(str0, 0));
	printf("int2str(str0, 12345) = %s\n", int2str(str0, 12345));
	printf("int2str(str0, -12345) = %s\n", int2str(str0, -12345));
	printf("int2str(str0, INT_MAX) = %s\n", int2str(str0, INT_MAX));
	printf("int2str(str0, INT_MIN) = %s\n\n", int2str(str0, INT_MIN));

	// String manipulation
	printf("str0 = %s, str1 = %s\n", str0, str1);
	printf("strcpy(str0, str1) = %s\n\n", strcpy(str0, str1));
	
	printf("str0 = %s, str1 = %s\n", str0, str1);
	printf("strncpy(str0, str1, 5) = %s\n\n", strncpy(str0, str1, 5));

	printf("str0 = %s, str2 = %s\n", str0, str2);
	printf("strcat(str0, str2) = %s\n\n", strcat(str0, str2));

	printf("str1 = %s, str2 = %s\n", str1, str2);
	printf("strncat(str1, str2, 5) = %s\n\n", strncat(str1, str2, 5));

	printf("str0 = %s\n", str0);
	printf("strdup(str0) = %s\n\n", strdup(str0));
	
	// String examination
	printf("str0 = %s\n", str0);
	printf("strlen(str0) = %zu\n\n", strlen(str0));

	printf("str0 = %s, str1 = %s\n", str0, str1);
	printf("strcmp(str0, str1) = %d\n\n", strcmp(str0, str1));
	
	printf("str0 = %s, str1 = %s\n", str0, str1);
	printf("strncmp(str0, str1, 5) = %d\n\n", strncmp(str0, str1, 5));

	printf("str0 = %s\n", str0);
	printf("strchr(str0, 'l') = %s\n\n", strchr(str0, 'l'));

	printf("str0 = %s\n", str0);
	printf("strrchr(str0, 'l') = %s\n\n", strrchr(str0, 'l'));

	printf("str0 = %s, str2 = %s\n", str0, str2);
	printf("strpbrk(str0, str2) = %s\n\n", strpbrk(str0, str2));

	printf("str0 = %s, str2 = %s\n", str0, str2);
	printf("strstr(str0, str2) = %s\n\n", strstr(str0, str2));

	printf("str3 = %s\n", str3);
	printf("strtok(str3, \" ,!\") = %s\n", strtok(str3, " ,!"));
	printf("strtok(NULL, \" ,!\") = %s\n", strtok(NULL, " ,!"));
	printf("strtok(NULL, \" ,!\") = %s\n", strtok(NULL, " ,!"));
	printf("strtok(NULL, \" ,!\") = %s\n", strtok(NULL, " ,!"));
	printf("strtok(NULL, \" ,!\") = %s\n", strtok(NULL, " ,!"));
	printf("strtok(NULL, \" ,!\") = %s\n\n", strtok(NULL, " ,!"));

	printf("str4 = %s\n", str4);
	printf("strtok_r(str4, \" ,!\", &ptr) = %s\n", strtok_r(str4, " ,!", &ptr));
	printf("strtok_r(NULL, \" ,!\", &ptr) = %s\n", strtok_r(NULL, " ,!", &ptr));
	printf("strtok_r(NULL, \" ,!\", &ptr) = %s\n", strtok_r(NULL, " ,!", &ptr));
	printf("strtok_r(NULL, \" ,!\", &ptr) = %s\n", strtok_r(NULL, " ,!", &ptr));
	printf("strtok_r(NULL, \" ,!\", &ptr) = %s\n", strtok_r(NULL, " ,!", &ptr));
	printf("strtok_r(NULL, \" ,!\", &ptr) = %s\n\n", strtok_r(NULL, " ,!", &ptr));

	// Character array minipulation
	printf("str3 = %s, str0 = %s\n", str3, str0);
	printf("memcpy(str3, str0, 10) = %s\n\n", memcpy(str3, str0, 10));

	printf("str3 = %s\n", str3);
	printf("memset(str3, '@', 5) = %s\n\n", memset(str3, '@', 5));

	return 0;
}
