#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#include "string_sw.h"

#define MAXLEN 10000
#define MAXNUM 1000

struct d_t{
	char domain[500];
	char title[500];
};

struct d_t twin[MAXNUM];
int sqnum=0;
int adnum=0;

void handler1(int sig){
	return;
}

void handler2(int sig){
	char numStr[5];
	char numStr2[5];
	int i;
	sqnum++;
	write(1, strcat(strcat(strcat(int2str(numStr, sqnum), ">"), int2str(numStr2, adnum)), " titles\n"), 
	strlen(strcat(strcat(strcat(int2str(numStr, sqnum), ">"), int2str(numStr2, adnum)), " titles\n")));
	return;
}

void collect2(int Case){
	char input[MAXLEN];
	char input2[MAXLEN];
	int len;
	char c;
	char numStr[5];
	char numStr2[5];
	pid_t pids[MAXNUM];
	int child_status;
	int cut=0;
	char tld[500];
	char domain[500];
	int fd;
	int fl;
	char* fc;
	char* front;
	char* back;
	char title[500]; 
	char target[1000];
	int maxTitleLen, maxTitleLenI;
	int i, j=0;
	int popPoint=0;
	int prvCase;

	//printf("%d\n", getpid());
	signal(SIGINT, handler1);
	signal(SIGUSR1, handler2);

	while(1){
		// 1. Get Input String
		// 1.1. From Console
		if(Case==0){
			len=0;
			while(read(0, &c, 1)!=0){
				if(c=='\n'){
					input[len]='\0';
					break;
				}
				input[len++]=c;
			}
			strncpy(input2, input, len);
		}

		// 1.2. From File
		else{
			len=0;
			while(1){
				if(read(Case, &c, 1)!=0){
					if(c=='\n'){
						input[len]='\0';
						break;
					}
					input[len++]=c;
				}
				else{
					input[len]='\0';
					popPoint=1;
					break;
				}

			}
			strncpy(input2, input, len);
		}


		// 2.1. Web pages
		if(strncmp(input, "http://", 7)==0 || strncmp(input, "https://", 8)==0){
			sqnum++;
			pids[j]=fork();
			if(pids[j]==0)
				execl("/usr/bin/wget", "wget", "-qO", int2str(numStr, sqnum), input, NULL);
			
			wait(&child_status);
			if(WEXITSTATUS(child_status)){
				write(1, strcat(strcat(int2str(numStr, sqnum), ">"), "Error occurred!\n"),
				strlen(strcat(strcat(int2str(numStr, sqnum), ">"), "Error occurred!\n")));
			}

			else{
				j++;
				cut=0;
				for(i=0; i<len; i++)
					if(input[i]=='.')	cut++;
				strtok(input, ".");
				for(i=0; i<cut; i++)
					strcpy(tld, strtok(NULL, "."));
				for(i=0; i<strlen(tld); i++){
					if(tld[i]=='/'){
						strcpy(tld, strtok(tld, "/"));
						break;
					}
				}
				//printf("%s\n", tld);

				if(!strcmp(tld, "kr")){
					strtok(input2, ".");
					for(i=0; i<cut-1; i++){
						if(i==cut-3 || i==cut-2){
							strcat(domain, strtok(NULL, "."));
							strncat(domain, ".", 1);
						}
						else	strtok(NULL, ".");
					}
					strcat(domain, tld);
				}

				else{
					strtok(input2, ".");
					for(i=0; i<cut-1; i++){
						if(i==cut-2){
							strcat(domain, strtok(NULL, "."));
							strncat(domain, ".", 1);
						}
						else	strtok(NULL, ".");
					}
					strcat(domain, tld);
				}
				//printf("%s\n", domain);
				
				fd=open(int2str(numStr, sqnum), O_RDONLY);
				fl=lseek(fd, 0, SEEK_END);
				fc=(char*)malloc(sizeof(char)*fl);
				lseek(fd, 0, SEEK_SET);
				read(fd, fc, fl);

				front=strstr(fc, "<title>")+7;
				back=strstr(fc, "</title>");
				while(1){
					if(!strncmp(front, back, 8))	break;
					strncat(title, front, 1);
					front++;
				}
				//printf("%s\n", title);

				adnum++;
				strncpy(twin[adnum].domain, domain, strlen(domain));
				strncpy(twin[adnum].title, title, strlen(title));

				write(1, strcat(strcat(strcat(strcat(strcat(int2str(numStr, sqnum), ">"), domain), ":"), title), "\n"),
				strlen(strcat(strcat(strcat(strcat(strcat(int2str(numStr, sqnum), ">"), domain), ":"), title), "\n")));

				for(i=0; i<strlen(tld); i++)	tld[i]='\0';
				for(i=0; i<strlen(domain); i++)	domain[i]='\0';
				for(i=0; i<strlen(title); i++)	title[i]='\0';
				free(fc);
			}

		}

		// 2.2. print
		else if(!strncmp(input, "print ", 6)){
			if(strcmp(input, "print ")){
				sqnum++;
				strtok(input, " ");
				strcpy(target, strtok(NULL, " "));

				maxTitleLen=0;
				for(i=1; i<=adnum; i++){
					if(!strcmp(target, twin[i].domain)){
						if(strlen(twin[i].title)>maxTitleLen){
							maxTitleLen=strlen(twin[i].title);
							maxTitleLenI=i;
						}
					}
				}

				if(maxTitleLen==0){
					write(1, strcat(strcat(int2str(numStr, sqnum), ">"), "Not Available\n"), strlen(int2str(numStr, sqnum))+15);
				}
				else{
					write(1, strcat(strcat(strcat(strcat(strcat(int2str(numStr, sqnum), ">"), target), ":"), twin[maxTitleLenI].title), "\n"),
					strlen(strcat(strcat(strcat(strcat(strcat(int2str(numStr, sqnum), ">"), target), ":"), twin[maxTitleLenI].title), "\n")));
				}

				for(i=0; i<strlen(target); i++)	target[i]='\0';
			}
		}

		// 2.3. stat
		else if(!strcmp(input, "stat")){
			sqnum++;
			write(1, strcat(strcat(strcat(int2str(numStr, sqnum), ">"), int2str(numStr2, adnum)), " titles\n"), 
			strlen(strcat(strcat(strcat(int2str(numStr, sqnum), ">"), int2str(numStr2, adnum)), " titles\n")));
		}

		// 2.4. quit
		else if(!strcmp(input, "quit")){
			for(i=0; i<j; i++)
				kill(pids[i], SIGKILL);
			return;
		}

		// 2.5. load
		else if(!strncmp(input, "load ", 5)){
			if(strcmp(input, "load ")){
				prvCase=Case;
				strtok(input, " ");
				strcpy(target, strtok(NULL, " "));
				Case=open(target, O_RDONLY);
				collect2(Case);
				Case=prvCase;
			}
		}

		for(i=0; i<len; i++)				input[i]='\0';
		for(i=0; i<len; i++)				input2[i]='\0';
		for(i=0; i<strlen(numStr); i++)		numStr[i]='\0';
		for(i=0; i<strlen(numStr2); i++)	numStr2[i]='\0';

		if(popPoint==1){
			popPoint=0;
			break;
		}
	}
}

void collect(){
	collect2(0);
}

int main(int argc, char* argv[]){
	collect();
	return 0;
}
