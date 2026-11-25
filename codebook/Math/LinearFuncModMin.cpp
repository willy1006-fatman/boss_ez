// Input:  f(x) = (a*x+b) mod mod (x >= 0), 0 <= a,b < mod.
// Output: positions where f strictly updates its prefix minimum.
//         X.size() = DX.size()+1, and the i-th arithmetic progression is
//         X[i], X[i]+DX[i], ..., X[i+1] (neighboring progressions share ends).
pair<vec<i64>, vec<i64>> min_of_linear_segments(i64 a, i64 b, i64 mod) {
  assert(0 <= a && a < mod && 0 <= b && b < mod);
  vec<i64> X{0}, DX;
  i64 g = gcd(a, mod);
  a /= g; b /= g; mod /= g;

  // p/q <= (mod-a)/mod <= r/s
  i64 p = 0, q = 1, r = 1, s = 1;
  i64 dl = mod - a, dr = a, x = 0, y = b;
  while (y) {
    i64 k = dr / dl;
    dr %= dl;
    if (!dr) --k, dr = dl;
    r += k * p; s += k * q;
    while (true) {
      k = dl > y ? (dl - y - 1) / dr + 1 : 0;
      if (k > (dl - 1) / dr) break;
      dl -= k * dr; p += k * r; q += k * s;
      k = y / dl;
      y -= k * dl; x += k * q;
      X.push_back(x); DX.push_back(q);
    }
    k = dl / dr;
    dl -= k * dr; p += k * r; q += k * s;
  }
  return {X, DX};
}