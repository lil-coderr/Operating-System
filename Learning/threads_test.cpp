#include <iostream>
#include <pthread.h> 
#include <unistd.h>
#include <time.h>


using namespace std ; 

// void* routine(void* arg){
//         cout << "thread executing\n" << endl;
//         cout << getpid() << endl ;
//         sleep(3);
//         cout << "thread finished\n" << endl;
// }


// void* roll_dice(void * args){
//     int value = (rand() %6 ) +1 ; 
//     int* res = new int ; 
//     *res = value ; 
//     return res;     
// }

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int x = 0 ;  
void* thread_increments(void* args){
    sleep(1);
    printf("im a thread %lu i just entered the routine \n " , pthread_self() );
    
    sleep(1);
    pthread_mutex_lock(&mutex);
    printf("im a thread %lu i just entered the Critical section \n " , pthread_self() );
    x++;  
    pthread_mutex_unlock(&mutex);

    sleep(1);
    
}



int main (int* argc, char* argv[]){

    // pthread_t t1 ; 
    // pthread_t t2; 


    // if (pthread_create(&t1 , NULL , &routine, NULL) != 0 ){
    //     cout << "failed to create the thread";
    // } 

    // if(pthread_create(&t2, NULL , &routine , NULL ) != 0){
    //     cout<< "fail to creat the thread" << endl; 
    // } 

    
    // pthread_join(t1,NULL); 
    // pthread_join(t2 , NULL);



    ///// getting value back ////////////////////


    // srand(time(NULL));
    // pthread_t t1 ; 

    // int* results ; 

    // if ( pthread_create(&t1 , NULL , &roll_dice, NULL) !=0 ) {
    //     cout << "thread failed to create" << endl; 
    // }
    // if (pthread_join(t1, (void**) &results) != 0) {

    //     cout << "thread failed to terminate"<< endl ; 

    // } 

    // cout << *results << endl; 
    pthread_t thread[10]; 

    for (int i = 0 ; i < 4 ; i ++ ){
        
        pthread_create(&thread[i] ,NULL , &thread_increments, NULL) ;

    }

    for (int i = 0 ; i < 10 ; i ++ ){
        
        pthread_join(thread[i] ,NULL) ;
        
    }



    return 0 ; 
    

}