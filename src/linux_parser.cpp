#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <boost/algorithm/string.hpp>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <vector>

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
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      string s = linestream.str();
      std::vector<std::string> results;
      boost::split(results, s, [](char c) { return c == '='; });
      key = results[0];
      value = results[1];
      dict[key] = value;
    }
  }
  if (dict.find(dict_key) != dict.end()) {
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
std::set<int> LinuxParser::Pids() {
  std::set<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.insert(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float mem_total{1};
  float mem_free{0};
  float buffers{0};
  std::ifstream stream (kProcDirectory+kMeminfoFilename);
  std::string key, value, unit;
  if (stream.is_open()){
    for (int i = 0; i<4 && (stream >> key >> value >> unit); i++){
      if (key == "MemTotal:") { mem_total = stof(value);}
      if (key == "MemFree:") { mem_free = stof(value);}
      if (key == "Buffers:") { buffers = stof(value);}
    }
  }
  return 1 - mem_free / (mem_total-buffers);
}

long LinuxParser::UpTime() {
  std::string line, token;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream ss(line);
    if (ss >> token) {
      return std::stoi(token);
    }
  }
  return 0;
}

long LinuxParser::Jiffies() { return UpTime() * sysconf(_SC_CLK_TCK); }

std::unordered_map<std::string, std::string> LinuxParser::ReadProcessStatus(
    int pid) {
  std::unordered_map<std::string, std::string> stat{};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  std::string line;
  std::vector<std::string> results;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      boost::split(results, line, [](char c) { return c == ':'; });
      auto key = results[0];
      auto value = results[1];
      boost::algorithm::trim(value);
      stat[key] = value;
    }
  }
  return stat;
}

std::vector<std::string> LinuxParser::ReadProcessStat(int pid) {
  std::vector<std::string> stat{};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  std::string line, token;
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream ss(line);
    while (ss >> token) {
      stat.push_back(token);
    }
  }
  return stat;
}

std::unordered_map<std::string, std::string> LinuxParser::ReadStat() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::unordered_map<std::string, std::string> umap;
  std::string line;
  while(std::getline(stream, line)){
    std::istringstream iss(line);
    std::string key, value, token;
    if(iss >> key){
      while (iss >> token) { value += token + " "; }
      umap[key] = value;
    }
  }
  return umap;
}

string LinuxParser::Command(int pid) {
  std::string line, token;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream ss(line);
    if (ss >> token) {
      return token;
    }
  }
  return "NaN";
}
