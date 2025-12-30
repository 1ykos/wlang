#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include "/home/usr/src/wmath/wmath"

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::setw;
using std::string;
using std::stringstream;
using std::unordered_map;
using std::unordered_set;
using std::variant;
using std::vector;

using wmath::universal_encode_uint64;
using wmath::length_of_universal_code;

string to_string(const vector<bool> data) {
  string result(data.size(),0);
  for (size_t i=0;i!=data.size();++i) {
    result[i]=data[i]?'1':'0';
  }
  return result;
}

int main() {
  for (size_t i=0;i<(1ull<<18)-1;++i) {
    uint64_t lev = universal_encode_uint64(i+1);
    const size_t l = length_of_universal_code(i+1);
    std::string s;
    for (size_t i=0;i!=l;++i) {
      s+= std::to_string((lev>>(63-i)) & 1u);
    }
    cout << setw(6) << i << " " << s << endl;
  }
}
