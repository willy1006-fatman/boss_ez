auto HopcroftKarp(int N, int M, auto&& g) {
  bool mt = true;
  vec<int> l(N, -1), r(M, -1), d(N);
  auto L{views::iota(0, N)};
  while (1) {
    queue<int> q; d.assign(N, -1);
    for (auto u : L) if (l[u] == -1) d[q.emplace(u)] = 0;
    while (!empty(q)) {
      auto u = q.front(); q.pop();
      for (auto v : g[u])
        if (r[v] != -1 && d[r[v]] == -1)
        d[q.emplace(r[v])] = d[u] + 1;
    }
    if (!mt) break;
    mt = false;
    auto dfs = [&](auto&& self, int u) -> bool {
      for (auto v : g[u])
        if (r[v] == -1 || (d[r[v]] == d[u] + 1 && self(self, r[v])))
          return (l[u] = v, r[v] = u, d[u] = -1), true;
      d[u] = -1; return false;
    };
    for (auto u : L) if (l[u] == -1 && dfs(dfs, u)) mt = true;
  }
  return tuple{move(l), move(r), move(d)};
}
// vertex cover : u if d[u] == -1 for u in L and l[u] if l[u] != -1 for u in L