/*
ALGO ARTIS プログラミングコンテスト2024 夏（AtCoder Heuristic Contest 035）
順位 	244th / 1743
*/
#include <bits/stdc++.h>
#pragma GCC target("avx")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

using namespace std;
// clang-format off

struct Init { Init() { ios::sync_with_stdio(0); cin.tie(0); cout << setprecision(13);srand(time(NULL)); } }init;

constexpr char nl = '\n';
#define _overload3(_1, _2, _3, name, ...) name
#define _rep(i, n) repi(i, 0, n)
#define repi(i, a, b) for (lint i = lint(a); i < lint(b); ++i)
#define rep(...) _overload3(__VA_ARGS__, repi, _rep, )(__VA_ARGS__)
#define all(a) (a).begin(), (a).end()
#define rall(x) (x).rbegin(), (x).rend()

#define pb push_back 
#define is insert
#define fi first
#define sc second

using ll = long long;
using lint = long long;

template <class T>
set<T> operator-(set<T> a, set<T> b){set<T> ret;set_symmetric_difference(all(a), all(b), inserter(ret, ret.begin()));return ret;}

const lint dx[8] = {1,0,-1,0,1,-1,-1,1},dy[8]={0,1,0,-1,1,1,-1,-1};
const int inf = INT_MAX/2;
const lint infl = 1LL<<60;

template <class T> bool chmin(T &a, const T &b) { return a > b ? a = b, true : false; }
template <class T> bool chmax(T &a, const T &b) { return a < b ? a = b, true : false; }
// clang-format on

// 時間計測クラス
struct Time_Keeper
{
   clock_t start_time, end_time;
   double limit_time;
   Time_Keeper(double limit_)
   {
      start_time = clock();
      limit_time = limit_;
   };
   double get_now_time()
   {
      clock_t now_time = clock();
      return (double)(now_time - start_time) / CLOCKS_PER_SEC;
   }
   bool is_time_over()
   {
      end_time = clock();
      return (double)(end_time - start_time) / CLOCKS_PER_SEC >= limit_time;
   }
};

int n = 6, m = 15, t = 10;

struct seed
{
   int id;
   vector<int> ab;
};
int im = 2 * 6 * 5;
vector<vector<int>> x(im, vector<int>(m));
vector<int> mx_id(15, 0);
vector<int> mx_v(15, 0);
vector<int> mx_v_id(15, 0);
set<int> unique_seeds;
vector<seed> seeds(im);
vector<vector<int>> ans(n, vector<int>(n, -1));

struct sub_k
{
   int id, uni_cnt, sum;
};
void input_data()
{
   rep(i, im)
   {
      rep(j, m)
      {
         cin >> x[i][j];
      }
   }
}
void solve()
{
   // 最強にするやつを決める。
   rep(i, m)
   {
      int mx{-1};
      rep(j, im)
      {
         if (chmax(mx, x[j][i]))
         {
            mx_id[i] = j;
            mx_v[i] = mx;
            mx_v_id[i] = j;
         }
      }
   }
   int king{}, king_id{}, king_sum{};
   rep(i, im)
   {
      int cnt{}, sum{};
      rep(j, m)
      {
         if (x[i][j] == mx_v[j])
            cnt++, sum += mx_v[j];
      }
      if (king <= cnt)
      {
         if (king < cnt)
         {
            king_id = i;
            king = cnt;
            king_sum = sum;
         }
         else // king == cnt
         {
            if (sum > king_sum)
            {
               king_id = i;
               king = cnt;
               king_sum = sum;
            }
         }
      }
   }

   // 最強になれる可能性がある種達
   rep(i, im)
   {
      rep(j, m)
      {
         if (x[i][j] == mx_v[j])
            unique_seeds.is(i);
      }
   }

   // 種子king_idを最強にしたい。
   // 既にking_idが最強な種のリスト
   set<int> king_mxs;
   rep(i, m)
   {
      if (x[king_id][i] == mx_v[i])
         king_mxs.is(i);
   }
   ans[2][2] = king_id;
   rep(i, im)
   {
      rep(j, m)
      {
         // kingがjをカバーしている場合
         if (king_mxs.contains(j))
            continue;

         if (x[i][j] == mx_v[j])
            unique_seeds.is(i);
      }
   }

   // king_idの周りにそいつらを配置する
   // より強いやつを配置。

   vector<sub_k> sub_king;
   rep(i, im)
   {
      if (i == king_id)
         continue;
      int uni_cnt{}, su{};
      rep(j, m)
      {
         su += x[i][j];
         if (!king_mxs.contains(j) && x[i][j] == mx_v[j])
            uni_cnt++;
      }
      sub_king.pb({(int)i, uni_cnt, su});
   }

   sort(all(sub_king), [&](const sub_k &a, const sub_k &b)
        { return a.uni_cnt != b.uni_cnt ? a.uni_cnt > b.uni_cnt : a.sum > b.sum; });

   int cnt{};
   set<int> use;
   use.is(king_id);

   rep(i, min(4, (int)sub_king.size()))
   {
      ans[2 + dy[cnt]][2 + dx[cnt]] = sub_king[i].id;
      cnt++;
      use.is(sub_king[i].id);
   }

   if (cnt != 4)
   {
      rep(i, m)
      {
         if (cnt == 4)
            break;

         if (king_mxs.contains(i))
            continue;

         if (use.contains(mx_v_id[i]))
            continue;

         // mx_v_id[i]を採用
         ans[2 + dy[cnt]][2 + dx[cnt]] = mx_v_id[i];
         cnt++;
         use.is(mx_v_id[i]);
      }
   }
   // 2番目のkingも同じようにする?

   // index,sumの順位
   vector<pair<lint, lint>> v;
   rep(i, im)
   {
      lint su{};
      rep(j, m)
          su += x[i][j];
      v.pb({su, i});
   }

   // rsort(v);
   sort(all(v), [&](const pair<lint, lint> &a, const pair<lint, lint> &b)
        {
      if(unique_seeds.contains(a.sc)!=unique_seeds.contains(b.sc))
         return unique_seeds.contains(a.sc)>unique_seeds.contains(b.sc);
      else
         return a.fi>b.fi; });
   rep(i, 4)
   {
      rep(j, im)
      {
         int id = v[j].sc;
         if (use.contains(id))
            continue;
         ans[2 + dy[cnt]][2 + dx[cnt]] = id;
         use.is(id);
         cnt++;
         break;
      }
   }
   rep(i, 1, 4)
   {
      rep(j, im)
      {
         int id = v[j].sc;
         if (use.contains(id))
            continue;
         ans[i][4] = id;
         use.is(id);
         break;
      }
   }
   rep(i, 1, 4)
   {
      rep(j, im)
      {
         int id = v[j].sc;
         if (use.contains(id))
            continue;
         ans[4][i] = id;
         use.is(id);
         break;
      }
   }

   // 最終手段
   rep(i, n)
   {
      rep(j, n)
      {
         if (ans[i][j] == -1)
         {
            rep(k, im)
            {
               int id = v[k].sc;
               if (!use.contains(id))
               {
                  use.is(id);
                  ans[i][j] = id;
                  break;
               }
            }
         }
      }
   }
   clock_t start_t = clock();
   for (;;)
   {
      clock_t now_time = clock();
      if ((double)(now_time - start_t) / CLOCKS_PER_SEC >= 0.19)
         break;

      int ii = rand() % 2;
      int jj = rand() % 2;
      int id1 = ans[ii][jj];
      int id2 = ans[ii][jj + 1];
      vector<int> av, bv;
      rep(i, m)
      {
         if (x[id1][i] == mx_v[i])
            av.pb(i);
         if (x[id2][i] == mx_v[i])
            bv.pb(i);
      }
      // ユニークパラメーターがあるのにそれが全く同じ
      if ((av.size() || bv.size()) && av == bv)
      {
         swap(ans[ii][jj], ans[ii][jj + 4]);
      }
      else
      {
         if (av != bv)
         {
            set<int> avs(all(av));
            set<int> bvs(all(bv));
            set<int> ori = avs - bvs;
            int dif = ori.size();
            av.clear(), bv.clear();
            // スワップしたらユニークパラメーターの数が増える場合
            swap(ans[ii][jj], ans[ii][jj + 4]);
            rep(i, m)
            {
               if (x[id1][i] == mx_v[i])
                  av.pb(i);
               if (x[id2][i] == mx_v[i])
                  bv.pb(i);
            }
            avs.clear(), bvs.clear(), ori.clear();
            int new_dif = ori.size();
            if (dif > new_dif)
               swap(ans[ii][jj], ans[ii][jj + 4]);
         }
      }
      // 縦方向に対してもやる
      id1 = ans[ii][jj];
      id2 = ans[ii + 1][jj];
      av.clear(), bv.clear();
      rep(i, m)
      {
         if (x[id1][i] == mx_v[i])
            av.pb(i);
         if (x[id2][i] == mx_v[i])
            bv.pb(i);
      }

      if ((av.size() || bv.size()) && av == bv)
         swap(ans[ii][jj], ans[ii + 4][jj]);
      else
      {
         if (av != bv)
         {
            set<int> avs(all(av));
            set<int> bvs(all(bv));
            set<int> ori = avs - bvs;
            int dif = ori.size();
            av.clear(), bv.clear();
            // スワップしたらユニークパラメーターの数が増える場合
            swap(ans[ii][jj], ans[ii + 4][jj]);
            rep(i, m)
            {
               if (x[id1][i] == mx_v[i])
                  av.pb(i);
               if (x[id2][i] == mx_v[i])
                  bv.pb(i);
            }
            avs.clear(), bvs.clear(), ori.clear();
            int new_dif = ori.size();
            if (dif > new_dif)
               swap(ans[ii][jj], ans[ii + 4][jj]);
         }
      }
   }
}

void out_plant()
{
   rep(i, n)
   {
      rep(j, n) cout << ans[i][j] << ' ';
      cout << endl;
   }
   ans = vector<vector<int>>(n, vector<int>(n, -1));
}
int main()
{
   cin >> n >> m >> t;
   rep(_, t)
   {
      input_data();
      solve();
      out_plant();
   }
}
