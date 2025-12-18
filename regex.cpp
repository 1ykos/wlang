#include <bit>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::tuple;
using std::unordered_map;
using std::vector;

struct element {
  uint64_t data = 0;
  /*
  enum uint64_t type {
    variable = 0, // followed by id
    variadic = 1, // followed by id
    tuple = 2     // followed number of elements, then the elements
  };
  */
  uint64_t type () const {
    return (data>>62);
  };
  uint64_t value() const {
    return (data&((~uint64_t(0))>>2));
  }
};

int main() {
  // [a-z] matches one of anything, (.*) or any letter
  // [a-z]* matches as much as possible, zero or more
  // ( matches (
  // ) matches )
  string capture;
  getline(cin,capture);
  vector<element> c;
  vector<bool> variable_observed(64);
  vector<tuple<size_t,size_t>> stack;
  for (size_t i=0;i!=capture.size();++i) {
    if ((capture[i]>='a')&&(capture[i]<='z')) {
      if (variable[capture[i]-'a') {
        cerr << "variables need to be unique,"
                "backreferences are alluring,"
                "but lead to unmanageable matching complexit" << endl;
        cerr << capture[i] << " used for the second time" << endl;
        return -1;
      }
      if (!stack.empty()) ++get<1>(stack.back());
      if (i+1!=capture.size()) {
        if (capture[i+1]=='*') { // variadic
          c.emplace_back((uint64_t(1)<<62)|capture[i]);
          ++i;
          continue;
        }
      }
      c.emplace_back(uint64_t(capture[i]));
      continue;
    }
    if (capture[i]==')') {
      if (stack.empty()) {
        cerr << "stray ) encountered at " << i << endl; 
        return -1;
      } else {
        c[get<0>(stack.back())].data|=get<1>(stack.back());
        stack.pop_back();
        continue;
      }
    }
    if (capture[i]=='(') {
      if (!stack.empty()) ++get<1>(stack.back());
      stack.emplace_back(c.size(),0);
      c.emplace_back(uint64_t(2)<<62);
    }
    // ignore anything else
  }
  for (auto it=c.begin();it!=c.end();++it) {
    cerr << std::hex << it->data << endl;
  }
  string replace;
  getline(cin,replace);
  string input;
  getline(cin,input);
  unordered_map<uint64_t,string> matches;
  for (size_t i=0;i!=c.size();++i) {
    switch (c[i].type) {
      case 0: // variable
        if (matches.count(c[i].value())) {
          // variable is known
        }
        break;
    }
  }
}
