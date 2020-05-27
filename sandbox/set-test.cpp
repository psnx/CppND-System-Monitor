#include <iostream>
#include <set>
#include <algorithm>
#include <vector>

std::set<int> Diff(const std::set<int>& one, const std::set<int>& two){
  std::set<int> diff{};
  std::set_difference(one.begin(), one.end(),
    two.begin(), two.end(),
    std::inserter(diff, diff.begin()));
  return diff;
}
void Remtest(int nr, std::vector<int>& v1) {
    auto it = std::remove_if(v1.begin(), v1.end(),
        [&](auto & elem){return elem == nr;});
    v1.erase(it, v1.end());
}

int main(){
    std::vector<int> v1{1,2,3,4,5,6,7,8};

    std::set<int> set1 {1,2,3,4,5,6,7,8};
    std::set<int> set2 {1,4,8};

    auto diff = Diff(set1, set2);
    std::cout << "DIFF------ \n";
    for (auto e : diff) {
        std::cout << e;
    }
    std::cout <<"----------\n";
    for(int i : diff) {
        Remtest(i, v1);;
    }

    for (auto elem: v1){
        std::cout <<elem <<"\n";
    }
return 0;
}

