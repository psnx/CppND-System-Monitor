#include <string>
#include <fstream>
#include <unordered_map>
#include <iostream>
#include <sstream>

const std::string kProcDirectory{"/proc/"};
const std::string kStatFilename{"/stat"};

std::unordered_map<std::string, std::string> ReadStat() {
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

int main(){
  auto stat = ReadStat();
  for (const auto& [k,v] : stat){
    std::cout << "key: \t" << k <<"\t : \t"<<v<<"\n";
  }
}