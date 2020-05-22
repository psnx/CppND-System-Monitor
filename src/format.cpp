#include <string>
#include <math.h>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int h = seconds / 3600;
  int m = (seconds / 60) % 60;
  int s = seconds % 60;
  string time {std::to_string(h) + ":" + std::to_string(m) + ":" + std::to_string(s)};
  return time; 
}