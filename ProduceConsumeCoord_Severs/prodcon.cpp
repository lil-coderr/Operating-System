#include <iostream> 
#include <pthread.h>
#include <queue>
#include <string> 
#include "tands.cpp" 
#include <fcntl.h> // for file open
#include <unistd.h> //dup2
#include <chrono> 
#include <iomanip> // For setprecision
#include <semaphore.h>
using namespace std; 

bool DONE = false; 
// Global start time variable
chrono::high_resolution_clock::time_point startTime;
//global bounded-buffer Through code need to ensure the size is constant
queue<int> bounded_buffer; //{T5,T6,1}  1 means work in buffer and MAXSIZE = 2*nConsumers

pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;
sem_t semEmpty;
sem_t semFull;

void logTime() {
    // Get the current time as the end time
    auto endTime = chrono::high_resolution_clock::now();
    // Calculate elapsed time in seconds as a double with three decimal places
    double elapsedSeconds = chrono::duration<double>(endTime - startTime).count();

    cout<< fixed << setprecision (3) << elapsedSeconds << " ";
}

void* prod_routine(void* args){

    int nConsumer = *((int*)args);
    int Maxsize = nConsumer*2;
    string line ;
    int work, n;  
    while(getline(cin, line)){
        if (line[0] == 'T') {
            // Handle T<n> command
            work = stoi(line.substr(1)); // Convert string to integer.



            while(bounded_buffer.size() >  Maxsize){
                //just wait until when the buffer is greater that maxsize
            }


            // Add to the buffer
            sem_wait(&semEmpty);
            pthread_mutex_lock(&queueMutex);
            
            bounded_buffer.push(work); 

            logTime();
            cout << "ID= 0 Q= " << bounded_buffer.size() << " Work      " << work << endl;
            pthread_mutex_unlock(&queueMutex);
            sem_post(&semFull);


        } 
        //// Handle S<n> command
        else if (line[0] == 'S') {
            n = stoi(line.substr(1)); // Convert string to integer.
           
            if (n >= 1 && n <= 100) {
                
                logTime();
                cout << "ID= 0      Sleep     " << n << endl;
                Sleep(n);

            } else {
                cerr << "Error: Sleep duration must be between 1 and 100." << endl;
            }
        } else {
            cerr << "Error: Invalid command." << endl;
        }
    
    }
    logTime();
    cout << "ID= 0      END "<< endl;
    DONE = true;
}

/*
    args = dynamically allocated array
    args[2]= {Thread_Num , MaxSize}

*/
void* cons_routine(void* args){

    int Thread_id = *((int*)args+0);
    int MaxSize = *((int*)args+1);

    while(!DONE){

        // Remove from the buffer
        logTime();
        cout << "ID= " << Thread_id << "      Ask" << endl;
        sem_wait(&semFull);

        pthread_mutex_lock(&queueMutex);

        int n = bounded_buffer.front();  
        bounded_buffer.pop() ; 

        logTime();
        cout << "ID= " << Thread_id << " Q= " << bounded_buffer.size() << " Receive   " << n << endl;

        pthread_mutex_unlock(&queueMutex);
        sem_post(&semEmpty);

        Trans(n); 
        logTime();
        cout << "ID= " << Thread_id << "      Complete  " << n << endl;
    } 
    delete[] args; 
}


int main(int argc , char* argv[]) {
    // Get the start time at the beginning of the program
    startTime = chrono::high_resolution_clock::now();

    if (argc < 2 || argc > 3) {
        cerr << "Usage: " << argv[0] << " nthreads [id]" << endl;
        return 1;
    }

    // read the number of consumers 
    int nConsumers = stoi(argv[1]);

    string logFilename; 
    if (argc == 3){
        //id given there for create the log with corresponding id
        logFilename = "prodcon." + string((argv[2])) + ".log.txt";   
    }
    else if (argc == 2)
    {
        //create the log with default id 0
        logFilename = "prodcon.0.log.txt";
    }
    
    int logFile = open(logFilename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (logFile == -1) {
        perror("Error opening output file");
        return 1; 
    }

    //to make easier to write to file and not require to implement an I/O buffer write file use STDOUT
    dup2(logFile , STDOUT_FILENO);

    sem_init(&semEmpty, 0, nConsumers*2);
    sem_init(&semFull, 0, 0);

    pthread_t threads[nConsumers +1];
    //create n consumer and 1 producer
    for(int i = 0 ; i<(nConsumers +1) ; i++){


        int* consumer_args = new int[2]; 
        consumer_args[0] = i+1;
        consumer_args[1] = nConsumers*2;


        if( i == nConsumers){
            //create the producer thread (i.e my server)
            pthread_create(&threads[i], NULL , prod_routine , &nConsumers);
            
        }else{
            //create the consumer threads( i.e excute work given by server)
            pthread_create(&threads[i], NULL , cons_routine, consumer_args );
        }
    }
    
    //termintae all the threads
    for(int i = 0 ; i<(nConsumers +1)  ; i++){
        pthread_join(threads[i], NULL );
    }


     // Close the file.
    cout.flush();
    close(logFile);

    return 0; 

}