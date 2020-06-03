#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <unordered_map>
#include <vector>

#include "password.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid, Password* pwd_pointer);
  void Update();
  int Pid() const;                         // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process & a);  // TODO: See src/process.cpp
  bool operator>(Process & a);

  // TODO: Declare any necessary private members
 private:
  int pid_;
  long ActiveJiffies();           // The the process's own jiffies
  long preciding_active_{0};
  long preciding_total_{0};
  
  std::unordered_map<std::string, std::string> process_status{};
  std::vector<std::string> stat_;  // from /proc/PID/stat
  enum eProcess_stat {
    user = 13,
    kernel = 14,
    children_user = 15,
    children_kernel = 16,
    up_time =21
  };
  Password* pwd_;
};

#endif