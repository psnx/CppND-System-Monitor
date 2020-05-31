#include "processor.h"

#include <algorithm>
#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <vector>
#include <string>

#include "linux_parser.h"

float Processor::Utilization() {
  stat_ = LinuxParser::ReadStat();
  std::vector<std::string> stat_cpu = stat_line_to_vector("cpu");
  return (float)ActiveJiffies(stat_cpu) / ((float)ActiveJiffies(stat_cpu) + (float)IdleJiffies(stat_cpu));
}

std::vector<std::string> Processor::stat_line_to_vector(std::string key){
  std::istringstream ss(stat_[key]);
  std::string token;
  std::vector<std::string> cpu;
  while (ss >> token ){cpu.push_back(token);}
  return cpu;
}

long Processor::ActiveJiffies(std::vector<std::string>& cpu) {
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

long Processor::IdleJiffies(std::vector<std::string>& cpu) {
  long idle{0};
  idle += std::stol(cpu[LinuxParser::CPUStates::kIdle_]);
  idle += std::stol(cpu[LinuxParser::CPUStates::kIOwait_]);
  return idle;
}
