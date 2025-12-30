// Maximum length for any identifier, variable or function name
constexpr size_t max_identifier_length = 32;

// allowed symbols in any identifier, variable or function names (69)
const char identifier_characters[] =
"0123456789"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";

// scope separator
const char scope_separator = ':';

// special placeholder (only in function body)
// "_"

// brackets {} () [] are different but equal (must match)

// 0 = space
// 1 = identifier character
// 2 = scope separator
// 3 = junk
constexpr size_t classify_char(
    char c
    )
{
  if ((c=='\t')||(c==' ')) { // we are some kind of space
    return 0;
  }
  if (((c>='0')&&(c<='9'))
    ||((c>='A')&&(c<='Z'))
    ||((c>='a')&&(c<='z'))) return 1;
  if (c==':') {             // scope separator
    return 2;
  }
  if (c=='{') return '{';
  if (c=='}') return '}';
  if (c=='(') return '(';
  if (c==')') return ')';
  if (c=='[') return '[';
  if (c==']') return ']';
  return 3;
}

unorderd_map<uint64_t,char[max_identifier_length]> code_name;
unorderd_map<char[max_identifier_length],uint64_t> name_code;

bool permutation_is_rotation(
    const size_t n,
    const int64_t *(f[2])
    )
{
  int64_t d = f[0][0]-f[0][1];
  for (size_t i=1;i!=n;++i) {
    if (f[i][0]-f[i][1]!=d) return false;
  }
  return true;
}

struct range{
  // 1,2,3,...,-3,-2,-1
  int64_t lo = 0;
  int64_t hi = 0;
};

vector<range> convert_to_ranges(
    const vector<int64_t>& a
)
{
  vector<range> result;
  if (a.size()==0) return result;
  result.push_back({a.front(),a.front()});
  size_t j = 0;
  for (size_t i=1;i!=a.size();++i) {
    if (a[i]==a[i-1]) ranges.back().hi=a[i];
    else ranges.emplace_back(a[i],a[i]);
  }
  return result;
}


struct symbol_generator{
  size_t n = 0;
  vector<size_t> recycled_symbols;
  operator()(){
    if (recycled_symbols.size()==0) {
      return n++;
    } else {
      const size_t m = recycled_symbols.back();
      recycled_symbols.pop_back();
      return m;
    }
  }
  void recycle(size_t i) {
    recycled_symbols.push_back(i);
  }
};

constexpr bool is_variable(const uint64_t symbol)
{
  return (symbol&(1ull<<63))!=0;
}

constexpr bool is_function(const uint64_t symbol)
{
  return (symbol&(1ull<<63))==0;
}

constexpr tuple<uint64_t,uint64_t> analyze(
    const uint64_t n
    const uint64_t *function_body
)
{
  unordered_set<uint64_t> vars;
  unordered_set<uint64_t> funs;
  for (size_t i=0;i!=n;++i) {
    const uint64_t symbol = ((1ull<<63)-1)&function_body[i];
    if (is_variable(function_body[i])) vars.insert(symbol);
    else                               funs.insert(symbol);
  }
  return {vars.size(),funs.size()};
}

constexpr bool is_permutation(
    nargs 
    )
{
  i
}

struct function_data{
  unordered_map<uint64_t,function*> definitions;
  uint64_t datasize = 0;
  uint64_t body[1];
  //uint64_t nargs;
  //uint64_t resolve_args[nargs];
  //rest of body
};

struct function {
  int64_t iteration = 0;
  function_data *data;
  void operator()(uint64_t *args) {
    for (auto it=args.begin();it!=args.end();++it) {
      size_t i = resolve_args[get<0>(*it)];
      const uint64_t symbol = body[i+1];
    }
    for (size_t i=1;i!=2*body_length+1;i+=2) {
      // body[i] is the iteration count
      const size_t body_iteration_count = body[i];
      const uint64_t symbol = body[i+1];
      const bool is_variable = (symbol&(1ull<<63))>0;
      if (is_variable) {
        const size_t variable = symbol^(1ull<<63);
        if (variable<args.size()) {
          *args[
        }
      }
    }
  }
}

uint64_t* trampoline(
    uint64_t* f,

    )
{
}

struct ast{

};
