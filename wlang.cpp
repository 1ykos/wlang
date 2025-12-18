#include <cstdint>
#include <sstream>
#include <string>
#include <unordered_map>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::stringstream;
using std::unordered_map;
using std::vector;

unordered_map<string,uint64_t> defined_symbols;

unordered_map<uint64_t,string> defined_symbols_reverse;

// 0 encompasses everything
// read: i is a categories[i] 
unordered_map<uint64_t,vector<uint64_t>> categories;

vector<string> builtin_symbols = {
  {"any"},
  {"banch"},
  {"define_symbol"},
  {"define_pattern"},
  {"lambda"},
  {"back_reference"},
};

class syntax_node;

class syntax_node{
  public:
  uint64_t symbol = 0;
  uint64_t number = 0; // run-length encoding basically
  syntax_node* more = nullptr;
  ~syntax_node() {
    delete[] more;
  }
  bool match(const syntax_node& a) const {
    const bool number_match = (number==a.number)||(number==0);
    switch (symbol) {
      case 0:
        return number_match;
      case 1:
        if (a.symbol!=1) return false;
        if (!number_match) return false;
        for (size_t i=0;i!=number;++i) {
          if (!more[i].match(a.more[i])) return false;
        }
        return true;
      default:
        if (!number_match) return false;
        vector<uint64_t> todo{a.symbol};
        unordered_map<uint64_t> done;
        while (todo) {
          const uint64_t s = todo.back();
          if (symbol==s) return true;
          todo.pop();
          todo.push_back(categories[s].begin(),categories[s].end());
        }
        return false;
    }
  }
};

uint64_t next_symbol = 0;

vector<tuple<vector<syntax_node>,vector<syntax_node>>> patterns;

/*
define pow ( int int ) ( ... )
           ( int real ) ( ... )
           ( real int ) ( ... )
           ( ) ()
*/

// for each line:
// 1. if it starts with "define", register definition
// 2. lazy lambda calculus
//    

int main() {
  ios_base::sync_with_stdio(false);
  for (;next_symbol!=builtin_symbols.size();++next_symbol) {
    defined_symbols[builtin_symbols[next_symbol]] = next_symbol;
    defined_symbols_reverse[next_symbol] = builtin_symbols[next_symbol];
  }
  for (string line;getline(cin,line);) {
    vector<string> words;
    stringstream ss(line);
    bool ispattern = false;
    for (string word;ss>>word;words.push_back(word)) ispattern |= (word=="=");
    if (words.size()==0) continue;
    size_t i = 0;
    if (words[0]=="define") {
      defined_symbols[words[1]]=next_symbol++;
      ++i;
    }
    vector<uint64_t> ir;
    ir.reserve(words.size());
    for (;i!=words.size();++i) {
      uint64_t symbol = 0;
      if (defined_symbols.count(words[i])) {
        symbol = defined_symbols.at(words[i]);
      }
      ir.push_back(symbol);
    }
    if (ispattern) {
      i
    }
  }
}
