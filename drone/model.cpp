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

constexpr double naca0012(const double x)
{
  if ((x>0)&&(x<1)) {
    return
      0.594689181*(
        0.298222773*sqrt(x)
        +x*(-0.127125232+x*(-0.357907906+x*(0.291984971-x*0.105174606)))
      );
  }
  return 0;
}

uint8_t inside_wing(
    double x, // mm
    double y, // mm
    double z  // mm
    )
{
  const double width = 100;
  const double height = 100;
  if (x<0) x=-x;
  if (x> 300) return 0;
  if (y<-height) return 0;
  if (y> height) return 0;
  if (x> 100) {
    x-=100;
  } else {
    return 0;
  }
  x*=0.5;
  const double halfcord = sqrt(height*height-x*x);
  if (y> halfcord) return 0;
  if (y<-halfcord) return 0;
  const double p = halfcord*naca0012(1-(0.5*(y/halfcord)+0.5));
  if (z<0) z=-z;
  if (z>p) return 0;
  return 1;
}

uint8_t inside_duct(
    double x,
    double y,
    double z
    )
{
  const double length = 160;
  const double radius = 100;
  const double yend = 100-length;
  if (y<yend) return 0;
  if (y>100) return 0;
  double r = sqrt(x*x+z*z);
  const double p = length*naca0012(1-(y-yend)/length);
  if (abs(r-radius)>p) return 0;
  return 1;
}

uint8_t inside_thrustvector(
    double x,
    double y,
    double z
    )
{
  const double a = - 30;
  const double b = -100;
  const double radius = 100;
  if (y>a) return 0;
  if (y<b) return 0;
  const double r = sqrt(x*x+z*z);
  if (r>radius) return 0;
  const double p = (a-b)*naca0012(1-(y-b)/(a-b));
  if (abs(z)>p) return 0;
  return 1;
}

uint8_t inside_pod(
    double x,
    double y,
    double z
    )
{
  const double a = 80;
  const double b =-60;
  if (y>a) return 0;
  if (y<b) return 0;
  double r = sqrt(x*x+z*z);
  const double p = (a-b)*naca0012(1-(y-b)/(a-b));
  if (abs(r)>3*p) return 0;
  return 1;
}

uint8_t inside_stator_vanes(
    double x,
    double y,
    double z
    )
{
  double _x = 0.923879532511287*x+0.38268343236509 *z;
  double _z = -0.38268343236509*x+0.923879532511287*z;
  swap(x,_x);
  swap(z,_z);
  const double a =  20;
  const double b =   0;
  const double radius = 100;
  if (y>a) return 0;
  if (y<b) return 0;
  const double r = sqrt(x*x+z*z);
  if (r>radius) return 0;
  if (x<0) x=-x;
  if (z<0) z=-z;
  if (z>x) swap(x,z);
  _x =  sqrt(0.5)*x+sqrt(0.5)*z;
  _z = -sqrt(0.5)*x+sqrt(0.5)*z;
  if (_x>x) {
    swap(x,_x);
    swap(z,_z);
  }
  const double p = (a-b)*naca0012(1-(y-b)/(a-b));
  if (abs(z)>p) return 0;
  return 1;
}

uint8_t inside_drone(double x,double y,double z) {
    return 0
      |inside_wing(x,y,z)
      |inside_duct(x,y,z)
      |inside_thrustvector(x,y,z)
      |inside_thrustvector(z,y,x)
      |inside_pod(x,y,z)
      |inside_stator_vanes(x,y,z);
}

int sign(double x) {
  return (x>0)-(x<0);
}

int main() {
  std::random_device rd;
  std::mt19937 gen; // gen(rd());
  std::normal_distribution<double> nd(0.0,1.0);
  std::uniform_real_distribution<double> rand(0,1);
  /*
  std::uniform_real_distribution<double> xd(-300,300);
  std::uniform_real_distribution<double> yd(-140,140);
  std::uniform_real_distribution<double> zd(-100,100);
  */
  if (false) {
    cout << "ply\n";
    cout << "format ascii 1.0\n";
    cout << "element vertex N\n";
    cout << "property float x\n";
    cout << "property float y\n";
    cout << "property float z\n";
    cout << "end_header\n";
  }
  std::vector<std::array<double,4>> data;
  for (int z=-111;z!=111;++z) {
    for (int y=-101;y!=101;++y) {
      for (int x=-301;x!=301;++x) {
        uint64_t v = 0;
        for (size_t i=0;i!=4;++i) {
          for (size_t j=0;j!=4;++j) {
            for (size_t k=0;k!=4;++k) {
              const double xd = x+i*0.5-0.25;
              const double yd = y+j*0.5-0.25;
              const double zd = z+k*0.5-0.25;
              const uint64_t d = inside_drone(xd,yd,zd);
              v|=d<<(i+4*(j+4*k));
            }
          }
        }
        const size_t idx = 602*((x+301)+202*((y+101)+222*(z+111)));
        if (v==~uint64_t(0)) continue;
        if (v==0)            continue;
        data.clear();
        // cout << x << " " << y << " " << z << " " << popcount(v) << endl;
        for (size_t i=0;i!=64;++i) {
          const double xd = nd(gen);
          const double yd = nd(gen);
          const double zd = nd(gen);
          const double sd = inside_drone(x+xd+0.5,y+yd+0.5,z+zd+0.5)?1.0:-1.0;
          //cout << xd << " " << yd << " " << zd << " " << sd << endl;
          data.push_back({xd,yd,zd,sd});
        }
        //cout << "data.size() = " << data.size() << endl;
        {
          size_t np = 0;
          size_t nm = 0;
          for (auto it=data.begin();it!=data.end();++it) {
            if ((*it)[3]>0) {
              ++np;
              continue;
            }
            if ((*it)[3]<0) {
              ++nm;
              continue;
            }
          }
          if (np==0) continue;
          if (nm==0) continue;
          if (np<16) continue;
          if (nm<16) continue;
        }
        const std::array<double,4> plane = linear_binary_svm(data);
        cout << x-(plane[0]*plane[3]-0.5) << " "
             << y-(plane[1]*plane[3]-0.5) << " "
             << z-(plane[2]*plane[3]-0.5) << endl; 
      }
    }
  }
}
