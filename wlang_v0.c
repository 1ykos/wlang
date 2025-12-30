#include <stdint.h>
#include <stdlib.h>


// apply n_args
// function*
// 
// 
// 
// 
// 



// 0 function
// 1 variable
// 2 apply
uint8_t inline decode(uint64_t& v)
{
  // is variable -> 63 bit int
  if (v&(1ull<<63)) {
    if (v&(1ull<<62)==0) v^=(1ull<<63);
    return 1;
  }
  // is apply -> 62 bit uint
  if (v&(1ull<<62)) {
    v^=(1ull<<62);
    return 2;
  }
  // else is pointer -> 62 bit uint
  return 0;
}

struct parameterpack_header{
  uint64_t size; // size of parameter pack in 64 bit chunks
};

struct function_header{
  uint64_t size;            // number of 64 bit chunks
   int64_t iteration_count; // built-in recursion
};
// header followed by body
// then bound parameterpack beginning with size

// f - function
// args - parameter pack
// assumption: bound variables follwing f is 0
uint64_t* apply_function(
          uint64_t *f,
    const uint64_t *args
    )
{
  uint64_t funsize = f[0];
  int64_t iteration_count = ((int64_t*)f)[1];
  uint64_t numvars = 0; // count the variables
  uint64_t argsize = args[0];
  uint64_t numargs = argsize-1;
  uint64_t newsize = 2;
  for (size_t i=2;i!=funsize;++i) {
    uint64_t v = f[i];
    switch (decode(v)) {
      case 0:
        // Move along, nothing to do here
        ++newsize;
        break;
      case 1:
        // variable substitution
        int64_t i = *((int64_t*)(&v));
        if (i<0) i = numargs-i;
        if ((i<0)||(i>=numargs)) {
          ++newsize;
          break; // can't substitute, not enough args
        }
        break;
      case 2:
        break;
    }
  }
  return g;
}

int main()
{
  uint64_t *function = malloc();
  struct function_header *h = (function_header)function;
  uint64_t body = 
}
