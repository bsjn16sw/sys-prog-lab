// 2016314723 Jeon Su Bin - PA4 Client

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>

int strmember(char* str, char ch){
	int i, ret=0;
	for(i=0; i<strlen(str); i++){
		if(str[i]==ch){
			ret=1;
			break;
		}
	}
	return ret;
}

int main()
{
	char input[200]={0}, input2[200]={0}, host[200]={0}, pathname[200]={0}, fileName[200]={0};
	int ret, port, cfd, rstCode, fileLen, fd, i;
	struct hostent *h;
	struct sockaddr_in saddr;
	char* storage;

	while(1){
		// 0. Initialize arrays to null
		for(i=0; i<200; i++){
			input[i]='\0';
			input2[i]='\0';
			host[i]='\0';
			pathname[i]='\0';
			fileName[i]='\0';
		}

		// 1. Disassemble user input
		ret=read(0, input, 200);
		if(!strcmp(input, "exit\n"))	exit(1);

		if(!strmember(input, '/'))	continue;

		strcpy(input2, input);
		strcpy(host, strtok(input, "/"));
		strcpy(pathname, strchr(input2, '/'));
		strcpy(pathname, pathname+1);

		port=10001;
		if(strmember(host, ':')){
			strtok(host, ":");
			port=atoi(strtok(NULL, ":"));
		}

		//printf("%s %d %s\n", host, port, pathname);

		// 2. socket(), connect()
		if((cfd=socket(AF_INET, SOCK_STREAM, 0))<0){
			write(1, "E_CONN, ", 8);
			write(1,  pathname, strlen(pathname));
			continue;
		}

		if((h=gethostbyname(host))==NULL){
			write(1, "E_CONN, ", 8);
			write(1, pathname, strlen(pathname));
			close(cfd);
			continue;
		}

		bzero((char*)&saddr, sizeof(saddr));
		saddr.sin_family=AF_INET;
		bcopy((char*)h->h_addr, (char*)&saddr.sin_addr.s_addr, h->h_length);
		saddr.sin_addr.s_addr=htonl(INADDR_ANY);
		saddr.sin_port=htons(port);

		if(connect(cfd, (struct sockaddr*)&saddr, sizeof(saddr))<0){
			write(1, "E_CONN, ", 8);
			write(1, pathname, strlen(pathname));
			close(cfd);
			continue;
		}

		// 3. write()
		write(cfd, pathname, strlen(pathname));

		// 4. read()
		read(cfd, &rstCode, sizeof(int));
		rstCode=ntohl(rstCode);

		if(rstCode==0){
			write(1, "OK, ", 4);
			write(1, pathname, strlen(pathname));

			read(cfd, &fileLen, sizeof(int));
			fileLen=ntohl(fileLen);
			storage=(char*)calloc(fileLen, sizeof(char));

			if(strmember(pathname, '/')){
				strcpy(fileName, strrchr(pathname, '/'));
				strcpy(fileName, fileName+1);
			}
			else	strcpy(fileName, pathname);
			strtok(fileName, "\n");
			
			fd=open(fileName, O_RDWR | O_CREAT | O_TRUNC, 0600);
			read(cfd, storage, fileLen);
			write(fd, storage, fileLen);
			free(storage);
			close(fd);
		}

		else if(rstCode==1){
			write(1, "E_FILE, ", 8);
			write(1, pathname, strlen(pathname));
		}

		else if(rstCode==2){
			write(1, "E_PERM, ", 8);
			write(1, pathname, strlen(pathname));
		}
		else{
			write(1, "E_UNKN, ", 8);
			write(1, pathname, strlen(pathname));
		}

		// 5. Finish
		close(cfd);
	}
}
