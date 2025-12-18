#include <bit>
#include <cstdint>
#include <iostream>
#include <limits>
#include <string>
#include <variant>
#include <vector>

using std::bit_width;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::string;
using std::variant;

struct lambda;

struct lambda
{
  vector<string> variables;
  vector<variant<string,lambda>> body;
  bool match(vector<lambda> arguments) {
    auto itv=variables.begin();
    auto ita=arguments.begin();
    for (;
  }
  size_t arity() const {
    size_t a = 0;
    for (auto it=variables.begin();it!=variables.end();++it) {
      if (*it=="...") return ~size_t(0);
      ++a;
    }
    return a;
  }
  bool eval(
      vector<lambda> arguments
      )
  {
    i
  }
};

int main()
{
  for (size_t j=0;;++j) {
    bitstring n = wac_encode(j);
    n.print();
    //cerr << "from 0 to 1=" << " " << n.extract_size_t(0,1) << endl;
    const size_t k = wac_decode_natural_number(n);
    cout << j << " == " << k << endl;
    //for (size_t i=0;i!=n.size();++i) {
    //  cout << n[i];
    //}
    //cout << endl;
  }
}
