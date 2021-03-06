#include<bits/stdc++.h>
using namespace std;
using Int = long long;
//BEGIN CUT HERE
template<typename T,int X>
struct BinaryTrie{
  struct Node{
    bool f;
    T laz;
    int par,idx,cnt,nxt[2];
    Node(bool f,int par):
      f(f),laz(0),par(par),idx(-1),cnt(0){nxt[0]=nxt[1]=-1;}
  };

  vector<Node> v;
  BinaryTrie(){v.emplace_back(0,-1);}

  inline int count(int x){
    return x<0?0:v[x].cnt;
  }

  inline void eval(int i,int x){
    if((v[x].laz>>i)&1){
      swap(v[x].nxt[0],v[x].nxt[1]);
      v[x].laz^=T(1)<<i;
    }
    for(int k=0;k<2;k++){
      if(!~v[x].nxt[k]) continue;
      v[v[x].nxt[k]].laz^=v[x].laz;
      v[v[x].nxt[k]].f=k;
    }
    v[x].laz=0;
  }
  
  void add(const T b,int x){
    int pos=0;
    for(int i=X-1;i>=0;i--){
      eval(i,pos);
      bool f=(b>>i)&1;
      if(~v[pos].nxt[f]){
	pos=v[pos].nxt[f];
	continue;
      }
      int npos=v.size();
      v[pos].nxt[f]=npos;
      v.emplace_back(f,pos);
      pos=npos;
    }
    if(!~v[pos].idx) v[pos].idx=x;
    v[pos].cnt++;
    for(int i=0;i<X;i++){
      pos=v[pos].par;
      v[pos].cnt=count(v[pos].nxt[0])+count(v[pos].nxt[1]);
    }
  }

  void update(const T b){
    v[0].laz^=b;
  }

  int find(const T b){
    int pos=0;
    for(int i=X-1;i>=0;i--){
      eval(i,pos);
      bool f=(b>>i)&1;
      if(~v[pos].nxt[f]) pos=v[pos].nxt[f];
      else return -1;
    }
    return pos;
  }

  void erase(int pos){
    v[pos].cnt--;
    for(int i=0;i<X;i++){
      pos=v[pos].par;
      v[pos].cnt=count(v[pos].nxt[0])+count(v[pos].nxt[1]);
      for(int k=0;k<2;k++)
	if(!count(v[pos].nxt[k])) v[pos].nxt[k]=-1;
    }
  }
  
  int xmax(const T b){
    int pos=0;
    for(int i=X-1;i>=0;i--){
      eval(i,pos);
      bool f=(~b>>i)&1;
      f^=!~v[pos].nxt[f];
      pos=v[pos].nxt[f];
    }
    return v[pos].idx;
  }

  int xmin(const T b){
    return xmax(~b&((T(1)<<X)-1));
  }
  
};
//END CUT HERE

//INSERT ABOVE HERE
signed JAG2013SUMMERWARMINGUP_F(){
  int n;
  cin>>n;
  vector<int> a(n);
  for(int i=0;i<n;i++) cin>>a[i];
  vector<int> s(n+1,0);
  for(int i=0;i<n;i++) s[i+1]=s[i]^a[i];
  BinaryTrie<int, 30> bt;
  bt.add(0,0);
  int ans=-1,idx=-1,idy=-1;
  for(int i=0;i<n;i++){
    int k=bt.xmax(a[i]);
    int res=s[i+1]^s[k];
    if(ans<res||(ans==res&&idx>k)){
      ans=res;
      idx=k;
      idy=i;
    }
    bt.update(a[i]);
    bt.add(0,i+1);
  }
  cout<<ans<<" "<<idx+1<<" "<<idy+1<<endl;
  return 0;
}
/*
  verified on 2018/03/11
  https://beta.atcoder.jp/contests/jag2013summer-warmingup/tasks/icpc2013summer_warmingUp_f
*/
signed CFR470_C(){
  int n;
  scanf("%d",&n);
  vector<int> a(n),p(n);
  for(Int i=0;i<n;i++) scanf("%d",&a[i]);
  for(Int i=0;i<n;i++) scanf("%d",&p[i]);

  BinaryTrie<int, 30> bt;
  for(int i=0;i<n;i++) bt.add(p[i],i);
  
  for(Int i=0;i<n;i++){
    if(i) printf(" ");
    int k=bt.xmin(a[i]);
    printf("%d",a[i]^p[k]);
    bt.erase(bt.find(p[k]));
  }
  puts("");
  return 0;
}
/*
  verified on 2018/03/11
  http://codeforces.com/contest/947/problem/C
*/

signed main(){
  //JAG2013SUMMERWARMINGUP_F();
  CFR470_C();
  return 0;
}
