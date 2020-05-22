#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <dirent.h>

#include <boost/algorithm/string.hpp>

#include <iterator>
#include <map>

using std::vector;
using std::to_string;
using std::string;

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

std::unordered_map<std::string, std::string> ReadProcessStat(int pid) {
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
vector<int> Pids() {
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

int main (){
    vector<int> pids = Pids();
    for (auto p: pids){
      std::cout << p << "\n";
    }
    std::cout << "----------------\n";
    for (auto p : pids){
      auto process_map = ReadProcessStat(p);
      std::cout << to_string(p["Uid"]) << "\n";
    }

    //std::cout << process_map.size() <<"\n";
    //std::cout << "UID \t" << process_map["Uid"];
    /*
    string a, b, c;
    std::istringstream ss (process_map["Uid"]);
    ss >> a >> b >> c;
    std::cout << a << ":" << b << ":" << c << ":" << "\n";
    string msg = process_map["Name"];
    boost::algorithm::trim(msg);
    std::cout << msg << "\n";
    */
}
