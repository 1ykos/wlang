#include <cstdint>
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
using std::string;
using std::stringstream;
using std::unordered_map;
using std::unordered_set;
using std::variant;
using std::vector;



struct ast{
// read as: i is all of definitions[i] and i itself 
// unordered_map<size_t,unordered_set<size_t>> definitions;
  vector<variant<size_t,ast>> elements;
  vector<size_t> recycle;
  unordered_map<string,size_t> defined_symbols;
  unordered_map<size_t,string> defined_symbols_reverse;
  vector<vector<size_t>> backtrack_symbols(1);
  size_t level = 0;
  void push(string& p) {
    if (p=='(') {
      ++level;
      backtrack_definitions.push_back(vector<size_t>());
      cin.get();
      return;
    }
    if (p==')') {
      for (auto i : backtrack_definitions[level]) {
        defined_symbols.erase(defined_symbols_reverse[i]);
        defined_symbols_reverse.erase(i);
      }
      --level;
      cin.get();
      return;
    }
    
  }
};

int main() {
  defined_symbols["lambda"] = 0;
  defined_symbols_reverse[0] = "lambda";
  defined_symbols["define"] = 1;
  defined_symbols_reverse[1] = "define";
  ast a;
  vector<vector<size_t>> backtrack_definitions(1);
  std::ios_base::sync_with_stdio(false);
  while (cin) {
    int p = cin.peek();
    if (p=='(') {
      cin.get();
      continue;
    }
    if (p==')') {
      continue;
    }
    if (p==' ') {
      cin.get();
      continue;
    }
    string word;
    for (p=cin.peek();cin;p=cin.peek()) {
      if (p==')'||p==' '||p=='(') break;
      word+=cin.get();
    }
    if (!cin) break;
    if (0==defined_symbols.count(word)) {
      cerr << word << " = " << next_symbol << endl;
      defined_symbols_reverse[next_symbol] = word;
      defined_symbols[word] = next_symbol;
      backtrack_definitions[level].push_back(next_symbol);
      ++next_symbol;
    }
  }
  return 0;
}
