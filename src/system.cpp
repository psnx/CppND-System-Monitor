#include "system.h"

#include <unistd.h>

#include <algorithm>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "password.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() {
  Update();
  pwd = Password(); //TODO: Implement some update mechanism?
  return this->processes_;
}

void System::Update() {
  AddNewProcesses();
  RemoveTerminatedProcesses(ExpiringPids());
  std::for_each(processes_.begin(), processes_.end(), 
    [&](auto p){p.Update();});
}
void System::AddNewProcesses() {
  std::set<int> nascent_pids =
      RelativeComplement(LinuxParser::Pids(), ExistentPids());
  std::for_each(nascent_pids.begin(), nascent_pids.end(),
                [&](auto& pid) { processes_.push_back(Process(pid, &pwd)); });
}
std::set<int> System::ExpiringPids() {
  std::set<int> expiring_pids =
      RelativeComplement(ExistentPids(), LinuxParser::Pids());
  std::for_each(
      expiring_pids.begin(), expiring_pids.end(),
      [&expiring_pids](const auto& pid) { expiring_pids.insert(pid); });
  return expiring_pids;
}
void System::RemoveTerminatedProcesses(std::set<int> expired_pids) {
  for (auto pid : expired_pids) {
    auto it = std::remove_if(processes_.begin(), processes_.end(),
                             [&](auto& p) { return p.Pid() == pid; });
    processes_.erase(it, processes_.end());
  }
}

std::set<int> System::ExistentPids() const {
  std::set<int> pids{};
  std::for_each(begin(processes_), end(processes_),
                [&pids](auto& p) { pids.insert(p.Pid()); });
  return pids;
}

std::set<int> System::RelativeComplement(const std::set<int>& one,
                                         const std::set<int>& two) {
  std::set<int> diff{};
  std::set_difference(one.begin(), one.end(), two.begin(), two.end(),
                      std::inserter(diff, diff.begin()));
  return diff;
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { 
  return LinuxParser::MemoryUtilization();
 }

std::string System::OperatingSystem() { return LinuxParser::OS("PRETTY_NAME"); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { 
  return Cpu().RunningProcesses(); 
}

int System::TotalProcesses() { 
  return Cpu().TotalProcesses();
 }

long int System::UpTime() { return LinuxParser::UpTime(); }
