template <signed_integral Cap = i64> struct Dinic {
  struct E { int to, rev; Cap cap, flow; };
  int n, st, ed;
  vec<vec<E>> g;
  vec<int> lv, id;
  bool bfs() {
    lv.assign(n, 0); id.assign(n, 0);
    queue<int> q; lv[q.emplace(st)] = 1;
    while (!empty(q) && !lv[ed]) {
      int u = q.front(); q.pop();
      for (auto&& e : g[u]) if (e.flow < e.cap && !lv[e.to])
        lv[q.emplace(e.to)] = lv[u] + 1;
    }
    return lv[ed];
  }
  Cap dfs(int u, Cap f = numeric_limits<Cap>::max()) {
    if (u == ed) return f;
    Cap ret = 0;
    for (int& i = id[u]; i < ssize(g[u]); i++) {
      auto &[to, rev, cap, flow] = g[u][i];
      Cap rem = cap - flow;
      if (rem <= 0 || lv[to] != lv[u] + 1) continue;
      Cap nf = dfs(to, min(f, rem));
      flow += nf; g[to][rev].flow -= nf;
      ret += nf; f -= nf;
      if (f == 0) return ret;
    }
    if (ret == 0) lv[u] = 0;
    return ret;
  }
  void init(int n_) { g.assign(n = n_, vec<E>()); }
  void add_edge(int u, int v, Cap c) {
    int i = size(g[u]), j = size(g[v]);
    g[u].emplace_back(v, j + (u == v), c, 0);
    g[v].emplace_back(u, i, 0, 0);
  }
  Cap max_flow(int st_, int ed_) {
    st = st_, ed = ed_; Cap ret = 0;
    while (bfs()) ret += dfs(st);
    return ret;
  }
};
