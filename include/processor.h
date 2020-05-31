#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <functional>

class Processor {
  public:
    float Utilization();  // TODO: See src/processor.cpp

  private:
    long ActiveJiffies(std::vector<std::string>& cpu);
    long IdleJiffies(std::vector<std::string>& cpu);
    std::vector<std::string> stat_line_to_vector(std::string key);

    std::unordered_map<std::string, std::string> stat_ = {};
};

#endif