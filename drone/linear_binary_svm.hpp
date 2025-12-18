#include <algorithm>
#include <cmath>
#include <cstddef>
#include <limits>
#include <utility>
#include <vector>

struct classification_objective{
  std::vector<std::array<double,4>>& data;
  //std::tuple<double,std::array<double,4>,std::array<std::array<double,4>,4>>
  std::tuple<double,std::array<double,4>>
  operator()(
      const std::array<double,4>& plane,
      const double mind = 0
  ) const
  {
    // ax + bx + cz + d = 0
    const double a = plane[0], b = plane[1], c = plane[2], d = plane[3];
    double f = 0;
    std::array<double,4> der{0.0,0.0,0.0,0.0};
    std::array<std::array<double,4>,4> h;
    double minp =  std::numeric_limits<double>::infinity();
    double minm = -std::numeric_limits<double>::infinity();
    size_t m = 0;
    for (size_t i=0;i!=2;++i) {
      for (auto it=data.begin();it!=data.end();++it) {
        const double x = (*it)[0], y = (*it)[1], z = (*it)[2], s = (*it)[3];
        const double n2 = a*a+b*b+c*c;
        const double n = sqrt(n2);
        const double n3 = n*n2;
        const double t = x*a+y*b+z*c+d;
        const double u = t/n;
        if (i==0) {
          if (s<0) if (u>minm) minm = u;
          if (s>0) if (u<minp) minp = u;
          if ((t>0)==(s>0)) continue;
        } else {
          if (s>0) if (u>minp) continue;
          if (s<0) if (u<minm) continue;
          ++m;
        }
        const double v = t/n2;
        f += s*u;
        der[0] += s*(x-a*v)/n;
        der[1] += s*(y-b*v)/n;
        der[2] += s*(z-c*v)/n;
        der[3] += s/n;
        h[0][0] += s*(-t+3*a*a*v-2*a*x  )/n3;
        h[0][1] += s*(   3*a*b*v-a*y-b*x)/n3;
        h[0][2] += s*(   3*a*c*v-a*z-c*x)/n3;
        h[0][3] += s*(-a                )/n3;
        h[1][0] += s*(   3*a*b*v-a*y-b*x)/n3;
        h[1][1] += s*(-t+3*b*b*v-2*b*y  )/n3;
        h[1][2] += s*(   3*b*c*v-b*z-c*y)/n3;
        h[1][3] += s*(-b                )/n3;
        h[2][0] += s*(   3*a*c*v-a*z-c*x)/n3;
        h[2][1] += s*(   3*b*c*v-b*z-c*y)/n3;
        h[2][2] += s*(-t+3*c*c*v-2*c*z  )/n3;
        h[2][3] += s*(-c                )/n3;
        h[3][0] += s*(-a                )/n3;
        h[3][1] += s*(-b                )/n3;
        h[3][2] += s*(-c                )/n3;
        h[3][3] += 0;
      }
      if (f<0) return {f,der};
    }
    f/=m;
    der[0]/=m;
    der[1]/=m;
    der[2]/=m;
    der[3]/=m;
    return {f,der};
  }
};

/*
template <typename T> int signum(T val) {
    return (T(0) < val) - (val < T(0));
}

struct classification_objective{
  std::vector<std::array<double,4>>& data;
  //std::tuple<double,std::array<double,4>,std::array<std::array<double,4>,4>>
  std::tuple<double,std::array<double,4>>
  operator()(
      const std::array<double,4>& plane
  ) const
  {
    // ax + bx + cz + d = 0
    const double a = plane[0], b = plane[1], c = plane[2], d = plane[3];
    double f = 0;
    std::array<double,4> der{0.0,0.0,0.0,0.0};
    for (auto it=data.begin();it!=data.end();++it) {
      const double x = (*it)[0], y = (*it)[1], z = (*it)[2], s = (*it)[3];
      const double n2 = a*a+b*b+c*c;
      const double n = sqrt(n2);
      const double n3 = n*n2;
      const double t = x*a+y*b+z*c+d;
      const double u = t/n;
      const double v = t/n2;
      f += sqrt(abs(s*u))*signum(s*u);
      der[0] += s*(x-a*v)/n/(2*sqrt(abs(s*u)));
      der[1] += s*(y-b*v)/n/(2*sqrt(abs(s*u)));
      der[2] += s*(z-c*v)/n/(2*sqrt(abs(s*u)));
      der[3] += s        /n/(2*sqrt(abs(s*u)));
    }
    return {f,der};
  }
};
*/

void test_derivative(
    const classification_objective& o,
    const std::array<double,4>& plane
    )
{
  const auto [v0,d0] = o(plane);
  const double epsilon = 1e-12;
  for (size_t i=0;i!=4;++i) {
    std::array<double,4> plus = plane;
    std::array<double,4> minus = plane;
    plus[i] +=0.5*epsilon;
    minus[i]-=0.5*epsilon;
    const auto [vp,dp] = o(plus);
    const auto [vm,dm] = o(minus);
    std::cout << d0[i] << " " << (vp-vm)/epsilon << std::endl;
  }
  std::cout << std::endl; 
}

std::array<double,4> linear_binary_svm(
    std::vector<std::array<double,4>>& data
    )
{
  size_t np = 0;
  size_t nm = 0;
  for (auto it=data.begin();it!=data.end();++it) {
    if ((*it)[3]<0) ++nm;
    if ((*it)[3]>0) ++np;
  }
  if (nm==0) return std::array<double,4>{};
  if (np==0) return std::array<double,4>{};
  /* 
  for (auto it=data.begin();it!=data.end();++it) {
    if ((*it)[3]>0) (*it)[3]*=1.0/np;
    if ((*it)[3]<0) (*it)[3]*=1.0/nm;
  }
  */
  
  std::random_device rd;
  std::mt19937 gen(rd());
  std::normal_distribution<double> nd(0.0,1.0);
  std::array<double,4> x0{nd(gen),nd(gen),nd(gen),nd(gen)};
  //x0[0] = 1;
  //x0[1] = 1;
  //x0[2] = 1;
  //x0[3] = 1;
  const double norm=sqrt(x0[0]*x0[0]+x0[1]*x0[1]+x0[2]*x0[2]);
  x0[0]/=norm;
  x0[1]/=norm;
  x0[2]/=norm;
  std::vector<std::tuple<double,size_t>> projection(data.size());
  classification_objective o{data};
  auto [v0,der0] = o(x0);
  if (v0<0) {
    x0[0]=-x0[0];
    x0[1]=-x0[1];
    x0[2]=-x0[2];
    x0[3]=-x0[3];
    auto [v1,der1] = o(x0);
    v0 = v1;
    der0 = der1; 
  }
  const double  phi = 0.5*(1+sqrt(5));
  const double iphi = 0.5*(sqrt(5)-1);
  double step = 1.0;
  double mind = 0;
  for (size_t i=0;i!=64;++i) {
    double norm = sqrt(der0[0]*der0[0]+der0[1]*der0[1]+der0[2]*der0[2]);
    //std::cerr << "norm = " << norm << std::endl;
    const std::array<double,3> q = {x0[0]/norm,x0[1]/norm,x0[2]/norm};
    const double a = (q[0]*der0[0]+q[1]*der0[1]+q[2]*der0[2]);
    const std::array<double,3> p = {q[0]*a,q[1]*a,q[2]*a};
    std::array<double,4> x1 = {der0[0]-p[0],der0[1]-p[1],der0[2]-p[2],0.0};
    norm=sqrt(x1[0]*x1[0]+x1[1]*x1[1]+x1[2]*x1[2]);
    x1[0]*=step/norm;
    x1[1]*=step/norm;
    x1[2]*=step/norm;
    x1[0]+=x0[0];
    x1[1]+=x0[1];
    x1[2]+=x0[2];
    norm=sqrt(x1[0]*x1[0]+x1[1]*x1[1]+x1[2]*x1[2]);
    x1[0]/=norm;
    x1[1]/=norm;
    x1[2]/=norm;
    x1[3]=x0[3];
    //test_derivative(o,x1);
    for (size_t i=0;i!=data.size();++i) {
      std::get<0>(projection[i]) =
         data[i][0]*x1[0]
        +data[i][1]*x1[1]
        +data[i][2]*x1[2];
      std::get<1>(projection[i]) = i;
      //std::cout << std::get<0>(projection[i]) << " "
      //          << std::get<1>(projection[i])<< std::endl;
    }
    std::nth_element(
        projection.begin(),
        projection.begin()+nm,
        projection.end()
        );
    std::nth_element(
        projection.begin(),
        projection.begin()+nm-1,
        projection.end()
        );
    /*
    std::sort(projection.begin(),projection.end());
    for (auto it=projection.begin();it!=projection.end();++it) {
      std::cout << get<0>(*it) << " " << data[get<1>(*it)][3] << std::endl;
    }
    */
    x1[3] = -0.5*(get<0>(projection[nm-1])+get<0>(projection[nm]));
    //std::cerr << "d = " << x1[3] << std::endl;
    bool linearly_separable = true;
    for (size_t i=0;i!=nm;++i) {
      if (data[get<1>(projection[i])][3]>0) linearly_separable = false;
    }
    for (size_t i=nm;i!=data.size();++i) {
      if (data[get<1>(projection[i])][3]<0) linearly_separable = false;
    }
    if (linearly_separable) {
      //std::cerr << " data is linearly separable " << std::endl;
      //break;
    }
    mind = 0.5*(get<0>(projection[nm])-get<0>(projection[nm-1]));
    //std::cerr << "mind = " << mind << std::endl;
    //std::cout << " -> " << x1[3] << std::endl;
    auto [v1,der1] = o(x1);
    //std::cerr << "v1= " << v1  << std::endl;
    if (v1>v0) {
      x0 = x1;
      v0 = v1;
      der0 = der1;
      step*=phi;
    } else {
      step*=exp(-1);
    }
    //std::cerr << "step = " << step << std::endl;
  }
  
  return x0;
}
