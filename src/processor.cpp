#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  auto cpu1 = LinuxParser::CpuUtilization();
  return std::stof(cpu1[1]);
}