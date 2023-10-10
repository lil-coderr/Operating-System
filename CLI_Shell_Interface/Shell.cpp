#include "shell379.h"

void Shell379::run(){

    this->process_counter = 0; 
    //// keeps polling for commands to come to the shell
    while(1){
        cout<<"SHELL379: ";
        //get comand from the user 
        string user_command ; 
        getline(cin, user_command); // reads full command line( until \n is found )
         // get the parsed user input 
        vector<string> parsed_commands = get_commands(user_command); 

        // if the input string was empty 
        if(parsed_commands.size() == 0){

            cout<< endl; 
            continue;
        }

        
        command_handler(parsed_commands,user_command);
        cout<<endl;
    }

}
    
void Shell379::command_handler(vector<string> cmds,string user_cmd){

    string command = cmds[0];
    int second_arg;;
    if (command == "jobs"){
        this->jobs_cmd();
        return; 
    }

    if (command == "kill" || command == "resume" || command == "sleep" || command == "suspend" || command == "wait" ){
        // these comand input shold be in this format. the second argument must be an integer
        // cmd <pid>. and there for second argument should be a vaild pid or seconds must be an interger 
        try
        {
            int second_arg = stoi(cmds[1]);
        }
        catch(const exception& e)
        {
            cout << command  << " command only accepts only one arg and must be integer" << e.what() << endl;
            return;
        }

    }
    
    if (command == "kill"){
        this->kill_cmd(second_arg);
        return; 
    }
     
    if (command == "resume"){
         this->resume_cmd(second_arg);
         return;

    }
    
    if (command == "sleep"){
        this->sleep_cmd(second_arg);
        return; 
    }

    if (command == "suspend"){
         this->suspend_cmd(second_arg);
         return; 
    }

    if (command == "wait"){
         this->wait_cmd(second_arg);
    }
    //for any other commands 
    this->execute_cmd(cmds,user_cmd);

};

vector<string> Shell379::get_commands(string user_command){

    vector<string> input_commands ; 
    // Check if the user_command is empty or consists of only whitespaces
    if (user_command.find_first_not_of(" \t\n") == string::npos) {
        return input_commands; // Return an empty vector
    }
    stringstream ss(user_command);

    string temp; 
    // use the extraction operator to extract each word into temp and append to the vector 
    while(ss >> temp){
        
        input_commands.push_back(temp); 
    }

    return input_commands;
};

void Shell379::jobs_cmd(){
    // Variables to store user and system time
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage); // get resource usage for the shell itself

    int total_user_time = usage.ru_utime.tv_sec;
    int total_system_time = usage.ru_stime.tv_sec;

    // include child processes:
    getrusage(RUSAGE_CHILDREN, &usage);
    total_user_time += usage.ru_utime.tv_sec;
    total_system_time += usage.ru_stime.tv_sec;

    // Printing running processes
    cout << "Running processes:\n";
    cout << "#  PID S SEC COMMAND\n";
    int running_processes = 0;

    for (const auto& entry : process_control_block) {
        const process_info& pi = entry.second;
        if (pi.status == 'R') {
            running_processes++;
            cout << pi.process_num << ": " << pi.pid << " " << pi.status << " " << pi.sec_taken << " " << pi.command << "\n";
        }
    }
    cout << "Processes = " << running_processes << " active\n";

    // Printing completed processes
    cout << "Completed processes:\n";
    cout << "User time = " << total_user_time << " seconds\n";
    cout << "Sys time = " << total_system_time << " seconds\n";


}

void Shell379::kill_cmd(const int pid){

    //check if the specified pid is in the pcb 
    if (this->process_control_block.find(pid) == this->process_control_block.end()) {
            // Process with given PID not found
            cout << "Error: Process with PID " << pid << " not found.\n";
            return;
        }

    // send kill signal to the corresponding pid    
    if (kill(pid, SIGKILL) == 0){

        //remove the proces from the process control block 
        this->process_control_block.erase(pid);
        this->process_counter--;

        return ; 
    }else{
        cout << "kill command fail for some reason" << endl; 
    }
}

void Shell379::suspend_cmd(const int pid){

    //check if the specified pid is in the pcb 
    if (this->process_control_block.find(pid) == this->process_control_block.end()) {
            // Process with given PID not found
            cout << "Error: Process with PID " << pid << " not found.\n";
            return;
        }
    
    if (kill(pid, SIGSTOP) == 0) { // send SIGSTOP signal
        cout << "Process with PID " << pid << " suspended.\n";

        // Update the status in your process table
        process_control_block[pid].status = 'S';  // 'S' for suspended

    }else{

        cout << "suspend command fail for some reason" << endl; 
    }


};

void Shell379::resume_cmd(const int pid){

    //check if the specified pid is in the pcb 
    if (this->process_control_block.find(pid) == this->process_control_block.end()) {
            // Process with given PID not found
            cout << "Error: Process with PID " << pid << " not found.\n";
            return;
        }
    
    if (kill(pid, SIGCONT) == 0) { //
        // Update the status in your process table
        process_control_block[pid].status = 'R';  // 'S' for suspended

    }else{

        cout << "resume command fail for some reason" << endl; 
    }


};

void Shell379::sleep_cmd(const int seconds){

    if (seconds < 0) {
        cout << "Error: Invalid sleep time provided.\n";
        return;
    }

    sleep(seconds);
    cout<<"SHELL379: ";
}

void Shell379::wait_cmd(const int pid){

    //check if the specified pid is in the pcb 
    if (this->process_control_block.find(pid) == this->process_control_block.end()) {
            // Process with given PID not found
            cout << "Error: Process with PID " << pid << " not found.\n";
            return;
        }
    
    int status;
    pid_t returnedPid = waitpid(pid, &status, 0);
    if (returnedPid == -1) {
        perror("Error waiting for process");
    } else {
        if (WIFEXITED(status)) {
            // Remove the process from process_control_block since it's completed
            process_control_block.erase(pid);
            this->process_counter--;
        }
    }

}

void Shell379::special_args_handler(bool &redirectInput, bool &redirectOutput, vector<string> &parsed_input,
                                    string &inputFileName, string &outputFileName){

 // Parse the command for redirection
    for (size_t i = 0; i < parsed_input.size(); ++i) {
        if (parsed_input[i] == "<" && i + 1 < parsed_input.size()) {

            redirectInput = true;
            inputFileName = parsed_input[i + 1];
            parsed_input.erase(parsed_input.begin() + i, parsed_input.begin() + i + 2);
            i--;  // Adjust index due to erasure
        } else if (parsed_input[i] == ">" && i + 1 < parsed_input.size()) {
            redirectOutput = true;
            outputFileName = parsed_input[i + 1];
            parsed_input.erase(parsed_input.begin() + i, parsed_input.begin() + i + 2);
            i--;  // Adjust index due to erasure
        }
    }

};

void Shell379::execute_cmd(vector<string> parsed_input,const string user_command ){

    // Decide if it's a background process based on the last argument
    bool isBackground = false;
    if (parsed_input.back() == "&") {
        isBackground = true;
        parsed_input.pop_back();  // Remove the '&' from parsed_input
    }
    
    // Convert vector<string> to char* array for execvp
    vector<char*> argv;
    for (const auto& arg : parsed_input) {
        argv.push_back(const_cast<char*>(arg.c_str()));
    }
    argv.push_back(nullptr);  // execvp expects a null-terminated array

    // Redirection flags and file names
    bool redirectInput = false, redirectOutput = false;
    string inputFileName, outputFileName;

    special_args_handler(redirectInput,redirectOutput,parsed_input,inputFileName,outputFileName);

    int pid = fork();

    if (pid < 0){
        printf("fork faild");
    }

    if(pid == 0 ){
        //child process
        // Handle input redirection
        if (redirectInput) {
            int fdIn = open(inputFileName.c_str(), O_RDONLY);
            if (fdIn == -1) {
                perror("Error opening input file");
                exit(EXIT_FAILURE);
            }
            dup2(fdIn, STDIN_FILENO);
            close(fdIn);
        }
        // Handle output redirection
        if (redirectOutput) {
            int fdOut = open(outputFileName.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fdOut == -1) {
                perror("Error opening output file");
                exit(EXIT_FAILURE);
            }
            dup2(fdOut, STDOUT_FILENO);
            close(fdOut);
        }

        // Execute the command
        if (execvp(parsed_input[0].c_str(), &argv[0]) == -1) {
            perror("Error executing command");
            exit(EXIT_FAILURE);  // End the child process
        }

    }else{
        // Parent process
        if (!isBackground) {
            int status;
            waitpid(pid, &status, 0);
            
        } else {
            // Update process table for background processes
            process_info process;
            process.pid = pid;
            process.process_num = this->process_counter;
            process.command = user_command;  // Assuming the command is the first string in command_args
            process.status = 'R';  // Running
            process_control_block[pid] = process;
        }


    }


}
