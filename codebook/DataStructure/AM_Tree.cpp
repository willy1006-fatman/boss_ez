template <class T, class C = less<T>> struct AMTree {
  int c; vec<int> p, s;
  vec<T> t; C cmp;
  AMTree(int n, C cmp = {}) : c(n), p(n, -1), s(n, 1), t(n), cmp(cmp) {}
  void promote(int x) {
    int y = p[x]; s[y] -= s[x];
    if (p[y] != -1 && cmp(t[y], t[x])) p[x] = p[y];
    else s[x] += s[y], swap(t[x], t[y]), p[x] = exchange(p[y], x);
  }
  void calibrate(int x) {
    while (p[x] != -1)
      if (3 * s[x] <= 2 * s[p[x]]) x = p[x];
      else promote(x);
  }
  optional<T> add_edge(int u, int v, T w) {
    if (u == v) return w;
    calibrate(u), calibrate(v);
    while (p[u] != -1) promote(u);
    while (p[v] != -1) promote(v);
    if (p[u] == v) 
      return cmp(w, t[u]) ? exchange(t[u], w) : w;
    if (s[u] > s[v]) swap(u, v);
    s[v] += s[u], p[u] = v, t[u] = w, --c;
    return nullopt;
  }
  optional<T> path_max(int u, int v) {
    calibrate(u), calibrate(v);
    optional<T> r;
    for (; u != v; u = p[u]) {
      if (s[u] > s[v]) swap(u, v);
      if (p[u] == -1) return nullopt;
      if (!r || cmp(*r, t[u])) r = t[u];
    }
    return r;
  }
};