//-----------------------------------------------------------
// 
// SWE2007: Software Experiment II (Fall 2017)
//
// September 17, 2017 For first version
// March 23, 2020 For revised version
// Su-bin Jeon
//
//-----------------------------------------------------------

#include "convert.h"
#include "string_sw.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int convert(const char *input)
{
    int fd_in, fd_out;
    int len, len1, len2, len3, len_write;
    int i, j, k, r = 0, n = 0;
    int convert_flag = 0;
    char *buf, *buf1, *buf2, *buf3, *ext, *tmp, *output;
    char err_msg[] = "Error occurred!\n";

    // Open and read input
    if((fd_in = open(input, O_RDONLY)) < 0){
        switch(errno){
            case 2:
                write(STDERR, err_msg, strlen(err_msg));
                return E_INPUT_NOT_EXIST;
            case 13:
                write(STDERR, err_msg, strlen(err_msg));
                return E_INPUT_NOT_ALLOWED;
            default:
                return -1;
        }
    }

    len = lseek(fd_in, 0, SEEK_END);
    lseek(fd_in, 0, SEEK_SET);
    buf = (char*)malloc(sizeof(char) * len);
    read(fd_in, buf, len);
    close(fd_in);

    // Count # of CR and LF
    for(i=0; i<len; i++){
        if(buf[i] == '\r')      r++;
        else if(buf[i] == '\n') n++;
    }

    // Case 1) convert to unix style
    if((r>0 && r==n) || (r>0 && r!=n && n-r>=r)){
        convert_flag = 1;

        // Unify newline characters
        buf1 = (char*)malloc(sizeof(char) * len);
        len1 = len;

        for(i=0, j=0; i<len; i++){
            if(buf[i] == '\r' && buf[i + 1] == '\n'){
                strncat(buf1, buf + j, i - j);
                j = i + 1;
                len1--;
            }
        }

        if(buf + j != NULL)
            strncat(buf1, buf + j, len - j);

        // Remove whitespaces before newline characters
        buf2 = (char*)malloc(sizeof(char) * len1);
        len2 = len1;

        for(i=0, j=0; i<len1; i++){
            if(buf1[i] == '\n'){
                k = i;
                while(buf1[k - 1] == ' ' || buf1[k - 1] == '\t'){
                    k--;
                    len2--;
                }
                strncat(buf2, buf1 + j, k - j);
                j = i;
            }
        }

        if(buf1 + j != NULL)
            strncat(buf2, buf1 + j, len1 - j);

        // Convert 8 spaces to 1 tab
        ext = strstr(input, ".");
        if(!strcmp(ext, ".c") || !strcmp(ext, ".h") || !strcmp(ext, ".java") || !strcmp(ext, ".txt")){
            buf3 = (char*)malloc(sizeof(char) * len2);
            len3 = len2;

            tmp = strstr(buf2, "        ");
            j = 0;
            while(tmp != NULL){
                strncat(buf3, buf2 + j, tmp - (buf2 + j));
                strncat(buf3, "\t", 1);
                j = tmp + 7 - buf2 + 1;
                len3 -= 7;
                tmp = strstr(buf2 + j, "        ");
            }

            if(buf2 + j != NULL)
                strncat(buf3, buf2 + j, len2 - j);
        }
        else{
            buf3 = buf2;
            len3 = len2;
        }
    }

    // Case 2) convert to dos style
    else if((r==0 && n>0) || (r>0 && r!=n && n-r<r)){
        convert_flag = 1;

        // Unify newline characters
        buf1 = (char*)malloc(sizeof(char) * len * 2);
        len1 = len;

        for(i=0, j=0; i<len; i++){
            if(buf[i] == '\n' && buf[i-1] != '\r'){
                strncat(buf1, buf + j, i - j);
                strncat(buf1, "\r", 1);
                j = i;
                len1++;
            }
        }

        if(buf + j != NULL)
            strncat(buf1, buf + j, len - j);

        // Remove whitespaces before newline characters
        buf2 = (char*)malloc(sizeof(char) * len1);
        len2 = len1;

        for(i=0, j=0; i<len1; i++){
            if(buf1[i] == '\r'){
                k = i;
                while(buf1[k - 1] == ' ' || buf1[k - 1] == '\t'){
                    k--;
                    len2--;
                }
                strncat(buf2, buf1 + j, k - j);
                j = i;
            }
        }

        if(buf1 + j != NULL)
            strncat(buf2, buf1 + j, len1 - j);

        // Convert 1 tab to 4 spaces
        ext = strstr(input, ".");
        if(!strcmp(ext, ".c") || !strcmp(ext, ".h") || !strcmp(ext, ".java") || !strcmp(ext, ".txt")){
            buf3 = (char*)malloc(sizeof(char) * len2 * 3);
            len3 = len2;

            tmp = strstr(buf2, "\t");
            j = 0;
            while(tmp != NULL){
                strncat(buf3, buf2 + j, tmp - (buf2 + j));
                strncat(buf3, "    ", 4);
                j = tmp - buf2 + 1;
                len3 += 3;
                tmp = strstr(buf2 + j, "\t");
            }

            if(buf2 + j != NULL)
                strncat(buf3, buf2 + j, len2 - j);
        }
        else{
            buf3 = buf2;
            len3 = len2;
        }
    }

    // Case 3) no work
    else{
        convert_flag = 0;
        buf3 = buf;
        len3 = len;
    }

    // Open and write output
    output = (char*)malloc(sizeof(char) * (strlen(input) + 5));
    strcpy(output, input);
    strcat(output, ".out");

    if((fd_out = open(output, O_WRONLY | O_CREAT | O_EXCL, 0755)) < 0){
        switch(errno){
            case 17:
                write(STDERR, err_msg, strlen(err_msg));
                return E_OUTPUT_EXIST;
            case 13:
                write(STDERR, err_msg, strlen(err_msg));
                return E_OUTPUT_NOT_ALLOWED;
            default:
                return -1;
        }
    }

    if((len_write = write(fd_out, buf3, len3)) < 0)
        return -1;
    close(fd_out);

    // Print result message
    printf("Input:%s Ouput:%s Size:%d\n", input, output, len3);

    // Free memory
    if(convert_flag){
        free(buf); free(buf1); free(buf2); free(buf3);
    }
    else{
        free(buf);
    }

    return 0;
}