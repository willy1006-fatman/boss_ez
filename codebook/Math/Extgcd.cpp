//a * p.first + b * p.second = gcd(a, b)
pair<i64, i64> extgcd(i64 a, i64 b) {
  if (b == 0) return {1, 0};
  auto [y, x] = extgcd(b, a % b);
  return pair{x, y - (a / b) * x};
}