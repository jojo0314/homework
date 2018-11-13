/* include fig01 */
#include	"unp.h"

int
main(int argc, char **argv)
{
	int					i, maxi, maxfd, listenfd, connfd, sockfd;
	int					nready, client[FD_SETSIZE],mark[FD_SETSIZE];
	fd_set				rset, allset;
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	maxfd = listenfd;			/* initialize */
	maxi = -1;					/* index into client[] array */
	for (i = 0; i < FD_SETSIZE; i++)
		mark[i] = client[i] = -1;			/* -1 indicates available entry */
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);
/* end fig01 */

/* include fig02 */
	for ( ; ; ) {
		rset = allset;		/* structure assignment */
		nready = Select(maxfd+1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(listenfd, &rset)) {	/* new client connection */
			clilen = sizeof(cliaddr);
			connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
#ifdef	NOTDEF
			printf("new client: %s, port %d\n",
					Inet_ntop(AF_INET, &cliaddr.sin_addr, 4, NULL),
					ntohs(cliaddr.sin_port));
			
#endif

			for (i = 0; i < FD_SETSIZE; i++)
				if (client[i] < 0) {
					client[i] = connfd;	
					if(mark[i]<0)	mark[i] = 0;/* save descriptor */

					break;
				}
			if (i == FD_SETSIZE)
				err_quit("too many clients");

			FD_SET(connfd, &allset);	/* add new descriptor to set */
			if (connfd > maxfd)
				maxfd = connfd;			/* for select */
			if (i > maxi)
				maxi = i;				/* max index in client[] array */

			if (--nready <= 0)
				continue;	
					/* no more readable descriptors */
		}
		
		for (i = 0; i <= maxi; i++) {	/* check all clients for data */
			sockfd = client[i];
			//printf("%d %d\n",i,sockfd);
			if(mark[i]==0){
					//printf("%d %d\n",i,sockfd);
					mark[i] =1;
					//close(listenfd);
					send(sockfd,"HTTP/1.1 200 OK\n\n", 17,0);
					char buff[1024];
 					int f = open("./hello.html",O_RDONLY);
					bzero(buff,1024);
					int size;
					while((size= read(f,buff,1024))){
							//printf("ppp\n");
							write(sockfd,buff,size);
							bzero(buff,1024);		
						}
					close(sockfd);	
					FD_CLR(sockfd, &allset);
					client[i] = -1;
					}
					
}

							/* no more readable descriptors */
			
			

		
	}
}

/* end fig02 */
