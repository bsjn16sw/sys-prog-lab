//-----------------------------------------------------------
// 
// SWE2007: Software Experiment II (Fall 2017)
//
// Skeleton code for PA #1
// September 13, 2017
//
// Jong-Won Park
// Embedded Software Laboratory
// Sungkyunkwan University
//
//-----------------------------------------------------------

#include "convert.h"
#include <unistd.h>
#include <stdio.h>

int main(void)
{
	int ret = 0;
	char input[] = "data/hello1.txt";
    char error[] = "This shall never happen.\n";
	
	ret = convert(input);
	
    switch (ret)
    {
        case 0:
            break;
        case E_INPUT_NOT_EXIST:
			printf("E_INPUT_NOT_EXIST\n");
            break;
        case E_INPUT_NOT_ALLOWED:
			printf("E_INPUT_NOT_ALLOWED\n");
            break;
        case E_OUTPUT_EXIST:
			printf("E_OUTPUT_EXIST\n");
            break;
        case E_OUTPUT_NOT_ALLOWED:
            printf("E_OUTPUT_NOT_ALLOWED\n");
            break;

        default:
            write(2, error, sizeof(error)-1);
    }


	return ret;
}
