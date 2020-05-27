#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <unordered_map>
#include <vector>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  void Update();
  int Pid() const;                         // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  bool operator>(Process const& a) const;

  // TODO: Declare any necessary private members
 private:
  int pid_;
  std::unordered_map<std::string, std::string> process_status{};
  std::vector<std::string> stat;  // from /proc/PID/stat
  long ActiveJiffies();           // The the process's own jiffies
  enum eProcess_stat {
    user = 13,
    kernel = 14,
    children_user = 15,
    children_kernel = 16
  };
};

#endif