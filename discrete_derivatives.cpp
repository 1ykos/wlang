#include <array>
#include <cmath>
#include <cstdint>
#include <functional>
#include <numeric>
#include <ranges>
#include <set>
#include <tuple>
#include <vector>
#include <iostream>

/*
{
  {
    dec_domain,
    dec_value
  },
  {
    inc_domain,
    inc_value,
  }
}*/

// 0 -> dynamically sized
template<typename T,size_t ...n>
class tensor;

template<typename T,size_t ...n>
class tensor{
  private:
  constexpr static size_t num_zero() {
    if ((sizeof...(n))==0) return 0;
    const std::array<size_t,sizeof...(n)> v{n...};
    auto arezero = v | std::views::filter([](const auto& i){return i==0;});
    return std::reduce(arezero.begin(),arezero.end());
  }
  size_t dynamic_sizes[num_zero()]{};
  T* data;
  public:
  size_t size() const {
    if constexpr (sizeof...(n)==0) return 0;
    const std::array<size_t,sizeof...(n)> v{n...};
    size_t s = 1;
    for (size_t i=0,j=0;i!=v.size();++i) {
      if (v[i]) s*=v[i];
      else      s*=dynamic_sizes[j++];
    }
    return s;
  }
  auto sizes() const {
    std::array<size_t,sizeof...(n)> v{n...};
    for (size_t i=0,j=0;i!=v.size();++i) if (v[i]==0) v[i]=dynamic_sizes[j++];
    return v;
  }
  constexpr static std::array<size_t,sizeof...(n)> const_sizes() {
    return std::array<size_t,sizeof...(n)>{n...};
  }
  tensor() {
    data = new T[size()]();
  }
  tensor(T* data):data(data) {}
  ~tensor() {
    delete[] data;
  }
  auto& operator[](const size_t i) {
    const std::array<size_t,sizeof...(n)> v{n...};
    if constexpr (sizeof...(n)==0) {
      throw std::out_of_range ("dimentionless tensor cannot be accessed");
      return 0;
    }
    if constexpr (sizeof...(n)==1) {
      return data[i];
    } else {
      size_t s = 1;
      for (size_t i=1,j=(v[0]==0);i!=v.size();++i) {
        if (v[i]) s*=v[i];
        else      s*=dynamic_sizes[j++];
      }
      return tensor(data+i*s);
    }
  }
  auto& operator[](const int i) {
    const std::array<size_t,sizeof...(n)> v{n...};
    const size_t s = v[0]?v[0]:dynamic_sizes[0];
    const int j = (i%s);
    const size_t k = j<0?s+j:j;
    return operator[](k);
  }
  /* TODO contraction
  template<T0,T1,size_t...d>
  friend auto contraction(const T0& lhs,T1& rhs) {
    const auto v0 = lhs.sizes();
    const auto v1 = rhs.sizes();
    static_assert(lhs.back()==rhs.front(),"tensor sizes don't match");
    std::array<size_t,
  }
  */
  auto& operator*=(const T& v) {
    for (size_t i=0;i!=size();++i) data[i]*=v;
    return *this;
  }
};

/*
template<typename T>
struct variable;

template<>
struct variable<uint8_t>{
  uint8_t v = 0;
  uint8_t next_inc() {
    return ;
  }
  uint8_t next_dec() {
    return ;
  }
  const auto derivative() {
    array<array<uint8_t,2>,2> d;
    d[0][0] = v>  0?v-1:v;
    d[0][1] = v>  0;
    d[1][0] = v<255?v+1:v;
    d[1][1] = v<255;
    return d;
  }
};
*/

int8_t constexpr soft_abs(int8_t x) {
  x = abs(x);
  return x+(x<1)+(x<3);
}

int8_t constexpr relu(int8_t x) {
  return x<0?0:x;
  constexpr int8_t d[2][2] {
    {
      x<1?x:x-1;
      x<1?0:x-1;
    },
    {
      (x<1)?(1):((x<127)?(x+1):(x)),
      (x<1)?(1):((x<127)?(x+1):(x))
    }
  };
}

// returns the closest value to x for which absdiff(abs(z),y) is minimal
int8_t constexpr inv_abs(int8_t x,int8_t y) {
  return relu(y);
}

// returns the closest value to x for which absdiff(relu(z),y) is minimal
int8_t constexpr inv_relu(int8_t x,int8_t y) {
  if (y<1) {
    if (x<1) return x;
    return 0;
  } else {
    return y;
  }
}

// diff ( add  ( div absdiff(x,y) 2 ) relu(z) )
// add diff ( div absdiff(x,y) 2 ) relu(z)
// add ( div ( div absdiff(x,y) ) 2 )
//
// sum mul a x mul b y mul c z
//
// x = 1,2,3
// y = 2 3,4
// z = 4,5,6
//
// f = 10 20 30
//
// 

int8_t constexpr soft_relu(int8_t x) {
  return (x<0?0:x)+(x==0);
  constexpr int8_t d[2][2] {
    {
      x<0?x:(x-1-(x==1)),
      x<0?0:x+(x==0)-1
    },
    {
      (x<0)?(0):((x<127)?(x+1+(x==0)):(x)),
      (x<0)?(1):((x<127)?(x+1+(x==0)):(x))
    }
  };
}

uint8_t constexpr soft_absdiff(uint8_t x,uint8_t y) {
  if (y<x) std::swap(x,y);
  const uint8_t d = y-x;
  return d+(d<1)+(d<3);
}

uint8_t absdiff(uint8_t x,uint8_t y) {
  if (y<x) std::swap(x,y);
  return y-x;
}

uint8_t f(uint8_t x,uint8_t y) {
  //std::cout << int(absdiff(16,x)+16*absdiff(y,x*x)) << std::endl; 
  return absdiff(16,x)+absdiff(x,y);
}

int main() {
  uint8_t d[2][2][2];
  uint8_t x = 30;
  uint8_t y = 0;
  uint8_t v = f(x,y);
  for (size_t i=0;i!=16;++i) {
    std::cout << int(x) << " " << int(y) << " " << int(f(x,y)) << std::endl;
    uint8_t _x;
    for (_x = x;_x>  0;--_x) if (f(_x,y)!=f(x,y)) break;
    d[0][0][0] = _x;
    d[0][0][1] = f(_x,y);
    for (_x = x;_x<255;++_x) if (f(_x,y)!=f(x,y)) break;
    d[0][1][0] = _x;
    d[0][1][1] = f(_x,y);
    uint8_t _y;
    for (_y = y;_y>  0;--_y) if (f(x,_y)!=f(x,y)) break;
    d[1][0][0] = _y;
    d[1][0][1] = f(x,_y);
    for (_y = y;_y<255;++_y) if (f(x,_y)!=f(x,y)) break;
    d[1][1][0] = _y;
    d[1][1][1] = f(x,_y);
    if ((d[0][0][1]<f(x,y))||(d[0][1][1]<f(x,y))) {
      if (d[0][0][1]<d[0][1][1]) x = d[0][0][0];
      if (d[0][0][1]>d[0][1][1]) x = d[0][1][0];
    }
    if ((d[1][0][1]<f(x,y))||(d[1][1][1]<f(x,y))) {
      if (d[1][0][1]<d[1][1][1]) y = d[1][0][0];
      if (d[1][0][1]>d[1][1][1]) y = d[1][1][0];
    }
  }
  for (size_t i=0;i!=16;++i) {
    std::cout << int(x) << " " << int(y) << " " << int(f(x,y)) << std::endl;
    uint8_t min_x = x>  0?x-1:0;
    uint8_t max_x = x<255?x+1:255;
    uint8_t min_y = y>  0?y-1:0;
    uint8_t max_y = y<255?y+1:255;
    uint8_t best_x = x;
    uint8_t best_y = y;
    uint8_t best_v = f(x,y);
    for (size_t _y=min_y;_y!=max_y;++_y) {
      for (size_t _x=min_x;_x<=max_x;++_x) {
        if (f(_x,_y)>=best_v) continue;
        best_x = _x;
        best_y = _y;
        best_v = f(_x,_y);
      }
    }
    x = best_x;
    y = best_y;
  }
}
