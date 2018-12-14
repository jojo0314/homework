#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#define PORT 9877
#define MAXLINE 100
#define LISTENQ 10
int connect_num;
int connfd[LISTENQ];
char NAME[10][20];
void list(int n){

    char tmp[] = {"User Online"};
    send(connfd[n],tmp,sizeof(tmp),0);
    int i;
    for(i=0;i<LISTENQ;i++){
        if(connfd[i]!=-1){
            char msg[MAXLINE]={};
            sprintf(msg,"%s",NAME[i]);
            printf("%s\n",NAME[i]);
         //   strcat(msg,"\n");
            printf("%s",msg);
            send(connfd[n],msg,sizeof(msg),0);

        }
    }
            char tmp2[MAXLINE]={};
            sprintf(tmp2,"%s",NAME[n]);
            strcat(tmp,">");
            send(connfd[n],tmp2,strlen(tmp2),0);

}
void sendmsgall(char *msg,int n){
    int i;
    strcat(msg,"\n");
    for(i=0;i<LISTENQ;i++){
        if(connfd[i]!=-1 && i!=n){
            send(connfd[i],msg,strlen(msg),0);
            char tmp[MAXLINE] = {};
            sprintf(tmp,"%s",NAME[i]);
            strcat(tmp,">");
            send(connfd[i],tmp,strlen(tmp),0);
        }
    }

}
void* service_thread(void* data){
    int n = *((int *) data);
    char buf[MAXLINE];
    
    memset(buf,0,sizeof(buf));
    recv(connfd[n],buf,sizeof(buf),0);
    sprintf(NAME[connect_num++],"%s",buf);
    printf("client %s connected\n",buf);

    while(1){
        memset(buf,0,sizeof(buf));
        if(recv(connfd[n],buf,sizeof(buf),0)<=0){
            int i;
            for(i=0;i<LISTENQ;i++){
                if(i==n){
                    printf("client %d exit\n",connfd[n]);
                    connfd[i]=-1;
                    break;
                }
            }
            pthread_exit((void*)i);
      }
    
    printf("%s\n",buf);
    if(strcmp(buf,"./list")==0)   list(n);
    else   sendmsgall(buf,n);
    }
}
int main(){

    pthread_t thread;
    int listenfd,i;
    struct sockaddr_in serveradd;

    listenfd = socket(AF_INET,SOCK_STREAM,0);
    if(listenfd<0)  perror("socket");

    bzero(&serveradd,sizeof(serveradd));
    serveradd.sin_family = AF_INET;
    serveradd.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveradd.sin_port = htons(PORT);

    if(bind(listenfd,(struct sockaddr*) &serveradd, sizeof(serveradd))==-1) printf("bind error\n");
 
    listen(listenfd,LISTENQ);

    for(i=0;i<LISTENQ;i++)   connfd[i] = -1;
    connect_num=0;
    
    while(1){

        struct sockaddr_in cliadd;
        socklen_t len = sizeof(cliadd);
        int fd = accept(listenfd,(struct sockaddr*)&cliadd,&len);
        if(fd == -1){
                printf("Client Connect Failed\n");
                continue;
        }
       
        for(i=0;i<LISTENQ;i++){
           if(connfd[i]==-1){

                connfd[i] = fd;
                
                pthread_t tid;
                pthread_create(&tid,NULL,service_thread,&i);
                break;
           }
        }


    }
}


