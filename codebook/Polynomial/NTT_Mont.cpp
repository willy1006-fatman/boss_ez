// Number theoretic transform over an odd modulus mod with primitive root g, inheriting Mont.
// ntt<inv>(a): in-place transform, size a power of two dividing mod - 1. ntt<true> is scaled so
// that exactly one pointwise mul per element in between makes the round trip exact, as in
// multiply; a bare forward-inverse pair multiplies values by R, and k pointwise muls need k - 1
// correction muls by r2 per element.
template <class U> struct NTT : Mont<U> {
  using Mont<U>::m, Mont<U>::r2, Mont<U>::mul, Mont<U>::add, Mont<U>::sub;
  array<U, 33> rt{}, zi{};  // rt[lv]: R-encoded 2^lv-th root; zi[lv]: (2^lv)^-1 * R^2
  NTT(U mod, U g) : Mont<U>(mod) {
    zi[0] = r2;  // The R^2 factor corrects the R^-1 of one pointwise montmul.
    int lv = min(32, countr_zero(U(mod - 1)));
    U x = mul(g, r2), r = mul(U(1), r2);  // Encoded base; the powers stay encoded.
    for (U n = (mod - 1) >> lv; n; n >>= 1, x = mul(x, x)) if (n & 1) r = mul(r, x);
    rt[lv] = r;
    while (lv--) rt[lv] = mul(rt[lv + 1], rt[lv + 1]);  // Encoded roots: montmul(z, wR) == zw.
    U z1 = mul(U((mod >> 1) + 1), r2);  // Encoded inverse of 2: montmul(z, z1) halves z exactly.
    for (int i = 1; i < 33; ++i) zi[i] = mul(zi[i - 1], z1);
  }
  template <bool inv = false> void ntt(vec<U>& a) const {
    int n = int(a.size());
    assert(n && !(n & (n - 1)) && (m - 1) % U(n) == 0);
    for (int i = 1, j = 0; i < n; i++) {
      int b = n >> 1;
      for (; j & b; b >>= 1) j ^= b;
      j ^= b;
      if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2, lv = 1; len <= n; len <<= 1, lv++)
      for (int i = 0; i < n; i += len) {
        U w = rt[0];  // R-encoded 1
        for (int j = 0; j < len / 2; j++, w = mul(w, rt[lv])) {
          U x = a[i + j], y = mul(a[i + j + len / 2], w);
          a[i + j] = add(x, y);
          a[i + j + len / 2] = sub(x, y);
        }
      }
    if constexpr (inv) {
      reverse(a.begin() + 1, a.end());
      U z = zi[countr_zero(unsigned(n))];
      for (U& x : a) x = mul(x, z);
    }
  }
  vec<U> multiply(vec<U> a, vec<U> b) const {
    if (a.empty() || b.empty()) return {};
    int sz = int(a.size() + b.size() - 1), n = 1;
    while (n < sz) n <<= 1;
    a.resize(n), b.resize(n), ntt(a), ntt(b);
    for (int i = 0; i < n; i++) a[i] = mul(a[i], b[i]);  // The R^-1 is corrected by ntt<true>.
    ntt<true>(a);
    a.resize(sz);
    return a;
  }
};
