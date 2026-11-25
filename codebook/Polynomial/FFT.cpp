struct FFT {
  using C = complex<double>; C rt[30];
  FFT() {
    for (int lv = 1, h = 1; lv < 30; lv++, h <<= 1) rt[lv] = polar(1.0, numbers::pi / h);
  }
  template <bool inv = false> void ntt(vec<C>& a) const {
    // same as NTT (drop m, replace add/sub/mul with + - *)
    if constexpr (inv) {
      reverse(a.begin() + 1, a.end());
      for (C& x : a) x /= n;
    }
  }
}; // after multiply, round x.real()