#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid), process_state(LinuxParser::ReadProcessStatus(pid)) {}

int Process::Pid() const { return pid_;}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    return 0; 
}

// TODO: Return the command that generated this process
string Process::Command() { 
   return "["+ process_state["Name"] + "]: " + LinuxParser::Command(Pid());
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
    return "11";
    //return process_state["VmSize"];
 }

// TODO: Return the user (name) that generated this process
string Process::User() { 
    return "T";
    //return process_state["Uid"];
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime(Pid());
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }