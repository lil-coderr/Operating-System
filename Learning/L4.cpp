#include <pthread.h>
#include <iostream> 

using namespace std ; 


//array of numbers --althought it is a shared array no issues since the only operation 
//involved is reading no writing to array(i.e no race conditions)
int array_sum[10] = {1,2,3,4,5,6,7,8,9 , 10}; 


/// needs both void* return type and Void type input parameter
//args to routine = array , which half to sum
void* routine_sum(void* args){

    int* arry_to_sum = NULL; 
    int segments; 

   //get the array 
    arry_to_sum = ((int*)args+0);

    segments = *((int*)args+1);


    int sum = 0; 
    for(int i =0 ; i< (10/segments) ; i++){

        sum = sum +  arry_to_sum[i];        
    }

    int* result = new int; 

    return &result; 

}



int main (int* argc, char* argv[]){


    //declaring thread IDS
    pthread_t t_ids[2]; 



    //create 2 threads in a loop 
    for(int i = 0 ; i < 2 ; i++){


        void* args_array[2] = {array_sum,&i};


        if (pthread_create(&t_ids[i],NULL, &routine_sum, &args_array) != 0){
            printf("failed creating thread");
        }
        
    }


    // waiting for all the threads to terminate
    for(int i = 0 ; i < 2 ; i++){
       
        if (pthread_join(t_ids[i],NULL) != 0){

            printf("failed creating thread");
        }
        
    }


    return 0; 
}