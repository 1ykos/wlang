vector<bool> wac_encode(size_t m) {
  size_t l = bit_width(m+(m==0));
  size_t n = l;
  vector<bool> wac;
  size_t a,b;
  for (a=1,b=1;(b<=n)&&(a<=b);(b=a+b,a=b-a)) wac.push_back(0);
  for (size_t i=0;i<wac.size();(++i,a=b-a,b=b-a)) {
    if (n>=a) {
      wac[wac.size()-i-1]=1;
      n-=a;
    }
  }
  wac.push_back(1);
  for (size_t i=0;i!=l;++i) {
    wac.push_back((m>>(l-i-1))&1u);
  }
  return wac;
}

size_t wac_decode_natural_number(const vector<bool> wac) {
  size_t l=0,i=0;
  for (size_t a=1,b=1,c=0;;(b=a+b,a=b-a,++i)) {
    c=(wac[i]*(++c));
    if (c<2) l+=b*wac[i];
    else break;
  }
  ++i;
  size_t n = 0;
  for (;i!=wac.size();++i) n=(n<<1)+wac[i];
  return n;
}
