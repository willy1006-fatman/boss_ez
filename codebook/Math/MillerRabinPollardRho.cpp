// Deterministic for all n < 2^64.
bool is_prime(u64 n) {
  if (n < 3 || n % 2 == 0) return n == 2;
  int s = countr_zero(n - 1);
  u64 d = (n - 1) >> s;
  Mont<u64> mo(n);
  u64 one = mo.encode(1), neg = n - one;
  for (u64 a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
    if (!(a = mo.encode(a))) continue;
    u64 x = one;
    for (u64 e = d; e; e >>= 1, a = mo.mul(a, a))
      if (e & 1) x = mo.mul(x, a);
    if (x == one) continue;
    for (int i = 1; i < s && x != neg; ++i) x = mo.mul(x, x);
    if (x != neg) return false;
  }
  return true;
}
// n >= 2: return a proper factor of n, or n itself if n is prime.
u64 pollard_rho(u64 n) {
  if (n % 2 == 0) return 2;
  if (n == 1 || is_prime(n)) return n;
  static mt19937_64 rng(random_device{}());
  Mont<u64> mo(n);
  while (true) {
    u64 y = mo.encode(rng()), c = mo.encode(rng()), g = 1;
    for (int len = 1; g == 1; len *= 2) {
      u64 x = y, p = 1;  // A plain p times an encoded difference stays plain.
      for (int i = 1; i <= len && g == 1; ++i) {
        y = mo.add(mo.mul(y, y), c), p = mo.mul(p, mo.sub(x, y));
        if (i % 64 == 0 || i == len) g = gcd(p, n);
      }
    }
    if (g != n) return g;  // g == n: the cycle closed, retry.
  }
}
// Append the prime factors of n >= 1 to res, with multiplicity, unsorted.
auto factorize(u64 n) {
  vec<u64> res;
  auto dfs = [&](auto&& self, u64 x) -> void {
    if (auto d = pollard_rho(x); d != x)
      self(self, d), self(self, x / d);
    else if (d != 1) res.emplace_back(d);
  }; dfs(dfs, n);
  ranges::sort(res); return res;
}
