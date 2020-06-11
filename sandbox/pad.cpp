#include <math.h>
#include <string>
#include <iostream>

using std::string;

string Pad(string s, char c, int padsize){
  if (padsize - static_cast<int>(s.size()) < 0) 
    return s;
  s.insert(s.begin(), padsize - s.size(), c);
  return s;
}

int main(){
    std::cout << Pad("13", '0', 5) << "\n";

}