
#include <boost/algorithm/string.hpp>

#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>


const std::string kProcDirectory{"/proc/"};
const std::string kStatFilename{"/stat"};

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
    int i{0};
    for (auto s : stat){
        std::cout << i++ << ": " << s << "\n"; 
    }
}