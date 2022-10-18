#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<ctype.h>
#include<fcntl.h>
#define SERV_PORT 9527

void sys_error(const char* str){
perror(str);
exit(0);
}


int main(int argc,char *argv[]){

     fflush(stdout);
     setvbuf(stdout,NULL,_IONBF,0);
     int save_fd = dup(STDOUT_FILENO); // 保存标准输出 文件描述符 注:这里一定要用 dup 复制一个文件描述符. 不要用 = 就像是Winodws下的句柄.
     int fd = open("test1.txt",(O_RDWR | O_CREAT), 0644);
     dup2(fd,STDOUT_FILENO); // 用我们新打开的文件描述符替换掉 标准输出
     dup2(save_fd,STDOUT_FILENO);	 


int lfd,cfd=0;
char buf[BUFSIZ],client_IP[1024];
int len;
struct sockaddr_in serv_addr,clit_addr;
socklen_t clit_addr_len;

serv_addr.sin_family=AF_INET;
serv_addr.sin_port=htons(SERV_PORT);
serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);

lfd=socket(AF_INET,SOCK_STREAM,0);
if(lfd==-1){
sys_error("socket error");
}

int ret=bind(lfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
if(ret==-1){
sys_error("bind error");
}
ret=listen(lfd,128);
if(ret==-1){
sys_error("listen error");
}
clit_addr_len=sizeof(clit_addr);

cfd=accept(lfd,(struct sockaddr*)&clit_addr,&clit_addr_len);
if(cfd==-1){
sys_error("accept error");
}

printf("client ip:%s port:%d\n",inet_ntop(AF_INET,&clit_addr.sin_addr.s_addr,client_IP,sizeof(client_IP)), ntohs(clit_addr.sin_port));
while(1){


len=read(cfd,buf,sizeof(buf));
write(STDOUT_FILENO,buf,len);
int i;
for(i=0;i<len;i++){
buf[i]=toupper(buf[i]);
}
write(cfd,buf,len);
}
close(lfd);
close(cfd);
return 0;   }
