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
  float active = static_cast<float>(this->ActiveJiffies());
  float total = static_cast<float>(LinuxParser::Jiffies());
  float d_total = total - preciding_total_;
  float d_active = active - preciding_active_;
  if (d_total > 10 && d_active > 2){
    preciding_active_ = active;
    preciding_total_ = total;
    preciding_utilization = d_active / d_total;
    return preciding_utilization;
  } else if (preciding_utilization == 0){
    preciding_utilization = active / total;
    return preciding_utilization;
  }
  return preciding_utilization;
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
  auto it = process_status.find("VmSize");
  if (it == process_status.end()){
    process_status["VmSize"]= "0 kB";
  }
  string vms_size = process_status["VmSize"]; 
  std::istringstream ss(vms_size);
  std::string token;
  ss >> token;
  return to_string(stoi(token)/1024);
}

string Process::User() { 
  std::string entry = process_status["Uid"];
  std::istringstream ss(entry);
  std::string token;
  return (ss>>token) ? (*pwd_).GetUserNameShorten(stoi(token), 6) : "-";
}

long int Process::UpTime() { 
  long int ticks = stol(stat_[eProcess_stat::up_time]);
  return ticks / sysconf(_SC_CLK_TCK); //seconds
}


bool Process::operator<(Process& a) {
  return this->CpuUtilization() < a.CpuUtilization();
}

bool Process::operator>(Process& a) {
  return this->CpuUtilization() > a.CpuUtilization();
}