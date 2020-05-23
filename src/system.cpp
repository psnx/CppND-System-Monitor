#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <set>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() {
    return cpu_; 
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
  std::vector<int> pids = LinuxParser::Pids();
  processes_.clear();
  for (int p : pids)
  {
    processes_.push_back(Process(p));
  }
  return processes_; 
}

std::string System::Kernel() { 
    return LinuxParser::Kernel(); 
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return 0.0; }

std::string System::OperatingSystem() { 
    return LinuxParser::OS("PRETTY_NAME"); 
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { 
  return LinuxParser::Pids().size();
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return 0; }

long int System::UpTime() { 
  return LinuxParser::UpTime();
 }