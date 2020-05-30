#include <iostream>
#include <set>
#include <algorithm>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <boost/algorithm/string.hpp>
#include <sstream>
#include <iterator>
#include <string>

const std::string kPasswordPath{"/etc/passwd"};

std::string User(int uid) {
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    std::string line;
    std::string uid_str = std::to_string(uid);
    std::string token {"x:" + uid_str};
    std::cout <<"token: \t" << token << "\n";
    while (std::getline(stream, line)) {
      auto marker = line.find(token);
      if (marker != std::string::npos) {
        return line.substr(0, marker - 1);
      }
    }
  }
  return "Nope";
}

std::unordered_map<int, std::string> ReadPasswd(){
    std::ifstream stream(kPasswordPath);
    std::unordered_map<int, std::string> passwd_dict;
    std::vector<std::string> results;
    std::string line;
    if (stream.is_open()){
        while (std::getline(stream, line)){
            boost::split(results, line, [](char c){return c==':';});
            int uid = stoi(results[2]);
            std::string uname = results[0];
            passwd_dict[uid] = uname;
        }
    }
    return passwd_dict;
}

int main(){
    std::string u = User(1);
    std::cout << u << "\n";
    std::cout << "-------------------------- \n";
    auto pwd{ReadPasswd()};
    for ([[maybe_unused]] auto const& [k,v] : pwd){
        std::cout << k << "\t" << v << "\n";
    }
}