#include <bit>
#include <cstdint>
#include <iostream>
#include <string>
#include <limits>
#include <vector>

using std::bit_width;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

std::string lambda_numeral(
    const size_t n
    )
{
  std::string numeral = "λf.λx.";
  for (size_t i=0;i!=n;++i) numeral+="(f ";
  numeral+="x";
  for (size_t i=0;i!=n;++i) numeral+=" )";
  return numeral;
}

std::string standard_numeral(
    const int i
    )
{
  return std::string("( f x ) => ( f^")
        +(i<0?"⁻":"")
        +std::to_string(i<0?-i:i)
        +" x )";
}

class bit;
class bitstring;

class const_bit{
  private:
    const uint8_t v;
  public:
    const_bit(const bool b):v(b){}
    operator bool() const
    {
      return v!=0;
    }
};

class bit{
  private:
    size_t &r;
    size_t mask;
  public:
    bit(size_t &r,size_t mask):r(r),mask(mask){}
    bit& operator=(const bool v)
    {
      size_t t = size_t(0)-v;
      r=(r&(~mask))    // reset bit
           +(mask&t);  // set bit
      return *this;
    }
    operator bool() const
    {
      return (r&mask)!=0;
    }
    // copy assignment impossible
};

class bitstring{
  private:
    vector<size_t> data;
  public:
    bitstring(const size_t s=0)
    {
      constexpr size_t d = std::numeric_limits<size_t>::digits;
      data.resize((s+d-1)/d);
    }
    bit operator[](const size_t i)
    {
      constexpr size_t d = std::numeric_limits<size_t>::digits;
      return bit(data[i/d],(size_t(1))<<(d-1-(i%d)));
    }
    const_bit operator[](const size_t i) const {
      constexpr size_t d = std::numeric_limits<size_t>::digits;
      return const_bit(data[i/d]&(size_t(1))<<(d-1-(i%d)));
    }
    const size_t size() const
    {
      return data.size();
    }
    const size_t extract_size_t(
        const size_t b,
        const size_t e
        ) const
    {
      constexpr size_t d = std::numeric_limits<size_t>::digits;
      const size_t lov = (data[b/d]>>(d-(e%d)))&((~size_t(0))>>(d-(e-b)));
      return lov;
    }
    const void write_size_t(
        const size_t num,
        const size_t len,
        const size_t pos
        )
    {
      constexpr size_t d = std::numeric_limits<size_t>::digits;
      const size_t val = num&((~size_t(0))>>(d-1-len));
      const size_t a = data[(pos%d)+0];
      const size_t b = data[(pos%d)+1];
      const size_t mask_lo = size_t(0)-((size_t(1))<<(d-1-(pos%d)));
      //const size_t mask_hi = (~size_t(0))<<(
      data[pos/d] = (data[pos/d])|(val<<(d-1-len-(pos%d)));
    }
    const void print()
    {
      for (auto it=data.begin();it!=data.end();++it) {
        for (size_t i=0;i!=64;++i) cout << (*this)[i];
        cout << endl;
      }
    }
};

bitstring wac_encode(size_t m)
{
  size_t l = bit_width(m+(m==0));
  size_t n = l;
  size_t a,b,j=0;
  for (a=1,b=1;(b<=n)&&(a<=b);(b=a+b,a=b-a,++j));
  bitstring wac(j+l+1);
  for (size_t i=0;i!=j;(++i,a=b-a,b=b-a)) {
    wac[j-i-1]=(n>=a);
    if (n>=a) n-=a;
  }
  //cerr << j << " " << wac[0] << wac[j] << endl;
  wac[j]=1;
  //cerr << j << " " << wac[0] << wac[j] << endl;
  wac.write_size_t(m,l-1,j+1);
  return wac;
}

// 0 => 0
//-1 => 1
// 1 => 2
//-2 => 3
bitstring wac_encode(ptrdiff_t i)
{
  size_t n = (i<0)?-i:i;
  n=(n<<1)-(i<0);
  return wac_encode(n);
}

size_t wac_decode_natural_number(const bitstring& wac)
{
  size_t l=0,i=0;
  for (size_t a=1,b=1,c=0;;(b=a+b,a=b-a,++i)) {
    c=(wac[i]*(++c));
    if (c<2) l+=b*wac[i];
    else break;
  }
  ++i;
  return wac.extract_size_t(i,i+l);
}

// 0 => 0
// 1 =>-1
// 2 => 1
// 3 =>-2
ptrdiff_t wac_decode_integer(const bitstring& wac)
{
  const size_t n = wac_decode_natural_number(wac);
  ptrdiff_t i = n>>1;
  i*=(n&1)?-1:1;
  return i;
}

// 0 function 
// 1 variable
// function (int)iterations (num)args (num)length <body>
// (num)variable
// (num)apply_args function
// 
//

void write_church_numeral(
    bitstring& s,
    int i
    )
{
  // 0 universal_encode(i) universal_encode(2)  
  s[0] = 0;
  
}

int main()
{
  for (size_t j=0;;++j) {
    bitstring n = wac_encode(j);
    n.print();
    //cerr << "from 0 to 1=" << " " << n.extract_size_t(0,1) << endl;
    const size_t k = wac_decode_natural_number(n);
    cout << j << " == " << k << endl;
    //for (size_t i=0;i!=n.size();++i) {
    //  cout << n[i];
    //}
    //cout << endl;
  }
}
