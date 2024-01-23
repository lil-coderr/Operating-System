#include <iostream> 
#include <pthread.h> 
#include <semaphore.h>
#include <queue> 
#include <time.h>
#include<unistd.h>

using namespace std ; 
int NUM_threads = 4;

//shared buffer 
queue<int> bounded_buffer;
pthread_mutex_t bufMutex = PTHREAD_MUTEX_INITIALIZER;
sem_t emptyslots ; 
sem_t fullslots; 


void* producer(void*){

    int value;
    while(1){


        //add to buffer 
        sem_wait(&emptyslots); //empt--

        pthread_mutex_lock(&bufMutex);
        value= (rand() %6 ) +1 ; 
        bounded_buffer.push(value);

        printf("rolled a die \n");
        fflush(stdout);
        
        sleep(1);
        pthread_mutex_unlock(&bufMutex);


        sem_post(&fullslots); //full ++



    }




}


void* consumer(void*){

    //remove from buffer 
    int value;
    while(1){


         //remove from buffer 
        sem_wait(&fullslots); //full--

        pthread_mutex_lock(&bufMutex);
        value= bounded_buffer.front();
        bounded_buffer.pop();

        printf("reacieved numeber %d \n",value );
        fflush(stdout);
        
        pthread_mutex_unlock(&bufMutex);


        sem_post(&emptyslots); //empty ++
        sleep(1);


    }
    


}


int main( int argc , char* argv[]){

    srand(time(NULL));
    pthread_t threads[NUM_threads];

    sem_init(&emptyslots,0 , 2);
    sem_init(&fullslots, 0 ,0);

    printf("creating threads\n");
    for(int i = 0 ; i<NUM_threads ; i++ ){

        if(i % 2 == 0){
            if (pthread_create(&threads[i], NULL , &producer, NULL ) != 0){
                perror("failed to create the thread");
            }
        }else{

            if (pthread_create(&threads[i], NULL , &consumer, NULL ) != 0){
                perror("failed to create the thread");
            }
        }

    }



    for(int i = 0 ; i<NUM_threads ; i++ ){

        if(pthread_join(threads[i], NULL) != 0){
            perror("failed to terminate he thead");
        } 

    }



}