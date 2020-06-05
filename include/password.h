#ifndef PASSWORD_H
#define PASSWORD_H

#include <string>
#include <unordered_map>

class Password {
  public:
    Password();
    std::string GetUserName(int uid);
    std::string GetUserNameShorten(int uid, int max_length);
    void Update();

  private:
    std::unordered_map<int, std::string> passwd_cache_;
};

#endif