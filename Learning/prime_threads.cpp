//find all the prime numbers between a and b
// using n process
// write result to out.txt
#include <iostream>
#include <fstream>
#include <string>
#include<unistd.h>
#include <pthread.h>
#include <queue>
#include <ctime>
#include <semaphore.h>
#include <fcntl.h> // for file open

using namespace std;


//global buffer
queue<int> buffer;

//pthread_cond_t condprint;
sem_t mySemphore; 

bool isPrime(int num){

// to check if x is a prime or not loop from 2 to x/2 and check if its divisble 

    int mid = num/2 +1;

    for (int i = 2; i <= mid; i++){

        if( (num % i)== 0){
            return false; 
        }
    }

    return true;
}

void* findPrime(void* args){

    int range_start,range_end; 
    //type casting
    range_start = *((int *)args);
    range_end = *((int *)args+1);

    //iterate and check range
    for(int i = range_start ; i <=range_end ; i++ ){
        if ( isPrime(i)){
            
            //add to buffer 
            sem_wait(&mySemphore);
            ///// critical section 
            //printf("found prime \n");
            cout << i << "  " ;
            //send signal to writing thread(which is the one waiting)
            //pthread_cond_signal(&condprint);
            sem_post(&mySemphore); //bufFull signal that buf may be full
        }   
    } 

    sem_wait(&mySemphore);
        cout << endl; 
    sem_post(&mySemphore);

    delete[] args; 

}


int main(){
    clock_t start = clock();
    int a = 1; 
    int b = 400000; 

    // cout << "Enter the number of threads to use ?" << endl; 

    int n = 8; // number of threads
    //cin >> n ; 

    // segment a & b using n 
    int segments = b/n ; // gets the integer part only 

    int outFile = open("prime_thread_output.txt" , O_WRONLY | O_CREAT | O_TRUNC, 0644);

    dup2(outFile, STDOUT_FILENO);

    //pthread_mutex_init(&mutex,NULL);
    //pthread_cond_init(&condprint,NULL);

    sem_init(&mySemphore , 0 ,1);
    pthread_t threads[n]; // number of threads

    int lower = a;
    for(int i = 0 ; i<n ; i++){

            int upper = lower + segments;
            if( upper > b ) {
                upper = b;
            }

            int* args_find_prime = new int[2];
            args_find_prime[0] = lower;
            args_find_prime[1] = upper; 


            pthread_create(&threads[i], NULL , &findPrime ,  (void*)args_find_prime) ; 
            lower = upper + 1;
        

    }

    //wait till all thread are complete
    for(int i = 0; i< n ; i++){

        pthread_join(threads[i] , NULL); 
    }

    //pthread_mutex_destroy(&mutex);
    //pthread_cond_destroy(&condprint);

   // write_buf_tofile();
    fflush(stdout);

    dup2(outFile, STDOUT_FILENO);
    fflush(stdout);

    close(outFile);
    clock_t end = clock();
    double elapsed = 1000.0 * (end - start) / CLOCKS_PER_SEC;
    cout << "Time taken: " << elapsed << " ms" << endl;

    return 0; 
}