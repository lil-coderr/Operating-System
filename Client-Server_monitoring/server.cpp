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
using namespace std;;


int main(int argc, char* argv[]) {


   // creating buffer for communication
    char buffer[1024] = {0};
    int port = 8888;
    // creating socket 
    int serverfd; 
    if( serverfd = socket(AF_INET,SOCK_STREAM,0)    ){
        perror("failed to create socket ");

    }
    
    // coneect 
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET; 
    server_addr.sin_addr.s_addr= htonl(INADDR_ANY);
    server_addr.sin_port = htons(8888);

    if(bind(serverfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("failed to bind");

    }
    //listen
    
    if( listen( serverfd, 10) < 0 ){
        perror("failed to listen");
    }

    

    //accept
    int newFDforclient;

    //POLLING FOR messages
    while(true){
        if((newFDforclient = accept(serverfd, (struct sockaddr *)&server_addr , (socklen_t*)&server_addr))){
            perror("lets go ");


        }

        read(newFDforclient,buffer , 1024);
        cout<< " messag recieved for client " << buffer << endl ;

        close(newFDforclient);
    }

 


    // write/ read()



    // close

    return 0;
}