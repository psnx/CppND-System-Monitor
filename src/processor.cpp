#include <algorithm>
#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <vector>

#include "processor.h"
#include "linux_parser.h"

float Processor::Utilization() {
  std::unordered_map<std::string, std::function<long(const std::vector<std::string>&)>> input_map;
  input_map["idle"] = &LinuxParser::IdleJiffies;
  long (*fpv)(const std::vector<std::string>&) = &LinuxParser::ActiveJiffies; //because of overload
  input_map["active"] = fpv;
  auto jiffies = GetJiffies(input_map);
  return (float)jiffies["active"] / ((float)jiffies["idle"] + (float)jiffies["active"]) * 100;
}

std::unordered_map<std::string, long> Processor::GetJiffies(std::unordered_map<std::string, std::function<long(const std::vector<std::string>&)>>& jiffy_map){
  std::unordered_map<std::string, long> result_dict;
  const std::vector<std::string>& cpu_core_reads = LinuxParser::CpuUtilization();
  for (auto const& [key, value] : jiffy_map)
  {
    result_dict[key] = jiffy_map[key](cpu_core_reads);
  }
  return result_dict;
}




