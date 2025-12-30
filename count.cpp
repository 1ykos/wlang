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

/*
class fibonacci_number {
  private:
    vector<bool> data;
  public:
    fibonacci_number(size_t n)
    {
      size_t a,b;
      for (a=1,b=2;b<n;(b=a+b,a=b-a)) data.push_back(0);
      for (size_t i=0;n;(++i;a=b-a,b=b-a)) {
        if (n>a) {
          data[i]=1;
          n-=a;
        }
      }
    }
};
*/
vector<bool> fibonacci_encode(size_t n)
{
  vector<bool> fib;
  fib.push_back(1);
  size_t a,b;
  for (a=1,b=1;b<=n;(b=a+b,a=b-a)) fib.push_back(0);
  for (size_t i=0;i<fib.size();(++i,a=b-a,b=b-a)) {
    if (n>=a) {
      fib[i+1]=1;
      n-=a;
    }
  }
  fib = vector<bool>(fib.rbegin(),fib.rend());
  return fib;
}

vector<bool> bct_encode(size_t n){
  size_t j=0;
  for (size_t i=3;n>=i;i*=3) {
    n-=i;
    ++j;
  }
  vector<bool> bct;
  bct.push_back(1);
  bct.push_back(1);
  for (size_t i=0;i!=j+1;++i) {
    switch (n%3) {
      case 0:
        bct.push_back(0);
        bct.push_back(0);
        break;
      case 1:
        bct.push_back(1);
        bct.push_back(0);
        break;
      case 2:
        bct.push_back(0);
        bct.push_back(1);
        break;
    }
    n/=3;
  }
  bct = vector<bool>(bct.rbegin(),bct.rend());
  return bct;
}

vector<bool> wac_encode(size_t m) {
  size_t l = 1+wmath::log2(m);
  size_t n = l;
  vector<bool> wac;
  size_t a,b;
  for (a=1,b=1;b<=n;(b=a+b,a=b-a)) wac.push_back(0);
  for (size_t i=0;i<wac.size();(++i,a=b-a,b=b-a)) {
    if (n>=a) {
      wac[wac.size()-i-1]=1;
      n-=a;
    }
  }
  for (size_t i=0;i!=l;++i) {
    wac.push_back((m>>(l-i-1))&1u);
  }
  return wac;
}

string to_string(const vector<bool> data) {
  string result(data.size(),0);
  for (size_t i=0;i!=data.size();++i) {
    result[i]=data[i]?'1':'0';
  }
  return result;
}

int main() {
  for (size_t i=0;i<(1ull<<10)-1;++i) {
    const auto fib = to_string(fibonacci_encode(i+1));
    const auto bct = to_string(bct_encode(i));
    uint64_t lev = universal_encode_uint64(i);
    const size_t l = length_of_universal_code(i);
    std::string s;
    for (size_t i=0;i!=l;++i) {
      s+= std::to_string((lev>>(63-i)) & 1u);
    }
    const auto wac = to_string(wac_encode(i+1));
    cout << setw(5)  << i << " "
//         << setw(8) << bct.size() << " "
         << setw(8) << fib << " "
//         << setw(8) << wac.size() << " "
         << setw(8) << s << " "
         << lev << endl;
    //if (i<16) ++i;
    //i=i*17/16;
  }
}
