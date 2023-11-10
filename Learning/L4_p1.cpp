/*
Summing numbers from an array using threads 
*/


#include <pthread.h>
#include <iostream> 

using namespace std ; 


//array of numbers --althought it is a shared array no issues since the only operation 
//involved is reading no writing to array(i.e no race conditions)
int array_sum[10] = {1,2,3,4,5,6,7,8,9,10}; 
int total = 10; 


/// needs both void* return type and Void type input parameter
//args to routine = array , which half to sum
void* routine_sum(void* args){

    int segments = *((int*)args); 
    
    int sum = 0; 
    for(int i =0 ; i< 5; i++){

        sum = sum +  array_sum[segments+i];        
    }

    delete args; 

    int* result = new int; 
    *result = sum; 

    return result; 

}



int main (int* argc, char* argv[]){
    //declaring thread IDS
    pthread_t t_ids[2]; 


    //create 2 threads in a loop 
    for(int i = 0 ; i < 2 ; i++){
        

        //need to pass index i to thread must use heap created mem address
        int* segments = new int; 

        *segments = i*5;  

        
        if (pthread_create(&t_ids[i],NULL, &routine_sum, segments) != 0){
            printf("failed creating thread");
        }
        
    }

    int* res; 
    // waiting for all the threads to terminate
    for(int i = 0 ; i < 2 ; i++){
       
        if (pthread_join(t_ids[i],(void**) &res) != 0){

            printf("failed creating thread");
            
        }

         cout << *res << endl; 

        delete res ; 
        
        
    }

    return 0; 
}