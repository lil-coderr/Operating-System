//find all the prime numbers between a and b
// using n process
// write result to out.txt
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include<unistd.h>
#include <sys/wait.h>
#include <stdio.h>
using namespace std; 

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

void findPrime(int process_num,int range_start, int range_end ){

    ofstream myfile; 

    // format filename
    string filename = "out" + to_string(process_num) + ".txt";

    myfile.open(filename,ios::out | ios::app | ios::binary);

    for(int i = range_start ; i <=range_end ; i++ ){

        if ( isPrime(i)){
            //write to file 
            myfile << i << ", ";

        }

    }

    myfile<< endl; 
    myfile.close();

}

int main(){
clock_t start = clock();
int a = 1; 
int b = 400000; 

// get pids
int parent_pid = getpid(); 

cout << "Enter the number of process to use ?" << endl; 

int n; // number of process

cin >> n ; 

// segment a & b using n 
int segments = b/n ; // gets the integer part only 

unordered_map<int,pair<int,int>> process_segments; 


process_segments[1].first = 1 ;
process_segments[1].second = 1*segments;

for( int i =2 ; i <=n ; i++){
    
    process_segments[i].first = process_segments[i-1].second + 1;
    process_segments[i].second = i*segments;

}

process_segments[n].second = b; 

// create N process 
//  1
//  |
//  |\
//  | 2
//  |\
//  | 3
//  |\
//  | 4
//  |\
//  | 5
// etc
pair<int, int> currpid;
currpid.first = parent_pid; 
currpid.second = 1; 
for( int i = 2 ; i <= n ; i++){
    
    int pid = fork();
     // if its a child process then break 
    if(pid == 0){

        currpid.first = getpid();
        currpid.second = i;
        
        break; 
    }
    
}


// all children and parent execute the following differently 
int process_num = currpid.second;

findPrime( process_num , process_segments[process_num].first , process_segments[process_num].second); 

// excute next section only if its in the parent process 
if ( getpid() == parent_pid){

    //wait until all children finish executing 

    while( wait(NULL) > 0);


    // create and open the final output file 
    ofstream finalfile; 
    finalfile.open("out.txt",ios::out | ios::app | ios::binary);



    for(int i = 1; i <= n ; i++){

        string filename = "out" + to_string(i) + ".txt";

        ifstream process_output; 

        process_output.open(filename, ios::binary);

        finalfile << process_output.rdbuf();
        
        process_output.close(); 
        remove(filename.c_str()); 
    }

    finalfile.close();

    clock_t end = clock();
    double elapsed = 1000.0 * (end - start) / CLOCKS_PER_SEC;
    cout << "Time taken: " << elapsed << " ms" << endl;

    

}

}