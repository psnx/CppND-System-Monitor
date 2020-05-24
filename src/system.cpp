#include "system.h"

#include <unistd.h>

#include <algorithm>
#include <cstddef>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  auto pids_to_create = GetNewPids();
  for (auto p : pids_to_create) {
    processes_.push_back(p);
  }
  return processes_;
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

std::unordered_set<int> System::GetNewPids() {
  std::vector<int> pids = LinuxParser::Pids();
  std::unordered_set<int> pids_to_create{};
  std::vector<int> existent_pids = {};
  for (auto process : processes_) {
    existent_pids.push_back(process.Pid());
  };

  for (auto pid : pids) {
    auto it = std::find(existent_pids.begin(), existent_pids.end(), pid);
    if (it == existent_pids.end()) {
      pids_to_create.insert(pid);
    }
  };
  return pids_to_create;
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return 0.0; }

std::string System::OperatingSystem() { return LinuxParser::OS("PRETTY_NAME"); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::Pids().size(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return 0; }

long int System::UpTime() { return LinuxParser::UpTime(); }