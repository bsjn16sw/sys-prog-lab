//-----------------------------------------------------------
// 
// SWE2007: Software Experiment II (Fall 2017)
//
// Whole code for PA #3
// October 11, 2017
//
// 2016314723 Jeon Su Bin
// Department of Software
// Sungkyunkwan University
//
//-----------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <wait.h>
#include "string_sw.h"

pid_t childsPgid;

char *pwd (int full)
{
    static char buffer[1024];
    char *ptr;
    if (getcwd(buffer, 1024) != buffer)
        return NULL;

    if (full)
        return buffer;
    
    if (strcmp("/", buffer) == 0)
        return buffer;

    ptr = strrchr(buffer, '/');
    return ptr+1;
}

void print_prompt()
{
    int ret;
    char *ptr = pwd(0);
    char head[200];

    if (!ptr)
        exit(1);
    if (strlen(ptr) > 190)
        exit(1);

    strcpy(head, "swsh:");
    strcat(head, ptr);
    strcat(head, "> ");

    ret = write(2, head, strlen(head));
    if (ret <= 0)
        exit(1);
}

void ctrlzHandling(int sig){
	int status;

	waitpid(-1, &status, WNOHANG | WUNTRACED);
	if(WIFSTOPPED(status))
		kill(-childsPgid, SIGKILL);
}

void errprint(int num, char cmd[]){
	char dest[10]={0};

	write(2, cmd, 2);
	if(num==13)			write(2, ": Permission denied\n", 20);
	else if(num==21)	write(2, ": Is a directory\n", 17);
	else if(num==2)		write(2, ": No such file or directory\n", 28);
	else if(num==20)	write(2, ": Not a directory\n", 18);
	else if(num==1)		write(2, ": Permission denied\n", 20);
	else{
		write(2, ": Error occured: ", 17);
		write(2, int2str(dest, num), strlen(int2str(dest, num)));
		write(2, "\n", 1);
	}
}

void diy(char* cmdArgv[])
{
	char buf[100];

	errno=0;
	if(!strcmp(cmdArgv[0], "mv")){
		rename(cmdArgv[1], cmdArgv[2]);
		if(errno>0)		errprint(errno, cmdArgv[0]);
	}
	else if(!strcmp(cmdArgv[0], "rm")){
		unlink(cmdArgv[1]);
		if(errno>0)		errprint(errno, cmdArgv[0]);
	}
	else{
		getcwd(buf, 100);
		write(1, buf, strlen(buf));
		write(1, "\n", 1);
	}
}

int main(void)
{
    int ret, cmdNum, valid=0, inRe=0, otRe=0, chunk=0, fDes, status, exitNum=0, i, j;
	int td[2], fd[2], prvPipeRead;
	pid_t pid;
    char input[240]={0}, cmd[100][50]={0}, cmd2[100][50]={0}, tmpcmd[50]={0}, fName[50]={0};
	char* cmdArgv[10];
	char cmdt0[][5]={"ls", "man", "grep", "sort", "awk", "bc", "head", "tail", "cat", "cp", "mv", "rm", "cd", "pwd", "exit"};
	char cmdt12[][5]={"ls", "man", "grep", "sort", "awk", "bc", "head", "tail", "cat", "cp"};
	char cmdt34[][5]={"mv", "rm", "pwd"};

	system("clear");
    while (1)
	{
		pipe(td);
		dup2(0, td[0]);
		dup2(1, td[1]);

        print_prompt();
		
		// Handling Signals
		signal(SIGINT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
		signal(SIGCHLD, ctrlzHandling);

		// Accepting Input
        ret = read(0, input, 200);
        if (ret < 0)	exit(1);
        else if (ret == 0) {
            write(2, "exit\n", 5);
            break;
        }
		else if(ret==1)		continue;

		// Deleting Newline Feed
		// Counting Number of Commands
		input[ret-1]='\0';
		cmdNum=1;
		for(i=0; i<ret; i++)
			if(input[i]=='|')	cmdNum++;

		// Tokening and Saving Commands
		strcpy(cmd[0], strtok(input, "|"));
		if(cmdNum>1)	cmd[0][strlen(cmd[0])-1]='\0';

		for(i=1; i<cmdNum; i++){
			strcpy(cmd[i], strtok(NULL, "|"));
			strcpy(cmd[i], cmd[i]+1);
			if(cmd[i][strlen(cmd[i])-1]==' ')
				cmd[i][strlen(cmd[i])-1]='\0';
		}

		// Saving Name of Commands and Checking Validity
		for(i=0; i<cmdNum; i++){
			strcpy(tmpcmd, cmd[i]);
			strcpy(cmd2[i], strtok(tmpcmd, " "));
			tmpcmd[0]='\0';
			
			valid=0;
			for(j=0; j<15; j++){
				if(!strcmp(cmd2[i], cmdt0[j])){
					valid=1;
					break;
				}
			}
			if(!strncmp(cmd2[i], "./", 2))
				valid=1;

			if(!valid){
				write(2, "swsh: Command not found\n", 24);
				break;
			}
		}

		// Doing Work!!!
		for(i=0; i<cmdNum; i++){
			if(!valid)	break;
			
			errno=0;
			if(i!=(cmdNum-1)){
				pipe(fd);
				dup2(fd[1], 1);
				close(fd[1]);
			}
			
			if(!strcmp(cmd2[i], "exit")){
				if(!strchr(cmd[i], ' ')){
					strtok(cmd[i], " ");
					exitNum=atoi2(strtok(NULL, " "));
				}
				write(2, "exit\n", 5);
				return exitNum;
			}

			if(!strcmp(cmd2[i], "cd")){
				strtok(cmd[i], " ");
				chdir(strtok(NULL, " "));
				if(errno>0)		errprint(errno, cmd2[i]);
			}

			if((pid=fork())==0){
				signal(SIGINT, SIG_DFL);
				signal(SIGTSTP, SIG_DFL);

				for(j=0; j<10; j++)
					cmdArgv[i]=NULL;
				
				// 0th command
				if(i==0){
					// Setting pgid
					setpgid(getpid(), getpid());

					// Checking Input/output Redirection
					otRe=0;
					inRe=0;
					for(j=0; j<strlen(cmd[0]); j++){
						if(cmd[0][j]=='<')	inRe=j;
						if(cmd[0][j]=='>'){
							otRe=j;
							break;
						}
					}

					if(otRe){ // available when cmdNum==1
						if(cmd[0][otRe+1]=='>'){
							strcpy(fName, cmd[0]+otRe+3);
							fDes=open(fName, O_RDWR | O_CREAT | O_APPEND, 0644);
						}
						else{
							strcpy(fName, cmd[0]+otRe+2);
							fDes=open(fName, O_RDWR | O_CREAT | O_TRUNC, 0644);
						}

						cmd[0][otRe-1]='\0';
						dup2(fDes, 1);
						close(fDes);
						fName[0]='\0';
					}

					if(inRe){
						strcpy(fName, cmd[0]+inRe+2);
						fDes=open(fName, O_RDONLY, 0644);
						if(errno==2){
							write(2, "swsh: No such file\n", 19);
							valid=0;
							if(cmdNum>1){
								close(fd[0]);
								close(fd[1]);
							}
							continue;
						}

						cmd[0][inRe-1]='\0';
						dup2(fDes, 0);
						close(fDes);
						fName[0]='\0';
					}

				}

				// 1st ~ (cmdNum-2)th command
				else if(0<i && i<cmdNum-1){
					// Setting pgid
					setpgid(getpid(), childsPgid);
					
					dup2(prvPipeRead, 0);
					close(prvPipeRead);
				}

				// (cmdNum-1)th command
				else{
					// Setting pgid
					setpgid(getpid(), childsPgid);

					// Checking Output Redirection
					otRe=0;
					for(j=0; j<strlen(cmd[i]); j++){
						if(cmd[i][j]=='>'){
							otRe=j;
							break;
						}
					}

					if(otRe){
						if(cmd[i][otRe+1]=='>'){
							strcpy(fName, cmd[i]+otRe+3);
							fDes=open(fName, O_RDWR | O_CREAT | O_APPEND, 0644);
						}
						else{
							strcpy(fName, cmd[i]+otRe+2);
							fDes=open(fName, O_RDWR | O_CREAT | O_TRUNC, 0644);
						}

						cmd[i][otRe-1]='\0';
						dup2(fDes, 1);
						close(fDes);
						fName[0]='\0';
					}

					dup2(prvPipeRead, 0);
					close(prvPipeRead);
				}

				// Chunking Command and Options/Arguments
				chunk=1;
				for(j=0; j<strlen(cmd[i]); j++)
					if(cmd[i][j]==' ')	chunk++;
				cmdArgv[0]=strtok(cmd[i], " ");
				for(j=1; j<chunk; j++)
					cmdArgv[j]=strtok(NULL, " ");
				cmdArgv[chunk]=NULL;

				// Executing Program
				// cmd_type1,2
				for(j=0; j<10; j++){
					if(!strcmp(cmdArgv[0], cmdt12[j]) || !strncmp(cmdArgv[0], "./", 2)){
						execvp(cmdArgv[0], cmdArgv);
						exit(1);
					}
				}

				// cmd_type3,4
				for(j=0; j<4; j++){
					if(!strcmp(cmdArgv[0], cmdt34[j])){
						errno=0;
						diy(cmdArgv);
						if(errno>0)	valid=0;
						exit(1);
					}
				}
			}
			
			wait(&status);
			if(i==0)
				childsPgid=pid;
			if(i!=(cmdNum-1))
				prvPipeRead=fd[0];
			dup2(td[0], 0);
			dup2(td[1], 1);
		}

		close(td[0]);
		close(td[1]);
		if(!valid)	continue;

    }

    return exitNum;
}
