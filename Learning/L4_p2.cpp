
#include <iostream> 
#include <pthread.h> 
#include <unistd.h>

using namespace std ;

pthread_mutex_t mutexFuel = PTHREAD_MUTEX_INITIALIZER;
int fuel = 0; 


void* fuel_filling(void* args){

    for (int i =0 ; i< 5 ; i++){

        pthread_mutex_lock(&mutexFuel); // when a thread reaches here a single thread get the lock and the rest wait 
        fuel+=15;
        pthread_mutex_unlock(&mutexFuel);// that single thread realse the lock and the next thread can enter
        printf( "filledd fuel... %d\n", fuel );
        sleep(1);

    }

}

void* car(void* args){
    pthread_mutex_lock(&mutexFuel);
    if(fuel >= 40){
        fuel -= 40;
        printf( "got fuel, now left: %d\n", fuel );
    }
    else{
        printf("No fuel\n");
    }

    pthread_mutex_unlock(&mutexFuel);

}

int main(int* argc , char* argv[] ){

    pthread_t thread[2];

    for(int i =0 ; i<2 ; i++ ){

        if( i==1){
            if( pthread_create(&thread[i], NULL , &fuel_filling, NULL) != 0 ){
                cout << " failed to create the thread" << endl;
            }
        }
        else{
            if( pthread_create(&thread[i], NULL , &car, NULL) != 0 ){
                cout << " failed to create the thread" << endl;
            }
        }
    } 


    for(int i =0 ; i<2 ; i++ ){

  
        if( pthread_join(thread[i], NULL ) != 0 ){
            cout << " failed to terminate the thread" << endl;
        }
        
    }       
 
}

