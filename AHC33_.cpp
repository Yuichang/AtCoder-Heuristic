/*
トヨタ自動車プログラミングコンテスト2024#5（AtCoder Heuristic Contest 033）
順位 	363rd / 4035
*/

#include <bits/stdc++.h>
#pragma GCC target("avx")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
using namespace std;
// clang-format off
/* #language C++ GCC */
struct Init { Init() { ios::sync_with_stdio(0); cin.tie(0); cout << setprecision(13); } }init;
constexpr char nl = '\n';
#define _overload3(_1, _2, _3, name, ...) name
#define _rep(i, n) repi(i, 0, n)
#define repi(i, a, b) for (int i = int(a); i < int(b); ++i)
#define rep(...) _overload3(__VA_ARGS__, repi, _rep, )(__VA_ARGS__)
#define all(a) (a).begin(), (a).end()
#define pb push_back 
#define is insert
#define fi first
#define sc second
#define co() cout << nl;

using lint = long long;
template<class T,class U> bool is_out(T y,T x,U h,U w){return (y<0||y>=h||x<0||x>=w);}
const int dx[8] = {1,0,-1,0,1,-1,-1,1},dy[8]={0,1,0,-1,1,1,-1,-1};

// clang-format on
int n = 5;
vector<vector<int>> a(n);
vector<vector<pair<int, char>>> g(n, vector<pair<int, char>>(n, {-1, '.'}));
vector<int> need(n);
vector<string> ans(5);
vector<bool> bombed(5, false);
pair<int, int> mode;
set<int> nex;
vector<deque<int>> next_act(5);

// 掴んでいる番号or-1、座標y,座標x
struct cranes
{
   int hold, y, x;
};
vector<cranes> crane(5);
pair<int, int> Q_pos = {0, 1};

struct IO
{
   void input_data()
   {
      cin >> n;
      int e;
      rep(i, n) rep(j, n)
      {
         cin >> e;
         a[i].insert(a[i].begin(), e);
      }
   }
   void out_ans()
   {
      rep(i, n)
      {
         for (auto nx : ans[i])
            cout << nx;
         co();
      }
   }
};
struct Solver
{
   IO io;
   Solver()
   {
      io.input_data();
      initialize_state();
   }
   void initialize_state()
   {
      rep(i, n) need[i] = 5 * i;
      rep(i, n) crane[i] = {-1, i, 0};
      rep(i, n)
      {
         g[i][0].fi = a[i].back();
         if (i == 0)
            g[i][0].sc = 'Q';
         else
            g[i][0].sc = 'q';
      }
      mode = confirm_mode();
      rep(i, n)
      {
         a[i].pop_back();
         nex.insert(5 * i);
      }
   }

   void solve()
   {
      // modeの最小値、何個modeを満たした数があるか
      solv(mode);
      func();
      io.out_ans();
   }

   bool is_in_area4()
   {
      bool res = false;
      rep(i, n) if (crane[i].x == 4) res = true;
      return res;
   }
   // 一旦コンテナを前に出せるだけ出す

   pair<int, int> confirm_mode()
   {
      int mode{};
      for (int i = 4; i >= 0; i--)
      {
         int cnt{};
         rep(j, n)
         {
            if (a[j][i] % 5 == 0)
            {
               cnt++;
            }
         }
         if (cnt)
            return make_pair(mode, cnt);
         mode++;
      }
   }
   void act_crane(char act, int id, int y, int x)
   {
      if (bombed[id])
         return;
      ans[id].pb(act);
      int ny = y, nx = x;
      if (act == 'R')
         nx++;
      if (act == 'L')
         nx--;
      if (act == 'U')
         ny--;
      if (act == 'D')
         ny++;
      if (id == 0)
         Q_pos = {ny, nx};
      switch (act)
      {
      case 'P':
         crane[id].hold = g[y][x].first;
         g[y][x].first = -1;
         if (x == 0 && a[y].size() > 0)
         {
            g[y][x].first = a[y].back();
            a[y].pop_back();
         }
         // 搬入口かつ、まだ搬入される荷物がある
         break;
      case 'Q':
         if (x == 4)
         {
            nex.erase(crane[id].hold);
            g[y][x].first = -1;
            need[y]++;
            if (need[y] >= y * 5 + 5)
               need[y] = -9;
            else
               nex.insert(need[y]);
         }
         else
            g[y][x].first = crane[id].hold;
         crane[id].hold = -1;
         break;
      case 'B':
         bombed[id] = true;
         g[y][x].sc = '.';
         crane[id] = {-1, -1, -1};
         break;
      // 方向 or '.'
      default:
         g[y][x].sc = '.';
         crane[id].y = ny, crane[id].x = nx;
         g[y][x].second = id == 0 ? 'Q' : 'q';
      }
   }
   pair<int, pair<int, int>> search_need()
   {

      queue<pair<int, int>> q;
      q.push(Q_pos);
      vector<vector<int>> seen(n, vector<int>(n, -1));
      seen[Q_pos.fi][Q_pos.sc] = 0;
      while (q.size())
      {
         auto [y, x] = q.front();
         q.pop();
         rep(k, 4)
         {
            int ii = y + dy[k], jj = x + dx[k];
            if (is_out(ii, jj, n, n))
               continue;
            if (seen[ii][jj] != -1)
               continue;
            if (nex.contains(g[ii][jj].fi))
            {
               int piv = g[ii][jj].fi / 5;
               need[piv]++;
               nex.erase(g[ii][jj].fi);

               if (need[piv] >= piv * 5 + 5)
                  need[piv] = -9;
               else
                  nex.insert(need[piv]);
               if (jj == 0)
               {
                  if (a[ii].size())
                  {
                     g[ii][jj].fi = a[ii].back();
                     a[ii].pop_back();
                  }
                  else
                     g[ii][jj].fi = -1;
               }
               else
                  g[ii][jj].fi = -1;

               return {piv, make_pair(ii, jj)};
            }
            seen[ii][jj] = seen[y][x] + 1;
            q.push({ii, jj});
         }
      }
      // return _,make_pair(i,j)
      return {-1, make_pair(-1, -1)};
   }
   // 行きたい場所の座標
   void go_pos(int y, int x, bool op)
   {
      int move_y = Q_pos.fi - y, move_x = Q_pos.sc - x;
      Q_pos = {y, x};
      rep(i, abs(move_y)) { ans[0] += (move_y > 0) ? 'U' : 'D'; }
      rep(i, abs(move_x)) { ans[0] += (move_x > 0) ? 'L' : 'R'; }
      // つかむ
      ans[0] += op ? 'P' : 'Q';
   }
   void move_front()
   {
      lint dx[3] = {1, 0, 0}, dy[3] = {0, 1, -1};
      rep(i, n)
      {
         if (a[i].size() && g[i][0].fi != -1)
         {
            rep(k, 3)
            {
               // debug(g);
               int ii = i + dy[k], jj = 0 + dx[k];
               if (is_out(ii, jj, n, n))
                  continue;
               if (g[ii][jj].fi != -1)
                  continue;

               go_pos(i, 0, true);
               go_pos(ii, jj, false);
               g[ii][jj].fi = g[i][0].fi;
               if (a[i].size())
               {
                  g[i][0].fi = a[i].back();
                  a[i].pop_back();
               }
               else
               {
                  g[i][0].fi = -1;
               }
               return;
            }
         }
      }
   }
   void func()
   {
      rep(_, 300)
      {
         auto get = search_need();
         int id = get.fi, y = get.sc.fi, x = get.sc.sc;
         if (id != -1)
         {
            go_pos(y, x, true);
            go_pos(id, n - 1, false);
         }
         else
         {
            bool jud = true;
            rep(i, n) if (need[i] != -9) jud = false;
            if (!jud)
               move_front();
            else
            {
               io.out_ans();
               exit(0);
            }
         }
      }
   }

   void find_nx_dir()
   {
      vector<int> v;
      rep(i, n)
      {
         if (nex.contains(g[i][0].fi))
            v.push_back(g[i][0].fi);
      }
      sort(v.begin(), v.end());
   }

   void solv(pair<int, int> mode_)
   {
      rep(_, 100)
      {
         rep(i, n)
         {
            if (bombed[i])
               continue;
            auto [hold, y, x] = crane[i];
            if (i == 0 && hold == -1 && count(all(bombed), true) == 4 && hold == -1)
            {
               return;
            } // x座標0
            if (x == 0)
            {
               if (i == 0 && count(all(bombed), true) == 4 && hold == -1)
               {
                  return;
               }
               if (g[y][x + 1].fi != -1)
               {
                  if (i != 0)
                  {
                     act_crane('B', i, y, x);
                     continue;
                  }
                  else // i==0
                  {
                     if (hold == -1 && nex.contains(g[y][x].fi))
                     {
                        act_crane('P', i, y, x);
                        continue;
                     }
                     if (hold != -1 && nex.contains(hold))
                     {
                        act_crane('R', i, y, x);
                        continue;
                     }
                     act_crane('.', i, y, x);

                     if (count(all(bombed), true) == 4)
                        return;
                     continue;
                  }
                  if (count(all(bombed), true) == 4)
                     return;
               }
               // コンテナを持っていない
               if (hold == -1 && g[y][x].fi != -1 && g[y][x + 1].fi == -1)
               {
                  act_crane('P', i, y, x);
               }
               else if (hold != -1) // コンテナを持っている
               {
                  act_crane('R', i, y, x);
               }
               else // コンテナを持ってない、コンテナもない
               {
                  if (i != 0)
                     act_crane('B', i, y, x);
                  else
                  {
                     if (count(all(bombed), true) == 4)
                        return;
                  }
               }
            }
            else if (x == 1)
            {
               if (hold != -1)
               {
                  // 必要なものを保持している
                  if (nex.contains(hold))
                  {
                     if (g[y][x + 1].fi == -1 || i == 0)
                        act_crane('R', i, y, x);
                     else // Rに行けない
                        act_crane('Q', i, y, x);
                  }
                  else // 必要なものを保持していない
                  {
                     if (g[y][x + 1].fi == -1)
                        act_crane('R', i, y, x);
                     else // Rに行けない
                        act_crane('Q', i, y, x);
                  }
               }
               else
                  act_crane('L', i, y, x);
            }
            else if (x == 2)
            {
               if (next_act[i].size())
               {
                  char act = next_act[i].front();
                  next_act[i].pop_front();
                  act_crane(act, i, y, x);
                  continue;
               }
               // コンテナを保持している
               if (hold != -1)
               {
                  if (nex.contains(hold))
                  {
                     if (g[y][x + 1].fi == -1 || i == 0)
                        act_crane('R', i, y, x);
                     else
                        act_crane('Q', i, y, x);
                  }
                  else // 必要なものを保持していない
                  {
                     // その行の次の数がnextである
                     if (nex.contains(g[y][0].fi))
                     {
                        if (i <= 3)
                        {
                           if (g[y + 1][x].fi == -1 && g[y + 1][x].sc == '.')
                           {
                              act_crane('D', i, y, x);
                              next_act[i].push_back('Q');
                              next_act[i].push_back('U');
                              continue;
                           }
                        }
                        else // i==4
                        {
                           if (g[y - 1][x].fi == -1 && g[y - 1][x].sc == '.')
                           {
                              act_crane('U', i, y, x);
                              next_act[i].push_back('Q');
                              next_act[i].push_back('D');
                              continue;
                           }
                        }
                     }

                     if (g[y][x + 1].fi == -1)
                        act_crane('R', i, y, x);
                     else
                        act_crane('Q', i, y, x);
                  }
               }
               else // コンテナを保持してない
                  act_crane('L', i, y, x);
            }
            else if (x == 3)
            {
               // 侵入するかどうか。
               if (hold != -1)
               {
                  if (nex.contains(hold))
                  {
                     if (is_in_area4())
                        act_crane('.', i, y, x);
                     else
                        act_crane('R', i, y, x);
                  }
                  else
                  {
                     act_crane('Q', i, y, x);
                  }
               }
               else // コンテナを保持していない
                  act_crane('L', i, y, x);
            }
            else if (x == 4)
            {
               // 持ってる場合は置くか移動する
               if (hold != -1)
               {
                  // 正しい搬出口
                  if (y == hold / 5)
                     act_crane('Q', i, y, x);
                  else // 移動する
                  {
                     // 下に行く
                     if ((y - (hold / 5)) < 0)
                        act_crane('D', i, y, x);
                     else // 上に行く
                        act_crane('U', i, y, x);
                  }
               }
               else // 持ってない場合、いた場所に帰る
               {
                  if (i == 0 && count(all(bombed), true) == 4)
                     return;
                  if (y == i)
                     act_crane('L', i, y, x);
                  else
                  {
                     if (y - i < 0)
                        act_crane('D', i, y, x);
                     else
                        act_crane('U', i, y, x);
                  }
               }
            }
         }
      }
   }
};
int main()
{
   Solver solver;
   solver.solve();
}