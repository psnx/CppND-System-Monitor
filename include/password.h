#ifndef PASSWORD_H
#define PASSWORD_H

#include <string>
#include <unordered_map>

class Password {
  public:
    Password();
    std::string FindUsername(int uid);

  private:
    std::unordered_map<int, std::string> passwd_cache_;
};

#endif