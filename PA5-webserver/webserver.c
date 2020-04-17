// 2017-2 SWEX2 PA5 2016314723 Jeon Su Bin

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<strings.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<pthread.h>
#include"http.h"
#include"string_sw.h"

/* Global Variables */
int threadNum=0, isReset=0;
int extNum[] = {0, 0, 0, 0, 0, 0, 0};
long long sentBytes=0;
pthread_mutex_t m;

void *connThread(void *arg);

/* Main Function */
int main(int argc, char *argv[])
{
	struct sockaddr_in saddr;
	int port=1234, listenfd, connfd;
	pthread_t tid;
	
	// 0. Change port number
	if(argc==2 && argv[1]!=NULL)	port=atoi2(argv[1]);

	// 1. socket(), bind(), listen()
	if((listenfd=socket(AF_INET, SOCK_STREAM, 0))<0){
		perror("socket");
		exit(1);
	}

	bzero((char*)&saddr, sizeof(saddr));
	saddr.sin_family=AF_INET;
	saddr.sin_addr.s_addr=htonl(INADDR_ANY);
	saddr.sin_port=htons(port);

	if(bind(listenfd, (struct sockaddr*)&saddr, sizeof(saddr))<0){
		perror("bind");
		exit(2);
	}

	if(listen(listenfd, 5)<0){
		perror("listen");
		exit(3);
	}

	//printf("Server is ready...\n");

	// 2. accept(), Create new thread only for connection
	while(1){
		if((connfd=accept(listenfd, NULL, NULL))<0){
			perror("accept");
			exit(4);
		}

		while(1)	if(!isReset)	break;
		pthread_create(&tid, NULL, connThread, &connfd);
	}

	// 3. close(), free()
	close(listenfd);
	//close(connfd);

	return 0;
}

/* Connection-thread Function */
void *connThread(void *arg)
{
	int connfd, i, tmp, fd, length, headerLen;
	long long sent=0;
	char buffer[4096]={0}, line1[1024]={0}, line2[1024]={0}, pathName[1024]={0}, host[1024]={0};
	char ext[10]={0}, extValid[6][6]={".html", ".jpg", ".png", ".gif", ".js", ".css"};
	char *httpHeader=NULL, *httpBody=NULL;

	//printf("\nNew connections...\n");

	pthread_detach(pthread_self());
	pthread_mutex_lock(&m);
	threadNum++;
	pthread_mutex_unlock(&m);

	// 1. Get input
	connfd=*((int*)arg);
	do{
		sent+=read(connfd, buffer+sent, 256);
		if(sent>=4 && !strncmp(buffer+sent-4, "\r\n\r\n", 4))	break;
	} while(sent<=1024);

	// 2. Get pathName, host from input
	strcpy(line1, strtok(buffer, "\n"));
	strcpy(line2, strtok(NULL, "\n"));

	strtok(line1, " ");
	strcpy(pathName, strtok(NULL, " "));
	if(!strcmp(pathName, "/"))	strncpy(pathName, "/index.html", 11);
	tmp=strlen(pathName);
	for(i=0; i<tmp-1; i++)	pathName[i]=pathName[i+1];
	pathName[tmp-1]='\0';

	strtok(line2, " ");
	strcpy(host, strtok(NULL, "\r"));

	// 3. Do it
	// 1) stat, 2) reset
	//printf("pathName: %s\nhost: %s\n", pathName, host);

	if(!strcmp(pathName, "stat") || !strcmp(pathName, "reset")){
		if(!strcmp(pathName, "reset")){
			pthread_mutex_lock(&m);
			isReset=1; //global
			pthread_mutex_unlock(&m);
			while(1)	if(threadNum==1)	break;
			pthread_mutex_lock(&m);
			sentBytes=0; //global
			pthread_mutex_unlock(&m);

		}

		httpBody=(char*)calloc(4096, sizeof(char));
		httpBody=make_report(extNum[0]+extNum[1]+extNum[2]+extNum[3]+extNum[4]+extNum[5]+extNum[6], sentBytes, extNum[0], extNum[1], extNum[2], extNum[3], extNum[4], extNum[5], extNum[6]);
		length=strlen(httpBody);
		httpHeader=make_http_header(OK, TYPE_HTML, length);
		
		i=0;
	}

	else{
		strcpy(ext, strchr(pathName, '.'));
		for(i=0; i<6; i++){
			if(!strcmp(ext, extValid[i])) break;
		}

		//printf("i is %d\n", i);

		// 3) type error
		if(i==6){
			httpHeader=make_http_header(E_NOT_ALLOWED, TYPE_ERROR, 0);
			length=0;
		}
		else{
			// 4) open error
			if((fd=open(pathName, O_RDONLY, 0600))<0){
				//printf("open error: errno: %d\n", errno);
				if(errno==EACCES){
					switch(i){
						case 0: httpHeader=make_http_header(E_NOT_ALLOWED, TYPE_HTML, 0); break;
						case 1: httpHeader=make_http_header(E_NOT_ALLOWED, TYPE_JPG, 0); break;
						case 2: httpHeader=make_http_header(E_NOT_ALLOWED, TYPE_PNG, 0); break;
						case 3: httpHeader=make_http_header(E_NOT_ALLOWED, TYPE_GIF, 0); break;
						case 4: httpHeader=make_http_header(E_NOT_ALLOWED, TYPE_JS, 0); break;
						default: httpHeader=make_http_header(E_NOT_ALLOWED, TYPE_CSS, 0); break;
					}
				}
				else if(errno==ENOENT){
					switch(i){
						case 0: httpHeader=make_http_header(E_NO_FILE, TYPE_HTML, 0); break;
						case 1: httpHeader=make_http_header(E_NO_FILE, TYPE_JPG, 0); break;
						case 2: httpHeader=make_http_header(E_NO_FILE, TYPE_PNG, 0); break;
						case 3: httpHeader=make_http_header(E_NO_FILE, TYPE_GIF, 0); break;
						case 4: httpHeader=make_http_header(E_NO_FILE, TYPE_JS, 0); break;
						default: httpHeader=make_http_header(E_NO_FILE, TYPE_CSS, 0); break;
					}
				}

				i=6;
				length=0;
			}

			// 5) get file
			else{
				length=lseek(fd, 0, SEEK_END);
				lseek(fd, 0, SEEK_SET);
				httpBody=(char*)calloc(length, sizeof(char));
				read(fd, httpBody, length);

				switch(i){
					case 0: httpHeader=make_http_header(OK, TYPE_HTML, length); break;
					case 1: httpHeader=make_http_header(OK, TYPE_JPG, length); break;
					case 2: httpHeader=make_http_header(OK, TYPE_PNG, length); break;
					case 3: httpHeader=make_http_header(OK, TYPE_GIF, length); break;
					case 4: httpHeader=make_http_header(OK, TYPE_JS, length); break;
					default: httpHeader=make_http_header(OK, TYPE_CSS, length); break;
				}
			}
		}
	}

	// 1~5) Common Blocks
	sent=0;
	headerLen=strlen(httpHeader);
	do{
		sent+=write(connfd, httpHeader+sent, headerLen-sent);
	} while(sent<headerLen);

	if(httpBody!=NULL){
		sent=0;
		do{
			sent+=write(connfd, httpBody+sent, length-sent);
		} while(sent<length);
		free(httpBody);
	}

	free(httpHeader);
	close(connfd);
	pthread_mutex_lock(&m);
	extNum[i]++; //global
	sentBytes+=length; //global
	threadNum--; //global
	if(isReset)	isReset=0; //global
	pthread_mutex_unlock(&m);

	return NULL;
}
