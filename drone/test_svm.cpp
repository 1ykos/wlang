#include <bit>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

#include "linear_binary_svm.hpp"

using std::array;
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::popcount;
using std::swap;
using std::vector;
using std::tuple;

int main() {
  /*
  vector<array<double,4>> data =
  {
    {-0.544086 , 0.0,  0.265705, -1},
    { 0.0516259, 0.0, -2.47931 , -1},
    {-1.19133  , 0.0,  1.96499 ,  1},
    {-2.09972  , 0.0,  0.115673, -1},
    { 1.15139  , 0.0, -1.53007 , -1},
    {-0.886469 , 0.0,  1.03141 ,  1},
    {-1.10816  , 0.0, -0.978069, -1},
    { 0.201789 , 0.0,  0.640714,  1},
  };
  */
  
  std::random_device rd;
  std::mt19937 gen(rd());
  std::normal_distribution<double> nd(0.0,1.0);
  vector<array<double,4>> data;
  for (size_t i=0;i!=16;++i) {
    const double x = nd(gen);
    const double y = nd(gen);
    const double z = nd(gen);
    //const double sd = (((x-1.5)*(x-1.5)+(y-1.5)*(y-1.5)+(z-1.5)*(z-1.5))<4)?-1.0:1.0;
    const double sd = ((x+y+z)<0.25)?-1.0:1.0;
    data.push_back({x,y,z,sd});
    cerr << x << " " << y << " " << z << " " << sd << endl;
  }
  
  const array<double,4> plane = linear_binary_svm(data);
  cout << plane[0] << " "
       << plane[1] << " "
       << plane[2] << " "
       << plane[3] << endl;
  if (abs(plane[2])>0) {
    cout << (plane[2]>0?"-":"");
    cout << "(" << plane[0] << "*x";
    cout << (plane[1]<0?"-":"+") << abs(plane[1]) << "*y";
    cout << (plane[3]<0?"-":"+") << abs(plane[3]) << ")";
    cout << "/"                  << abs(plane[2]) << endl;
  }
  return 0;
}
