#include <algorithm>
#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <vector>

#include "processor.h"
#include "linux_parser.h"


// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  std::unordered_map<std::string, std::function<long(void)>> input_map;
  input_map["idle"]=(&LinuxParser::IdleJiffies);
  //pointer to ActiveJiffies()
  long (*fpv)() = &LinuxParser::ActiveJiffies;
  input_map["active"] = fpv;
  auto jiffies = GetJiffies(input_map);
  return (float)jiffies["active"] / ((float)jiffies["idle"]+(float)jiffies["active"]);
}

std::unordered_map<std::string, long> Processor::GetJiffies(std::unordered_map<std::string, std::function<long(void)>> jiffy_map){
  std::unordered_map<std::string, long> result_dict;
  for (auto const& [key, value] : jiffy_map)
  {
    result_dict[key] = jiffy_map[key]();
  }
  return result_dict;
}




