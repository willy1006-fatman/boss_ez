void fwt(vector <int> &a, bool inv = false) {
  // and : x += y * (1, -1)
  // or  : y += x * (1, -1)
  // xor : x = (x + y) * (1, 1/2)
  //       y = (x - y) * (1, 1/2)
  int n = __lg(sz(a));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < 1 << n; ++j) if (j >> i & 1) {
      int x = a[j ^ (1 << i)], y = a[j];
      // do something
    }
  }
}
vector<int> subs_conv(vector<int> a, vector<int> b) {
  // c_i = sum_{j & k = 0, j | k = i} a_j * b_k
  // Let f_i = a_i * x^(popcnt(i)), g_i = b_i * x^(popcnt(i))
  // do or fwt on f and g, then mult them pointwise
  // do inverse fwt and extract [popcnt(i)]f_i, done.
}