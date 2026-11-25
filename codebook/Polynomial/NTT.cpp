template <class U> struct NTT {
  U m, rt[64];
  NTT(U m, U g) : m(m) {
    int lv = countr_zero(m - 1);
    U r = 1;
    for (U e = (m - 1) >> lv; e; e >>= 1, g = mul(g, g))
      if (e & 1) r = mul(r, g);
    for (rt[lv] = r; lv--;) rt[lv] = mul(rt[lv + 1], rt[lv + 1]);
  }
  template <bool inv = false> void ntt(vec<U>& a) const {
    int n = a.size();
    assert(n && !(n & (n - 1)) && (m - 1) % n == 0);
    for (int i = 1, j = 0; i < n; i++) {
      for (int b = n >> 1; !((j ^= b) & b); b >>= 1) {}
      if (i < j) swap(a[i], a[j]);
    }
    for (int lv = 1, h = 1; h < n; lv++, h <<= 1)
      for (int i = 0; i < n; i += 2 * h) {
        U w = 1;
        for (int j = i; j < i + h; j++, w = mul(w, rt[lv])) {
          U x = a[j], y = mul(a[j + h], w);
          a[j] = add(x, y), a[j + h] = sub(x, y);
        }
      }
    if constexpr (inv) {
      reverse(a.begin() + 1, a.end());
      for (U z = m - (m - 1) / n; U& x : a) x = mul(x, z);
    }
  }
  vec<U> multiply(vec<U> a, vec<U> b) const {
    if (a.empty() || b.empty()) return {};
    int sz = a.size() + b.size() - 1, n = bit_ceil(0u + sz);
    a.resize(n), b.resize(n), ntt(a), ntt(b);
    for (int i = 0; i < n; i++) a[i] = mul(a[i], b[i]);
    return ntt<true>(a), a.resize(sz), a;
  }
};
