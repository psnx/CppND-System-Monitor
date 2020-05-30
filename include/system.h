#ifndef SYSTEM_H
#define SYSTEM_H

#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();                   // TODO: See src/system.cpp
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  float MemoryUtilization();          // TODO: See src/system.cpp
  long UpTime();                      // TODO: See src/system.cpp
  int TotalProcesses();               // TODO: See src/system.cpp
  int RunningProcesses();             // TODO: See src/system.cpp
  std::string Kernel();               // TODO: See src/system.cpp
  std::string OperatingSystem();      // TODO: See src/system.cpp

 private:
  void Update();  // cyclyc update of processes
  std::set<int> ExistentPids() const;
  std::set<int> RelativeComplement(const std::set<int>& one,
                                   const std::set<int>& two);
  std::set<int> ExpiringPids();
  void RemoveTerminatedProcesses(std::set<int> expired_pids);
  Processor cpu_ = {};
  std::vector<Process> processes_;
  std::unordered_map<int, std::string> passwd;
  Password pwd;
};

#endif