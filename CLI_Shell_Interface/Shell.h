#ifndef Shell_H
#define Shell_H

#include <sys/wait.h>
#include<iostream> 
#include <string>
#include <vector>
#include <unistd.h>
#include <unordered_map> 
#include <sstream>
#include <sys/time.h>
#include <sys/resource.h>
#include <signal.h>
#include <fcntl.h>
using namespace std; 

struct process_info {
    int process_num; 
    int pid; 
    char status; 
    int sec_taken; // use ps to get this 
    string command; 

    //construct when creating a process status intialized to R(running)
    process_info(){};
    process_info(int pn, string cmd , char s = 'R'): process_num(pn), command(cmd) , status(s) {} 
};


class Shell{

    public: 
        void run();


    private: 

        /*  Display the status of all running processes spawned by Shell. See
            the print format below in the example.*/ 
        void  jobs_cmd();

        /*  Kill process <pid>.*/ 
        void  kill_cmd(const int pid );
        
        /* Resume the execution of process <pid>. This undoes a suspend*/ 
        void  resume_cmd(const int pid);
        
        /* Sleep for <int> seconds.*/ 
        void  sleep_cmd(const int seconds);
        
        /*  Suspend execution of process <pid>. A resume will reawaken it*/ 
        void  suspend_cmd(const int pid);
        
        /*  Suspend execution of process <pid>. A resume will reawaken it*/ 
        void  wait_cmd(const int pid);
        
        /*  Spawn a process to execute command <cmd> with 0 or more arguments <arg>.
        <cmd> and <arg> are each one or more sequences of nonblank characters*/ 
        void  execute_cmd(vector<string> parsed_input, const string user_command);
        // return true if command entered in is right format 
        void command_handler(vector<string>,const string user_cmd);
        vector<string> get_commands(string user_command);

        void special_args_handler(bool &redirectInput, bool &redirectOutput, vector<string> &parsed_input,
                                    string &inputFileName, string &outputFileName);

        bool is_output_redirected();



// input string can have only one < and only one > special argument within input string 
// since there is only one program input and only one program output
//void special_arg_handler(string x){

    unordered_map<int,process_info> process_control_block; 
    int process_counter; 
    
 };



#endif
