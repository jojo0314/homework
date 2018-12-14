#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//#include <sys/type.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>
#define PORT 9877
#define MAXBUF 100
int fd;
char name[MAXBUF];
void* recv_thread(){

  
    while(1){    
       
        char buf[MAXBUF] = { };

        if(recv(fd,buf,sizeof(buf),0) <= 0){
            perror("read");
        }
        printf("\n");
        printf("%s\n",buf);
    
    }

}
int main(){

    struct sockaddr_in cli;
    
    fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd<0)    perror("socket");
    bzero(&cli,sizeof(cli));

    cli.sin_family = AF_INET;
    cli.sin_addr.s_addr =  inet_addr("127.0.0.1");
    cli.sin_port = htons(PORT);

    if(connect(fd,(struct sockaddr *)&cli, sizeof(cli))==-1){
            printf("Connect error\n");
    }
     
    pthread_t tid;
    char pd[MAXBUF];
    if(pthread_create(&tid,0,recv_thread,0)!=0)  perror("pthread_create");
    //pthread_join(tid,0);
    printf("Connect to ChatRoom\n");
    printf("UserName:");
    scanf("%s", name);
    printf("PassWord:");
    scanf("%s",pd);
    send(fd,name,strlen(name),0);
    printf("================\n");
    printf("%s>",name);  
    while(1){
            char buf[MAXBUF] = {};
            char msg[MAXBUF] = {};
     
            sprintf(msg,"%s",name);
        
            scanf("%s", buf);
            if(strcmp(buf,"./list")==0){
                send(fd,"./list",6,0);
                printf("%s>",name); 
                continue;
            }
            strcat(msg,">");
            strcat(msg,buf);
            //eroprintf("%s",msg);
            send(fd,msg,strlen(msg),0);
            printf("%s>",name); 
            memset(buf,0,sizeof(buf));
        
    }
    close(fd);
    
}