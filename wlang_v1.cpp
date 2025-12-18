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


struct sub_pattern{
  vector<tuple<>> from;
  vector<tuple<>> to;
  bool invoke(
      ast& a
      )
  {
    for (auto e : from) {
      if 
    }
  }
};

struct pattern{
  vector<sub_pattern> patterns;
};

struct ast{
  vector<variant<string,ast>> elements;
  ast& push(string& p) {
    if (p=='(') {
      elements.push_back(ast{});
      return elements.back();
    }
    if (p==')') {

      return;
    }
    elements.push_back()
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
