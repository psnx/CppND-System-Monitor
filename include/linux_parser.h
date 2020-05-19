#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <array>
#include <fstream>
#include <regex>
#include <string>
#include <unordered_set>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();
std::string OS(std::string dict_key);


// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
/*
const std::array<CPUStates, 8> nonidles =
{kUser_, kNice_, kSystem_, kIRQ_, kSoftIRQ_, kSteal_, kGuest_, kGuestNice_};

const std::array<CPUStates, 2> idles =
{ kIdle_, kIOwait_ };
*/
std::vector<std::string> CpuUtilization();
std::vector<std::string> GetCpuStatForCore(std::string core);

long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
};  // namespace LinuxParser

#endif