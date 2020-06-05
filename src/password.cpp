#include <iostream>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <vector>
#include <unordered_map>

#include "password.h"

const std::string kPasswordPath{"/etc/passwd"};

Password::Password(){
  Update();
}

void Password::Update(){
  std::ifstream stream(kPasswordPath);
  std::vector<std::string> results;
  std::string line;
  if (stream.is_open()){
    while (std::getline(stream, line)){
      boost::split(results, line, [](char c){return c==':';});
      int uid = stoi(results[2]);
      std::string uname = results[0];
      passwd_cache_[uid] = uname;
    }
  }
}

std::string Password::FindUsername(int uid){
  std::unordered_map<int,std::string>::const_iterator it = passwd_cache_.find(uid);
  if (it == passwd_cache_.end()){
    passwd_cache_.insert(std::make_pair(uid, "none"));
    return "none";
  } else {
    return passwd_cache_.at(uid);
  }
}


