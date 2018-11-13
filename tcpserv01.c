#include	"unp.h"
#include <fcntl.h>

int
main(int argc, char **argv)
{
	int					listenfd, connfd;
	pid_t				childpid;
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	listen(listenfd, LISTENQ);

	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (SA *) &cliaddr, &clilen);
	


		if ( (childpid = Fork()) == 0) {	/* child process */
			close(listenfd);	/* close listening socket */
			send(connfd,"HTTP/1.1 200 OK\n\n", 17,0);
			//send(connfd,"Content-length: 153", 19,0);
			//send(connfd,"Content-Type: text/html", 23,0);
			char buf[1024];
 			int f = open("./hello.html",O_RDONLY);
			bzero(buf,1024);
			int size;
			while((size= read(f,buf,1024))>0){
				
				write(connfd,buf,size);
				bzero(buf,1024);				
}
    
			exit(0);
		}
		wait(NULL);
		close(connfd);			/* parent closes connected socket */
	}
}
