#include<bits/stdc++.h>
using namespace std;
#define int long long

//BEGIN CUT HERE
struct HLDecomposition {
  vector<vector<int>> g;

  vector<int> vid, head, heavy, parent, depth, inv;
  
  HLDecomposition(){}
  HLDecomposition(int n){init(n);}

  void init(int n){
    for(auto &a:g) a.clear(); 
    g.clear();
    vid.clear();
    head.clear();
    heavy.clear();
    parent.clear();
    depth.clear();
    inv.clear();
    
    g.resize(n);
    vid.resize(n, -1);
    head.resize(n);
    heavy.resize(n, -1);
    parent.resize(n);
    depth.resize(n);
    inv.resize(n);
  }
  
  void add_edge(int u, int v) {
    g[u].push_back(v);
    g[v].push_back(u);
  }

  void build() {
    dfs(0, -1);
    bfs();
  }

  typedef tuple<int,int,int,int,int,int> T;
  int dfs(int curr, int prev) {
    stack<T> st;
    int result;
    int sub, max_sub, i, next;
  ENTRYPOINT:
    parent[curr] = prev;
    sub = 1;
    max_sub = 0;
    for(i=0;i<(int)g[curr].size();i++){
      next = g[curr][i];
      if (next != prev) {
	depth[next] = depth[curr] + 1;
	{
	  st.emplace(curr,prev,sub,max_sub,i,next);
	  prev=curr;curr=next;
	  goto ENTRYPOINT;
	}
      RETURNPOINT:
	T t=st.top();st.pop();
	curr    = get<0>(t);
	prev    = get<1>(t);
	sub     = get<2>(t);
	max_sub = get<3>(t);
	i       = get<4>(t);
	next    = get<5>(t);
	
	int sub_next=result;
	sub += sub_next;
	if (max_sub < sub_next) max_sub = sub_next, heavy[curr] = next;
      }
    }
    
    while(!st.empty()){
      result=sub;
      goto RETURNPOINT;
    }
    
    return sub;
  }

  void bfs() {
    int k = 0;
    queue<int> q({ 0 });
    while (!q.empty()) {
      int h = q.front(); q.pop();
      for (int i = h; i != -1; i = heavy[i]) {
	vid[i] = k++;
	inv[vid[i]] = i;
	head[i] = h;
	for (int j : g[i]) if (j != parent[i] && j != heavy[i]) q.push(j);
      }
    }
  }
  
  // for_each(vertex)
  // [l,r] <- attention!!
  void for_each(int u, int v, const function<void(int, int)>& f) {
    if (vid[u] > vid[v]) swap(u, v);
    f(max(vid[head[v]], vid[u]), vid[v]);
    if (head[u] != head[v]) for_each(u, parent[head[v]], f);
  }
  
  // for_each(edge)
  // [l,r] <- attention!!
  void for_each_edge(int u, int v, const function<void(int, int)>& f) {
    if (vid[u] > vid[v]) swap(u, v);
    if (head[u] != head[v]){
      f(vid[head[v]], vid[v]);
      for_each_edge(u, parent[head[v]], f);
    }else{
      if(u!=v) f(vid[u]+1,vid[v]);
    }
  }

  int lca(int u,int v){
    if(vid[u]>vid[v]) swap(u,v);
    if(head[u]==head[v]) return u;
    return lca(u,parent[head[v]]);
  }

  int distance(int u,int v){
    return depth[u]+depth[v]-2*depth[lca(u,v)];
  }
  
};
//END CUT HERE


struct BiconectedGraph{
  typedef pair<int,int> P;
  vector<vector<int> > G,C,T;
  vector<int> ord,low,belong;
  vector<P> B;
  int V;
  BiconectedGraph(){}
  BiconectedGraph(int n){
    G.clear();
    C.clear();
    T.clear();
    G.resize(n);
    C.resize(n);
    T.resize(n);
  }
  bool is_bridge(int u,int v){
    if(ord[u]>ord[v]) swap(u,v);
    return ord[u]<low[v];
  }
  void dfs(int u,int p,int &k){
    ord[u]=low[u]=k;
    ++k;
    for(int v:G[u]){
      if(v==p) continue;
      if(ord[v]>=0){
	low[u]=min(low[u],ord[v]);
      }else{
	dfs(v,u,k);
	low[u]=min(low[u],low[v]);
      }
      if(is_bridge(u,v)) B.push_back(P(u,v));
    }
  }
  void fill_component(int c,int u){
    C[c].push_back(u);
    belong[u]=c;
    for(int v:G[u]){
      if(belong[v]>=0||is_bridge(u,v)) continue;
      fill_component(c,v);
    }
  }
  void add_component(int u,int &k){
    if(belong[u]>=0) return;
    fill_component(k++,u);
  }
  
  void biconnectedgraph(int n){
    int k=0;
    ord.clear();
    ord.resize(n,-1);
    low.clear();
    low.resize(n);
    belong.clear();
    belong.resize(n,-1);
    for(int u=0;u<n;u++){
      if(ord[u]>=0) continue;
      dfs(u,-1,k);
    }
    k=0;
    for(int i=0;i<(int)B.size();i++){
      add_component(B[i].first,k);
      add_component(B[i].second,k);
    }
    add_component(0,k);
    V=k;
    for(int i=0;i<(int)B.size();i++){
      int u=belong[B[i].first],v=belong[B[i].second];
      T[u].push_back(v);
      T[v].push_back(u);
    }
  }
};

struct RMQ{
  int n;
  vector<set<int> > dat;
  RMQ(){}
  RMQ(int n_){init(n_);}
  void init(int n_){
    n=1;
    while(n<n_) n*=2;
    dat.clear();
    dat.resize(2*n-1);
  }
  void update(int k,int a){
    //cout<<k<<" "<<a<<endl;
    k+=n-1;
    assert(!dat[k].count(a));
    dat[k].insert(a);
    while(k>0){
      k=(k-1)/2;
      assert(!dat[k].count(a));
      dat[k].insert(a);
    }
  }
  void remove(int k,int a){
    //cout<<k<<" "<<a<<endl;
    k+=n-1;
    assert(dat[k].count(a));
    dat[k].erase(a);
    while(k>0){
      k=(k-1)/2;
      assert(dat[k].count(a));
      dat[k].erase(a);
    }
  }
  int query(int a,int b,int k,int l,int r){
    if(r<=a||b<=l) return -1;
    if(a<=l&&r<=b){
      if(dat[k].empty()) return -1;
      return *--dat[k].end();
    }
    int vl=query(a,b,k*2+1,l,(l+r)/2);
    int vr=query(a,b,k*2+2,(l+r)/2,r);
    return max(vl,vr);
  }
  int query(int a,int b){
    //cout<<a<<" "<<b<<endl;
    return query(a,b,0,0,n);
  }
};

signed main(){
  ios::sync_with_stdio(0);
  cin.tie(0);
  int n,e,q;
  cin>>n>>e>>q;
  BiconectedGraph big(n);
  for(int i=0;i<e;i++){
    int s,t;
    cin>>s>>t;
    s--;t--;
    //cout<<s<<" "<<t<<endl;
    big.G[s].push_back(t);
    big.G[t].push_back(s);
  }
  big.biconnectedgraph(n);

  int E=0;
  
  int V=big.V;
  HLDecomposition hl(V+1000);
  for(int i=0;i<V;i++)
    for(int j:big.T[i]) hl.add(i,j),E++;
  E/=2;
  assert(V==E+1);
  
  hl.build();
  RMQ rmq(V);
  map<int,int> m;
  int num=0;
  set<int> as;
  for(int i=0;i<q;i++){
    int d;
    cin>>d;
    if(d==1){
      int u,w;
      cin>>u>>w;
      u--;
      u=big.belong[u];
      u=hl.vid[u];
      //cout<<u<<":"<<w<<endl;
      m[w]=u;
      rmq.update(m[w],w);
      num++;
    }
    if(d==2){
      int s,t;
      cin>>s>>t;
      s--;t--;
      s=big.belong[s];
      t=big.belong[t];
      int ans=-1;
      //cout<<s<<"-"<<t<<endl;
      //cout<<" "<<hl.vid[s]<<" "<<hl.vid[t]<<endl;
      hl.for_each(s, t, [&](int l, int r) {
	  ans = max(ans,rmq.query(l, r + 1));
	  //cout<<ans<<endl;
	});
      cout<<ans<<endl;
      if(~ans) rmq.remove(m[ans],ans),num--;
    }
    assert(num==(int)rmq.dat[0].size());
  }
  return 0;
}

// verify yukucoder165 E
