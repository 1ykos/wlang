/* identifier name can occupy at most 64 bytes and must not contain spaces
 * elements:
 * 1. function ("define" for named "lambda" for anonymous)
 * 2. symbol_definition
 * 3. symbol_reference
 * 4. variadic_symbol_definition
 *    name... (name can be empty string)
 * 5. variadic_symbol_reference
 *    name... (name can be empty string)
 * 6. tuple
 *    ( tuple ... )
 *
 * comments between # and \n
 *
 * function
 *   element ( parameters... )
 *   element ( body... )
 *   # element ( arguments... )
 *   # greedily grabbing to the right based on parameter pattern
 * 
 *
 * numbers like 3 are a shorthand for:
 * define ( 3 f x ) ( f ( f ( f x ) ) )
 * lambda   ( f x ) ( f ( f ( f x ) ) )
 * 
 */

#include <bit>
#include <cstdint>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

struct element{
  uint64_t type = 0;
  uint64_t data = 0;
};

struct function{
  element parameters;
  element body;
};

uint64_t* eval(uint64_t* p)
{
  uint64_t size = p[0]; // (size-1)/2 elements
  uint64_t num_elements = (size-1)/2;
  element* elements = reinterpret_cast<element*>(p+1);
  if (element[0].is_variable()) return p; // cannot be reduced further
  if (element[0].is_function()) {
    const function_header* header = element[0].to_function();
    element* body = reinterpret_cast<element*>(element[0].to_function()+1);
    for (size_t i=0;i!=(header->size-4)/2;++it) {
      if (body[i].is_variable()) {
        const uint64_t v = body[i].to_variable();
        if (v<num_elements) {
          // replace variable with elements[v]; // variable 0 = self for recursion ? 
        }
      }
    }
  }
}

string read_word

vector<string> match(
    string regex,
    string s,
    vector<string> constants
    )
{
  
}

int main()
{
  size_t i=0;
  size_t j=0;
  string word;
  vector<char> paren_stack;
  for (int c=0;cin;c=cin.get()) {
    if (c=='\n') {
      ++j;
      i=0;
    } else {
      ++i;
    }
    switch (c) {
      case '(':
      case '{':
      case '[':
        cerr << (char(c)) << endl;
        paren_stack.push_back((char(c)));
        break;
      case ')':
      case '}':
      case ']':
        if (paren_stack.empty()) {
          cerr << "closing one too many parentheses" << endl;
          cerr << "in line " << j << " character " << i << endl;
          return -1;
        }
        if (paren_stack.back()!=c) {
          cerr << "openend with " << paren_stack.back() << " but closed with "
               << (char(c)) << endl;
          cerr << "in line " << j << " character " << i << endl;
          return -1;
        }
        paren_stack.pop_back();
        break;
      default:
        if (isspace(c)) {
          cerr << word << endl;
          word.clear();
        } else {
          word.push_back(c);
        }
    }
    if (word=="define") { // named function definition
    }
    if (word=="lambda") { // anonymous function definition
    }
  }
}
