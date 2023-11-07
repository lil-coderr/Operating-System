//find all the prime numbers between a and b
// using n process
// write result to out.txt
#include <iostream>
#include <fstream>
#include <string>
#include<unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <pthread.h>
#include <queue>
#include <ctime>
using namespace std;


//global buffer
queue<int> buffer;

pthread_mutex_t mutex;
pthread_cond_t condprint;


void write_buf_tofile() {
    // Open the file for writing
    ofstream outFile("output.txt");
    
    // Check if the file is open
    if (!outFile.is_open()) {
        cerr << "Failed to open output.txt for writing." << endl;
        return;
    }
    
    // Write each integer from the queue to the file
    while (!buffer.empty()) {
        outFile << buffer.front() << " ," ;  // Write the integer followed by a newline
        buffer.pop();  // Remove the integer from the queue
    }
    outFile<<endl;
    // Close the file
    outFile.close();
}

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
            //copy to buffer 
            ///// critical section 
            pthread_mutex_lock(&mutex);
            //printf("found prime \n");
            buffer.push(i);
            pthread_mutex_unlock(&mutex);
            //send signal to writing thread(which is the one waiting)
            pthread_cond_signal(&condprint);

        }

    }

    delete[] args; 

}

void* print_to_file(void* args){

    pthread_mutex_lock(&mutex);
    if( buffer.size() < 4096){
    // allow other threads to run while condition is not true
        //printf("waiting to fill\n");
        pthread_cond_wait(&condprint, &mutex);
    } else {
       // printf("printing\n");
        write_buf_tofile();
    }
    
    //● When it returns the mutex will be locked and owned by that thread
    pthread_mutex_unlock(&mutex);

}


int main(){
    clock_t start = clock();
    int a = 1; 
    int b = 400000; 

    cout << "Enter the number of threads to use ?" << endl; 

    int n; // number of threads
    cin >> n ; 

    // segment a & b using n 
    int segments = b/n ; // gets the integer part only 

    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&condprint,NULL);
    

    pthread_t threads[n]; // number of threads
    //create 
    int* args_find_prime = new int[2];


    int lower = a;
    for(int i = 0 ; i<n ; i++){


        if(i == 0 ){
            //printing thread
            if( pthread_create(&threads[i],NULL, print_to_file, NULL) != 0){
                perror("failed to create threads");
            }

        }else{
            //rest are computing threads
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

    }

    //wait till all thread are complete
    for(int i = 0; i< n ; i++){

        pthread_join(threads[i] , NULL); 
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condprint);

    write_buf_tofile();

    clock_t end = clock();
    double elapsed = 1000.0 * (end - start) / CLOCKS_PER_SEC;
    cout << "Time taken: " << elapsed << " ms" << endl;

    return 0; 
}