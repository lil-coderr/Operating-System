#include <iostream>
#include <pthread.h> 
#include <unistd.h>



using namespace std ; 

void* routine(void* arg){

        cout << "thread executing\n" << endl;

        cout << getpid() << endl ;
        sleep(3);
        
        cout << "thread finished\n" << endl;
}




int main (int* argc, char* argv[]){

    pthread_t t1 ; 
    pthread_t t2; 


    if (pthread_create(&t1 , NULL , &routine, NULL) != 0 ){
        cout << "failed to create the thread";
    } 

    if(pthread_create(&t2, NULL , &routine , NULL ) != 0){
        cout<< "fail to creat the thread" << endl; 
    } 

    
    pthread_join(t1,NULL); 
    pthread_join(t2 , NULL);


    return 0 ; 
    

}