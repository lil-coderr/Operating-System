#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std ;

int main(int argc, char *argv[])
{
    
    // create a socket 

    //connect 
    int connfd ; 
    struct sockaddr_in serv_addr; 
    connfd = socket(AF_INET,SOCK_STREAM,0);
    // Change an IP address as a string into a network address
    serv_addr.sin_addr.s_addr = inet_addr(”127.0.0.1”);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8888);
    if(connect(connfd, (struct sockaddr *)&serv_addr,
    sizeof(serv_addr))<0)
    { perror("Connect"); return 1; }
    // send messages read/ write 





    // close 












    return 0;
}
