#include <algorithm>
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
using std::min;
using std::string;
using std::stringstream;
using std::unordered_map;
using std::unordered_set;
using std::variant;
using std::vector;

vector<size_t> tokenize(const string& s)
{
  vector<size_t> token_lengths(s.size());
  vector<size_t> match_stack;
  size_t state = 0;
  // 0 : space
  // 1 : bracket
  // 2 : word
  // 3 : =>
  for (size_t i=0;i<=s.size();++i) {
    size_t nextstate = 2;
    if (s[i]==' '||s[i]=='\n') nextstate = 0;
    if (s[i]=='('||s[i]==')' ) nextstate = 1;
    if (s[i]=='='||s[i]=='>' ) nextstate = 3;
    if (state==nextstate) continue;
    if (state>1) { // word or => end
      if (match_stack.size()==0) return token_lengths;
      token_lengths[match_stack.back()]=i-match_stack.back();
      cerr << "pop(" << match_stack.back() << ") \"" <<s[i] << "\"" << endl;
      match_stack.pop_back(); 
    }
    if (nextstate>1) {
      cerr << i << endl;
      match_stack.push_back(i);
    }
    if (s[i]=='(') {
      state=0;
      cerr << i << endl;
      match_stack.push_back(i);
      continue;
    }
    if (s[i]==')') {
      state=0;
      if (match_stack.size()==0) return token_lengths;
      token_lengths[match_stack.back()]=i-match_stack.back();
      cerr << "pop(" << match_stack.back() << ") \"" <<s[i] << "\"" << endl;
      match_stack.pop_back();
      continue;
    }
    state = nextstate;
  }
  if (state) token_lengths[match_stack.back()]=s.size()-match_stack.back();
  return token_lengths;
/*
    if (s[i]==' '||s[i]=='\n') nextstate = 0;
  }
    */
  return token_lengths;
}

struct closure;

struct closure{
  vector<string> variables;
  vector<string> body_variables;
  vector<closure> body_lambdas;
  vector<lambda> args;
  vector<lambda> eval() {
    unordered_map<string,lambda> substitution;
    for (size_t i=0;i!=min(args.size(),variables.size());++i) {
      substitution.emplace(variables[i],args[i]);
    }
    size_t j = 0;
    for (auto it=body_variables.begin();it!=body_variables.end();++it) {
      if (*it=="()") { // i.e. not a variable
        
      }
    }
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  string sentence;
  for (int c=cin.get();cin;c=cin.get()) sentence.push_back(c);
  cerr << "read input" << endl;
  auto t = tokenize(sentence);
  for (size_t i=0;i!=sentence.size();++i) {
    cout << " " << sentence[i];
  }
  cout << endl;
  for (auto it=t.begin();it!=t.end();++it) {
    cout << std::setw(2) << *it;
  }
  cout << endl;
}
