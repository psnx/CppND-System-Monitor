#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>

#include <iterator>
#include <map>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::OS(std::string dict_key) {
  string line;
  string key;
  string value;
  std::map<string, string> dict;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()){
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      string s = linestream.str();
      std::vector<std::string> results;
      boost::split(results, s, [](char c){return c == '=';});
      key = results[0];
      value = results[1];
      dict[key]=value;
    }
  }
  if (dict.find(dict_key) != dict.end()){
    return dict.at(dict_key);
  }
  return "";
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, _;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> _ >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { return 0.0; }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  std::string line, token;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream ss (line);
    if (ss >> token) {
      return std::stoi(token);
    }
  }
  return 0;
 }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies(const std::vector<std::string>& cpu) { 
  // {kUser_, kNice_, kSystem_, kIRQ_, kSoftIRQ_, kSteal_, kGuest_, kGuestNice_};
  //auto cpu = LinuxParser::CpuUtilization();
  long activeJiffies{0};
  activeJiffies += std::stol(cpu[LinuxParser::CPUStates::kUser_]);
  activeJiffies += std::stol(cpu[LinuxParser::CPUStates::kNice_]);
  activeJiffies += std::stol(cpu[LinuxParser::CPUStates::kSystem_]);
  activeJiffies += std::stol(cpu[LinuxParser::CPUStates::kIRQ_]);
  activeJiffies += std::stol(cpu[LinuxParser::CPUStates::kSoftIRQ_]);
  activeJiffies += std::stol(cpu[LinuxParser::CPUStates::kSteal_]);
  activeJiffies += std::stol(cpu[LinuxParser::CPUStates::kGuest_]);
  activeJiffies += std::stol(cpu[LinuxParser::CPUStates::kGuestNice_]);
  return activeJiffies; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies(const std::vector<std::string>& cpu) { 
  //auto cpu = LinuxParser::CpuUtilization();  
  long idle{0};
  idle += std::stol(cpu[LinuxParser::CPUStates::kIdle_]);
  idle += std::stol(cpu[LinuxParser::CPUStates::kIOwait_]);
  return idle; 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  return GetCpuStatForCore("cpu");
}

std::vector<std::string> LinuxParser::GetCpuStatForCore(std::string core) {
  vector<string> utilization {};
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::string line;
  const std::string& delims = " ";
  const boost::algorithm::token_compress_mode_type& compress = boost::algorithm::token_compress_on;
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      boost::split(utilization, line, boost::is_any_of(delims), compress);
      if (utilization[0] == core) { 
        utilization.erase(utilization.begin());
        return utilization;
      }
    }
  }
  return utilization;
}

std::unordered_map<std::string, std::string> LinuxParser::ReadProcessStatus(int pid) {
  std::unordered_map<std::string, std::string> stat {};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  std::string line;
  std::vector<std::string> results;
  if (filestream.is_open()){
    while (std::getline(filestream, line)){
      boost::split(results, line, [](char c){return c == ':';});
      auto key = results[0];
      auto value = results[1];
      boost::algorithm::trim(value);
      stat[key]=value;
    }
  }
  return stat;
}





// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return 0; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  std::string token;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  long int time{0};
  if (filestream.is_open()){
    for (int i = 0; filestream >> token; ++i)
      if (i == 22) {
        long int time{stol(token)};
        time /= sysconf(_SC_CLK_TCK);
        return time;
      }
  }
  return time;
}