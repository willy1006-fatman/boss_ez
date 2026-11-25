template <class UI>
struct Mont {
  using UL = conditional_t<sizeof(UI) == 4, u64, u128>;
  static constexpr int W = sizeof(UI) * 8;
  UI m, inv, r2;
  explicit Mont(UI n) : m(n), inv(n) {
    assert(n > 1 && (n & 1));
    r2 = -UL(n) % n;
    for (int i = 0; i < 5; ++i) inv *= 2 - n * inv;
  }
  UI encode(UI a) const { return mul(a, r2); }
  UI add(UI a, UI b) const { return a >= m - b ? a - (m - b) : a + b; }
  UI sub(UI a, UI b) const { return a < b ? m - (b - a) : a - b; }
  UI mul(UI a, UI b) const {
    UL t = UL(a) * b;
    UI hi = t >> W, q = UI(t) * inv, k = UL(q) * m >> W;
    return hi - k + (hi < k ? m : 0);
  }
};