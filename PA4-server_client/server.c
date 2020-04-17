// 2016314723 Jeon Su Bin - PA4 Server

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<wait.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>

int main(int argc, char* argv[])
{
	int port=10001, listenfd, connfd, caddrlen, fd, rstCode, fileLen, i;
	struct hostent *h;
	struct sockaddr_in saddr, caddr;
	char pathname[200]={0};
	char* storage;
	pid_t pid;

	// 1. Get parameter
	if(argc==2)	port=atoi(argv[1]);
	
	// 2. socket(), bind(), listen()
	if((listenfd=socket(AF_INET, SOCK_STREAM, 0))<0){
		write(2, "socket() failed.\n", 17);
		return -1;
	}

	bzero((char*)&saddr, sizeof(saddr));
	saddr.sin_family=AF_INET;
	saddr.sin_addr.s_addr=htonl(INADDR_ANY);
	saddr.sin_port=htons(port);

	if(bind(listenfd, (struct sockaddr*)&saddr, sizeof(saddr))<0){
		write(2, "bind() failed.\n", 15);
		return -1;
	}

	if(listen(listenfd, 5)<0){
		write(2, "listen() failed.\n", 17);
		return -1;
	}

	// 3. accept()
	while(1){
		caddrlen=sizeof(caddr);
		if((connfd=accept(listenfd, (struct sockaddr*)&caddr, &caddrlen))<0){
			write(2, "accept() failed.\n", 17);
			continue;
		}

		h=gethostbyaddr((const char*)&caddr.sin_addr.s_addr, sizeof(caddr.sin_addr.s_addr), AF_INET);
		write(1, "Server connected...\n", 20);

		if((pid=fork())==0){
			read(connfd, pathname, 200);
			strtok(pathname, "\n");

			if((fd=open(pathname, O_RDONLY, 0600))<0){
				if(errno==ENOENT)		rstCode=htonl(1);
				else if(errno==EPERM)	rstCode=htonl(2);
				else				rstCode=htonl(3);
				write(connfd, &rstCode, sizeof(int));
			}
			else{
				rstCode=htonl(0);
				fileLen=lseek(fd, 0, SEEK_END);
				lseek(fd, 0, SEEK_SET);
				storage=(char*)calloc(fileLen, sizeof(char));
				read(fd, storage, fileLen);
				
				write(connfd, &rstCode, sizeof(int));
				fileLen=htonl(fileLen);
				write(connfd, &fileLen, sizeof(int));
				fileLen=ntohl(fileLen);
				write(connfd, storage, fileLen);
				free(storage);
				close(fd);
			}

			for(i=0; i<200; i++)	pathname[i]='\0';
			close(connfd);
			exit(4);
		}
		waitpid(pid, NULL, 0);

		close(connfd);
		write(1, "Connection terminated...\n", 25);
	}
}
