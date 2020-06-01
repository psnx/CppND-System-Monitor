
#include <boost/algorithm/string.hpp>

#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>


const std::string kProcDirectory{"/proc/"};
const std::string kStatFilename{"/stat"};

std::string Pad(std::string s, char c){
  s.insert(s.begin(), 2 - s.size(), c);
  return s;
}

std::string ElapsedTime(long seconds) {
  int h = seconds / 3600;
  int m = (seconds / 60) % 60;
  int s = seconds % 60;
  return std::string(Pad(std::to_string(h), '0') + ":" +
                     Pad(std::to_string(m), '0') + ":" +
                     Pad(std::to_string(s), '0'));
}

std::vector<std::string> ReadProcessStat(int pid) {
  std::vector<std::string> stat{};
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
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

int main (){
    std::vector<std::string> stat = ReadProcessStat(1509);
    long int elapsed = stol(stat[22])/sysconf(_SC_CLK_TCK);
    std::string formatted = ElapsedTime(elapsed);
    std::cout << formatted <<"\n";
}