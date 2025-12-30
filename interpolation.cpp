#include <vector>
#include <iostream>
#include <cstdint>
#include <cmath>
#include <string>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::stoull;
using std::swap;
using std::vector;

constexpr bool onlypositive = false;

/*                  1
 *                1   1
 *              1   2   1
 *            1   3   3   1
 *          1   4   6   4   1
 *        1   5  10  10   5   1
 *      1   6  15  20  15   6   1
 *    1   7  21  35  35  21   7   1
 *  1   8  29  56  70  56  29   8   1
 */

/*                  1  -1              
 *                1   0  -1            
 *              1   1  -1  -1          
 *            1   2   0  -2  -1        
 *          1   3   2  -2  -3  -1      
 *        1   4   5   0   5  -4  -1    
 *      1   5   9   5  -5   6  -5  -1 
 *
 *                  1/2    0   -1/2
 *               1/2   1/2  -1/2   -1/2
 *            1/2    1     0    -1    -1/2 
 *         1/2   3/2   1      1   -3/2   -1/2
 *      1/2    2    5/2    0   -5/2    -2   -1/2
 *   1/2   5/2   9/2   5/2  -5/2  -9/2   -5/2  1/2
 *
 */


// a-(a+b+c+d)/4, b-(a+b+c+d)/4, c-(a+b+c+d)/4, d-(a+b+c+d)/4

double target(
    const size_t n,
    const vector<double>& data,
    const vector<double>& s,
          vector<double>& d
    )
{
  for (size_t i=0;i!=d.size();++i) d[i]=0;
  double v = 0;
  for (size_t i=1;i!=s.size()-1;++i) {
    double c = s[i-1]-2*s[i]+s[i+1];
    d[i-1] +=   c;
    d[i  ] -= 2*c;
    d[i+1] +=   c;
    v+=c*c/2;
  }
  for (size_t i=0;i!=data.size();++i) {
    double a = 0;
    for (size_t j=0;j!=n;++j) {
      a+=d[i*n+j];
    }
    a/=n;
    for (size_t j=0;j!=n;++j) {
      d[i*n+j]-=a;
    }
  }
  return v;
}

void update(
    const size_t n,
    const vector<double>& data,
    const vector<double>& s,
    const vector<double>& d,
          vector<double>& o
          )
{
  for (size_t i=0;i!=d.size();++i) {
    o[i]=s[i]-d[i];
    if constexpr (onlypositive) if (o[i]<0) o[i]=0;
  }
  for (size_t i=0;i!=data.size();++i) {
    double a = 0;
    size_t m = n;
    for (size_t j=0;j!=n;++j) a+=o[i*n+j];
    double b = 0;
    while (true) {
      b=(a/m-data[i]);
      if constexpr (onlypositive) {
        size_t k = 0;
        for (size_t j=0;j!=n;++j) {
          k+=o[i*n+j]>=b;
        }
        if (k==m) break;
        m = k;
      } else {
        break;
      }
    }
    for (size_t j=0;j!=n;++j) {
      o[i*n+j]-=b;
      if constexpr (onlypositive) if (o[i*n+j]<0) o[i*n+j]=0;
    }
  }
}

void optimize(
    const size_t n,
    const vector<double>& data,
          vector<double>& s0
    )
{
  const double  phi = (sqrt(5.0) + 1.0) * 0.5;
  const double iphi = 1.0/phi;
  double beta = 1.0;
  vector<double> s1 = s0;
  vector<double> d0(s0.size());
  vector<double> d1(s1.size());
  double v0 = target(n,data,s0,d0);
  for (size_t i=0;i!=1u<<8;++i) {
    double a = 0;
    for (size_t i=0;i!=d0.size();++i) a+=d0[i]*d0[i];
    const double c = 1.0/sqrt(a);
    for (size_t i=0;i!=d0.size();++i) d0[i]*=beta*c;
    update(n,data,s0,d0,s1);
    double v1 = target(n,data,s1,d1);
    // cerr << v1 << " " << beta << endl;
    if (v1<v0) {
      beta*=2;
      swap(s0,s1);
      swap(d0,d1);
      v0=v1;
    } else {
      beta*=iphi;
    }
  }
}

int main(int argc, char** argv) {
  vector<double> data;
  for (double x;cin>>x;data.push_back(x));
  if (argc>1) {
    const size_t n = stoull(argv[1]);
    vector<double> s0 = data;
    for (size_t m=1;m<n;) {
      const size_t k = ((m*2)<n)?m*2:n;
      s0.resize(data.size()*k);
      for (size_t i=s0.size()-1;i<s0.size();--i) s0[i]=s0[i*m/k];
      m = k;
      optimize(m,data,s0);
    }
    for (size_t i=0;i!=s0.size();++i) cout << s0[i] << endl;
  } else {
    for (size_t i=0;i!=data.size();++i) cout << data[i] << endl;
  }
}
