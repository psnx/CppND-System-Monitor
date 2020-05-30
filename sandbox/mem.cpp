#include <string>
#include <fstream>
#include <iostream>

const std::string kProcDirectory{"/proc/"};
const std::string kMeminfoFilename{"/meminfo"};


float MemoryUtilization() { 
  float mem_total{1};
  float mem_free{0};
  float buffers{0};
  std::ifstream stream (kProcDirectory+kMeminfoFilename);
  std::string key, value, unit;
  if (stream.is_open()){
    for (int i = 0; i<4 && (stream >> key >> value >> unit); i++){
      if (key == "MemTotal:") { mem_total = stof(value); std::cout << "Total: \t" << mem_total << "\n";}
        
      if (key == "MemFree:") { mem_free = stof(value); std::cout << "free: \t" << mem_free << "\n";}
       
      if (key == "Buffers:") { buffers = stof(value); std::cout << "buffers: \t" << buffers << "\n";}
       

    }
  }
  return 1 - mem_free / (mem_total-buffers);
}

int main(){
    float util = MemoryUtilization();
    std::cout << util <<"\n";

}