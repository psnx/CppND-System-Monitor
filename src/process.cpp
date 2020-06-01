#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "linux_parser.h"
#include "password.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, Password* pwd_pointer)
    : pid_(pid),
      process_status(LinuxParser::ReadProcessStatus(pid)),
      stat_(LinuxParser::ReadProcessStat(pid)),
      pwd_(pwd_pointer) {}

int Process::Pid() const { return pid_; }

void Process::Update() {
  process_status = LinuxParser::ReadProcessStatus(pid_);
  stat_ = LinuxParser::ReadProcessStat(pid_);
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  long active = static_cast<float>(this->ActiveJiffies());
  long total = static_cast<float>(LinuxParser::Jiffies());
  float utilization = static_cast<float>((active - preciding_active_) /
                                         (total - preciding_total_));
  preciding_active_ = active;
  preciding_total_ = total;
  return utilization;
}

long Process::ActiveJiffies() {
  long active{0};
  active += stol(stat_[eProcess_stat::user]);
  active += stol(stat_[eProcess_stat::kernel]);
  active += stol(stat_[eProcess_stat::children_user]);
  active += stol(stat_[eProcess_stat::children_kernel]);
  return active;
}

// TODO: Return the command that generated this process
string Process::Command() {
  return "[" + process_status["Name"] + " (" + process_status["State"].at(0) +
         ")" + "]: " + LinuxParser::Command(Pid());
}

string Process::Ram() { 
  string vms_size = process_status["VmSize"]; 
  std::istringstream ss(vms_size);
  std::string token;
  ss >> token;
  return to_string(stof(token)/1000).substr(0,4);
}

string Process::User() { 
  std::string entry = process_status["Uid"];
  std::istringstream ss(entry);
  std::string token;
  return (ss>>token) ? (*pwd_).FindUsername(stoi(token)) : "-";
}

long int Process::UpTime() { 
  long int ticks = stol(stat_[eProcess_stat::up_time]);
  return ticks / sysconf(_SC_CLK_TCK); //seconds
}

bool Process::operator<(Process const& a) const { return a.Pid() < this->pid_; }
bool Process::operator>(Process const& a) const { return a.Pid() > this->pid_; }
