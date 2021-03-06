#include<bits/stdc++.h>
using namespace std;
using Int = long long;
//BEGIN CUT HERE
struct FullyIndexableDictionary{
  int len,blk;
  vector<int> bit,sum;
  
  FullyIndexableDictionary(){}
  FullyIndexableDictionary(int len)
    :len(len),blk((len+31)>>5),bit(blk,0),sum(blk,0){}
  
  void set(int k){
    bit[k>>5]|=1<<(k&31);
  }

  void build(){
    sum[0]=0;
    for(int i=1;i<blk;i++)
      sum[i]=sum[i-1]+__builtin_popcount(bit[i-1]);
  }

  bool operator[](int k) const{
    return bool((bit[k>>5]>>(k&31))&1);
  }
  
  int rank(int k){
    return sum[k>>5]+__builtin_popcount(bit[k>>5]&((1<<(k&31))-1));
  }
  
  int rank(bool v,int k){
    return (v?rank(k):k-rank(k));
  }

  int select(bool v,int k){
    if(k<0||rank(v,len)<=k) return -1;
    int low=0,high=len;
    while(low+1<high){
      int mid=(low+high)>>1;
      if(rank(v,mid)>=k+1) high=mid;
      else low=mid;
    }
    return high-1;
  }

  int select(bool v,int i,int l){
    return select(v,i+rank(v,l));
  }
};

template<class T,int MAXLOG>
struct WaveletMatrix{
  int len;
  FullyIndexableDictionary mat[MAXLOG];
  int zs[MAXLOG],buff1[MAXLOG],buff2[MAXLOG];

  WaveletMatrix(vector<T> data){
    len=data.size();
    vector<T> l(len),r(len);
    for(int dep=0;dep<MAXLOG;dep++){
      mat[dep]=FullyIndexableDictionary(len+1);
      int p=0,q=0;
      for(int i=0;i<len;i++){
	bool k=(data[i]>>(MAXLOG-(dep+1)))&1;
	if(k) r[q++]=data[i],mat[dep].set(i);
	else  l[p++]=data[i];
      }
      zs[dep]=p;
      mat[dep].build();
      swap(l,data);
      for(int i=0;i<q;i++) data[p+i]=r[i];
    }
  }
  
  T access(int k){
    T res=0;
    bool bit;
    for(int dep=0;dep<MAXLOG;dep++){
      bit=mat[dep][k];
      res=(res<<1)|bit;
      k=mat[dep].rank(bit,k)+zs[dep]*dep;
    }
    return res;
  }

  // return the number of v in [0,k)
  int rank(T v,int k){
    int l=0,r=k;
    for(int dep=0;dep<MAXLOG;dep++){
      buff1[dep]=l;buff2[dep]=r;
      bool bit=(v>>(MAXLOG-(dep+1)))&1;
      l=mat[dep].rank(bit,l)+zs[dep]*bit;
      r=mat[dep].rank(bit,r)+zs[dep]*bit;
    }
    return r-l;
  }

  // return the position of k-th v
  int select(T v,int k){
    rank(v,len);
    for(int dep=MAXLOG-1;dep>=0;dep--){
      bool bit=(v>>(MAXLOG-(dep+1)))&1;
      k=mat[dep].select(bit,k,buff1[dep]);
      if(k>=buff2[dep]||k<0) return -1;
      k-=buff1[dep];
    }
    return k;
  }

  int select(T v,int k,int l){
    return select(v,k+rank(v,l));
  }

  // return k-th largest value in [l,r)
  T quantile(int l,int r,int k){
    if(r-l<=k||k<0) return -1;
    T res=0;
    for(int dep=0;dep<MAXLOG;dep++){
      int p=mat[dep].rank(1,l);
      int q=mat[dep].rank(1,r);
      if(q-p>k){
	l=p+zs[dep];
	r=q+zs[dep];
	res|=(1<<(MAXLOG-(dep+1)));
      }else{
	k-=(q-p);
	l-=p;
	r-=q;
      }
    }
    return res;
  }
  
};

//END CUT HERE
signed main(){
  int n,q;
  while(scanf("%d %d",&n,&q)!=EOF){
    vector<int> v(n);
    for(int i=0;i<n;i++) scanf("%d",&v[i]);
    WaveletMatrix<int,32> wm(v);
    for(int i=0;i<q;i++){
      int l,r,k;
      scanf("%d %d %d",&l,&r,&k);
      l--;
      printf("%d\n",wm.quantile(l,r,(r-l)-k));
    }
  }
  return 0;
}

/*
  verified on 2017/11/06
  http://rhodon.u-aizu.ac.jp:8080/arena/room.jsp?id=3765
*/
