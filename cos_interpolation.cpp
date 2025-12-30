
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

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

double 

b =  0.633303
d =  0.137121
e = -0.542468
f =  0.138785

interpolate = x*(x*(x*(4*b+x*(x*(2*d+4*e+x*(2*f*x-7*f)+15*f)-5*d-10*e-20*f)+6*d+10*e+15*f)-6*b-4*d-5*e-6*f)+2*b+d+e+f-1)+1

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
