#include <fstream>
#include <iostream>

#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  // Idle = idle + iowait
  // NonIdle = user + nice + system + irq + softirq + steal
  // Total = Idle + NonIdle
  auto cpu1 = LinuxParser::CpuUtilization();
  return std::stof(cpu1[2]);
}