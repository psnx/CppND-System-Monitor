#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  // Idle = idle + iowait
  // NonIdle = user + nice + system + irq + softirq + steal
  // Total = Idle + NonIdle

  auto cpu1 = LinuxParser::CpuUtilization();
  float user = std::stof(cpu1[1]);
  float nice = std::stof(cpu1[2]);
  float system = std::stof(cpu1[3]);
  float idle = std::stof(cpu1[4]);
  float iowait = std::stof(cpu1[5]);
  float irq = std::stof(cpu1[6]);
  float softirq = std::stof(cpu1[6]);
  float steal = std::stof(cpu1[7]);

  float nonidle = user + nice + system + irq + softirq + steal;
  idle += iowait;
  return nonidle + idle > 0 ? nonidle / (nonidle + idle) * 100 : 0;
}