#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid)
    : pid_(pid),
      process_status(LinuxParser::ReadProcessStatus(pid)),
      stat(LinuxParser::ReadStat(pid)) {}

int Process::Pid() const { return pid_; }

void Process::Update() {
  process_status = LinuxParser::ReadProcessStatus(pid_);
  stat = LinuxParser::ReadStat(pid_);
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  return static_cast<float>(this->ActiveJiffies()) /
         static_cast<float>(LinuxParser::Jiffies());
}

long Process::ActiveJiffies() {
  long active{0};
  active += stol(stat[eProcess_stat::user]);
  active += stol(stat[eProcess_stat::kernel]);
  active += stol(stat[eProcess_stat::children_user]);
  active += stol(stat[eProcess_stat::children_kernel]);
  return active;
}

// TODO: Return the command that generated this process
string Process::Command() {
  return "[" + process_status["Name"] + " (" + process_status["State"].at(0) +
         ") " + "]: " + LinuxParser::Command(Pid());
}

// TODO: Return this process's memory utilization
string Process::Ram() { return process_status["VmSize"]; }

// TODO: Return the user (name) that generated this process
string Process::User() { return process_status["Uid"]; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { return a < this->pid_; }
bool Process::operator>(Process const& a) const { return a > this->pid_; }
