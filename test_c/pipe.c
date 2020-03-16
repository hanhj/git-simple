/*
 * File Name: pipe.c
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2020-03-07 21:28:56 week:6
*/
#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
int main(int argc,char *argv[]){
	int pipefd[2];
	pid_t pid;
	char buf;
	if(argc !=2){
		printf("Usage :%s <string>\r\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	if (pipe(pipefd) == -1){
		perror("pipe");
		exit (EXIT_FAILURE);
	}
	pid=fork();
	if(pid == -1){
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if(pid==0){//child
		close(pipefd[1]);//close write pipe；
		while(read(pipefd[0],&buf,1)>0){
			write(STDOUT_FILENO,&buf,1);
		}
		write(STDOUT_FILENO,"\n",1);
		close(pipefd[0]);
		int i=0;
		while(1){
			printf("%d hello\n",i);
			sleep(1);
			i++;
		}
		_exit(EXIT_SUCCESS);
	}else{//parent
		close(pipefd[0]);//close read
		write(pipefd[1],argv[1],strlen(argv[1]));
		close(pipefd[1]);
		wait(NULL);//wait for child
		exit(EXIT_SUCCESS);
	}
	
}

// vim:tw=72 
