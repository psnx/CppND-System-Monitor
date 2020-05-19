#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <functional>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  std::unordered_map<std::string, long> GetJiffies(std::unordered_map<std::string, std::function<long(const std::vector<std::string>&)>>& jiffy_map);
};

#endif