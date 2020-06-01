#include <math.h>
#include <string>

#include "format.h"
using std::string;

string Format::Pad(string s, char c, int padsize){
  s.insert(s.begin(), padsize - s.size(), c);
  return s;
}

string Format::ElapsedTime(long seconds) {
  int h = seconds / 3600;
  int m = (seconds / 60) % 60;
  int s = seconds % 60;
  return std::string(Format::Pad(std::to_string(h), '0', 2) + ":" +
                     Format::Pad(std::to_string(m), '0', 2) + ":" +
                     Format::Pad(std::to_string(s), '0', 2));
}