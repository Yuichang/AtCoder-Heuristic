/*
RECRUIT 日本橋ハーフマラソン 2024冬（AtCoder Heuristic Contest 029）
順位 	114th / 2428
*/

#include <bits/stdc++.h>
using namespace std;

// clang-format off
struct Init { Init() { ios::sync_with_stdio(0); cin.tie(0); cout << setprecision(13); } }init;

constexpr char nl = '\n';
#define _overload3(_1, _2, _3, name, ...) name
#define _rep(i, n) repi(i, 0, n)
#define repi(i, a, b) for (lint i = lint(a); i < lint(b); ++i)
#define rep(...) _overload3(__VA_ARGS__, repi, _rep, )(__VA_ARGS__)
#define all(a) (a).begin(), (a).end()

using lint = long long;
using ld = long double;

template <class T> bool chmin(T &a, const T &b) { return a > b ? a = b, true : false; }
template <class T> bool chmax(T &a, const T &b) { return a < b ? a = b, true : false; }

const int inf = INT_MAX/2;
const lint infl = 1LL<<60;

// clang-format on

lint n, m, k, t, money, L, pre;
struct pla
{
   lint kind{}, power{}, index{};
};

struct pro
{
   lint rest{}, value{}, index{};
};

struct new_p
{
   lint kind{}, power{}, cost{};
};

vector<pla> plan, plan2;
vector<pro> project, project2;
vector<new_p> new_plan;

void input_data()
{
   cin >> n >> m >> k >> t;
   plan.resize(n), project.resize(m), new_plan.resize(k);
   rep(i, n)
   {
      cin >> plan[i].kind >> plan[i].power;
      plan[i].index = i;
   }
   rep(i, m)
   {
      cin >> project[i].rest >> project[i].value;
      project[i].index = i;
   }
}
void input_data2()
{
   rep(i, m)
   {
      cin >> project[i].rest >> project[i].value;
      project[i].index = i;
   }
   cin >> money;
   rep(i, k) cin >> new_plan[i].kind >> new_plan[i].power >> new_plan[i].cost;
}

void decide_plan()
{
   // 増資カードを持っている
   sort(all(plan), [&](const pla &a, const pla &b)
        { return a.kind < b.kind; });
   rep(i, n)
   {
      if (plan.back().kind == 4)
      {
         cout << plan.back().index << ' ' << 0 << endl;
         pre = plan.back().index;
         return;
      }
   }
   // 労働力で降順ソート
   sort(all(plan), [&](const pla &a, const pla &b)
        {
      if(a.power!=b.power)
         return a.power>b.power;
      else 
         return a.kind < b.kind; });

   // 残務量で昇順ソート
   sort(all(project), [&](const pro &a, const pro &b)
        { 
      if(a.rest!=b.rest)
        return a.rest<b.rest;
      else
        return a.value>b.value; });

   // 最後のターンの時、利益を最大化させる
   if (t == 0)
   {
      lint profit = -1, mx_id = -1;
      rep(i, m)
      {
         if (plan[0].power >= project[i].rest)
         {
            if (chmax(profit, project[i].value))
               mx_id = i;
         }
      }
      if (mx_id != -1)
      {
         cout << plan[0].index << ' ' << project[mx_id].index << endl;
         pre = plan[0].index;
         return;
      }
   }

   // キャンセル、業務切り替えカードを持っている
   if (plan.back().kind == 3 || plan.back().kind == 2)
   {
      // コスパで昇順ソート
      sort(all(project), [&](const pro &a, const pro &b)
           { return (ld(a.value) / ld(a.rest) > ld(b.value) / ld(b.rest)); });

      // 一番コスパが悪い数値、index
      ld piv = ld(project.back().value) / ld(project.back().rest);
      lint piv_id = project.back().index;

      if (plan.back().kind == 3)
      {
         // 全てのプロジェクトの効率が1以下であり、5回以内の行動で終わるプロジェクトがない
         if (project[0].value / project[0].rest < 1)
         {
            // プロジェクトを残務量で昇順ソート
            sort(all(project), [&](const pro &a, const pro &b)
                 { return a.rest < b.rest; });

            // 一番残務量が少ないプロジェクトでも5回以上の操作が必要
            // 業務切り替えカードを使う
            if (project[0].rest > powl(2, L) * 4)
            {
               cout << plan.back().index << ' ' << 0 << endl;
               pre = plan.back().index;
               return;
            }
         }
      }

      // 残務量で昇順ソート
      sort(all(project), [&](const pro &a, const pro &b)
           { return a.rest < b.rest; });

      // 一番コスパが悪いプロジェクトのコスパが0.8以下である
      // キャンセルカードを使う
      if (piv <= 0.8)
      {
         rep(i, n) if (plan[i].kind == 2)
         {
            cout << plan[i].index << ' ' << piv_id << endl;
            pre = plan[i].index;
            return;
         }
      }
   }

   // 労働力で降順ソート
   sort(all(plan), [&](const pla &a, const pla &b)
        {
      if(a.power!=b.power)
         return a.power>b.power;
      else 
         return a.kind < b.kind; });

   // 残務量で昇順ソート
   sort(all(project), [&](const pro &a, const pro &b)
        { 
      if(a.rest!=b.rest)
        return a.rest<b.rest;
      else
        return a.value>b.value; });

   // 全体労働で完遂できるプロジェクトがあるなら基本的に早めに使う
   rep(i, n)
   {
      lint mi = infl, min_id = -1, min_power = infl;
      rep(j, m) chmin(mi, project[j].rest);
      if (plan[i].kind == 1 && plan[i].power >= mi)
      {
         if (chmin(min_power, plan[i].power))
            min_id = i;
      }
      // あと5回の最低労働でプロジェクトが終わるならそちらを優先する
      if (project[0].rest <= powl(2, L) * 5 && plan[min_id].power - project[0].rest > powl(2, L) * 5)
      {
         rep(j, n)
         {
            // 最低労働を残務量が一番少ないプロジェクトにする
            if (plan[j].kind == 0 && plan[j].power <= powl(2, L))
            {
               cout << plan[j].index << ' ' << project[0].index << endl;
               pre = plan[j].index;
               return;
            }
         }
      }
      if (min_id != -1)
      {
         cout << plan[min_id].index << ' ' << 0 << endl;
         pre = plan[min_id].index;
         return;
      }
   }

   // 一番労働力が大きいカードが通常労働である
   if (plan[0].kind == 0)
   {
      // 完遂できるプロジェクトがある
      if (plan[0].power >= project[0].rest)
      {
         // overkillしないようにindexを調節
         lint id = 0;
         while (id < m - 1 && plan[0].power >= project[id + 1].rest)
            id++;

         lint id2 = 0;
         while (id2 < n - 1 && plan[id2 + 1].kind == 0 && plan[id2 + 1].power >= project[id].rest)
            id2++;

         // overkillしすぎる場合
         if (project[id].rest <= powl(2, L) * 15 && plan[id2].power - project[id].rest >= powl(2, L) * 10)
         {
            if (id2 < n - 1 && plan[id2 + 1].kind == 0)
            {
               // これでもoverkillの場合、さらに調整
               if (plan[id2 + 1].power - project[id].rest >= powl(2, L) * 10)
               {
                  if (id < m - 1 && ld(project[id + 1].value) / ld(project[id + 1].rest) > ld(project[id].value) / ld(project[id].rest))
                  {
                     cout << plan[id2 + 1].index << ' ' << project[id + 1].index << endl;
                     pre = plan[id2 + 1].index;
                     return;
                  }
               }
               cout << plan[id2 + 1].index << ' ' << project[id].index << endl;
               pre = plan[id2 + 1].index;
               return;
            }
         }
         cout << plan[id2].index << ' ' << project[id].index << endl;
         pre = plan[id2].index;
         return;
      }
      // 一番労働力の大きいカードのpowerが最小値である時
      //
      else if (plan[0].power <= powl(2, L) || t <= 25)
      {
         cout << plan[0].index << ' ' << project[0].index << endl;
         pre = plan[0].index;
         return;
      }
      else // 一番労働力が大きいカードを一番コスパが良いプロジェクトに使う
      {
         sort(all(project), [&](const pro &a, const pro &b)
              { return a.value / a.rest > b.value / b.rest; });

         cout << plan[0].index << ' ' << project[0].index << endl;
         pre = plan[0].index;
         return;
      }
   }
   // 一番労働力の大きいカードが全力労働である
   else if (plan[0].kind == 1)
   {
      // 4回以内の最低労働で完遂するプロジェクトがある
      if (project[0].rest <= powl(2, L) * 4)
      {
         // indexの調節
         lint id = 1;
         while (id < n - 1 && plan[id + 1].kind == 0 && plan[id + 1].power >= project[0].rest)
            id++;
         cout << plan[id].index << ' ' << project[0].index << endl;
         pre = plan[id].index;
         return;
      }
      cout << plan[0].index << ' ' << 0 << endl;
      pre = plan[0].index;
      return;
   }
   else // キャンセル、業務切り替えカードしかない
   {
      // 労働力で降順ソート
      sort(all(plan), [&](const pla &a, const pla &b)
           {
         if(a.power!=b.power)
            return a.power>b.power;
         else 
            return a.kind < b.kind; });

      // コスパで降順ソート
      sort(all(project), [&](const pro &a, const pro &b)
           { return ld(a.value) / ld(a.rest) > ld(b.value) / ld(b.rest); });

      // 一番コスパが悪いプロジェクトを消す
      if (plan[0].kind == 2)
      {
         cout << plan[0].index << ' ' << project.back().index << endl;
         pre = plan[0].index;
      }
      else
      {
         cout << plan[0].index << ' ' << 0 << endl;
         pre = plan[0].index;
      }
      return;
   }
}
void buy_plan()
{
   // 増資カード内で一番安いものを探す
   lint min_id = -1, min_cost = infl;
   rep(i, k)
   {
      if (new_plan[i].kind == 4 && new_plan[i].cost <= 750 * powl(2, L) && money >= new_plan[i].cost && L < 20 && t > 90)
         if (chmin(min_cost, new_plan[i].cost))
            min_id = i;
   }
   // 見つかった
   if (min_id != -1)
   {
      L++;
      cout << min_id << endl;
      rep(i, n)
      {
         if (plan[i].index == pre)
            plan[i].kind = new_plan[min_id].kind, plan[i].power = new_plan[min_id].power;
      }
      return;
   }

   // コスパで昇順ソート
   sort(all(project), [&](const pro &a, const pro &b)
        { return ld(a.value) / ld(a.rest) < ld(b.value) / ld(b.rest); });

   // 一番コスパが良い値、悪い値
   ld piv = ld(project[0].value) / ld(project[0].rest);
   ld piv2 = ld(project.back().value) / ld(project.back().rest);

   // 残務量で昇順ソート
   sort(all(project), [&](const pro &a, const pro &b)
        { return a.rest < b.rest; });

   // 0円のキャンセルカードが売ってる場合、n>=4なら買う
   // プロジェクトの完遂に最速でも5回以上の行動が必要
   // 一番安いキャンセルカードを選ぶようにする

   min_id = -1, min_cost = infl;
   rep(i, k)
   {
      if (new_plan[i].kind == 2 && new_plan[i].cost == 0 && n >= 4)
      {
         cout << i << endl;
         rep(j, n) if (plan[j].index == pre) plan[j].kind = new_plan[i].kind, plan[j].power = new_plan[i].power;
         return;
      }
      // 一番コスパが悪い値が0.8以下で安いキャンセルカードが売ってる
      if (new_plan[i].kind == 2 && new_plan[i].cost <= powl(2, L) * 4 && piv < 0.8 && project[0].rest > powl(2, L) && new_plan[i].cost <= money)
         if (chmin(min_cost, new_plan[i].cost))
            min_id = i;
   }

   // 見つかった
   if (min_id != -1)
   {
      cout << min_id << endl;
      rep(j, n) if (plan[j].index == pre) plan[j].kind = new_plan[min_id].kind, plan[j].power = new_plan[min_id].power;
      return;
   }

   min_id = -1, min_cost = infl;

   // 全てのプロジェクトのコスパが1未満、業務切り替えが売ってる
   rep(i, k)
   {
      if (new_plan[i].kind == 3 && new_plan[i].cost <= powl(2, L) * 5 && piv2 < 1 && project[0].rest > powl(2, L) && new_plan[i].cost <= money)
         if (chmin(min_cost, new_plan[i].cost))
            min_id = i;
   }
   if (min_id != -1)
   {
      cout << min_id << endl;
      rep(j, n) if (plan[j].index == pre) plan[j].kind = new_plan[min_id].kind, plan[j].power = new_plan[min_id].power;
      return;
   }
   lint mx_id = 0, mx_power = 1;

   // 残りターン数が25以下の場合、確実に利益が出ない限りカードを買わない
   if (t <= 25)
   {
      lint min_rest = infl, min_val = infl;
      rep(i, m) if (chmin(min_rest, project[i].rest)) min_val = project[i].value;
      lint mx_power = -1, memo_id = -1;
      lint mi_cost = infl;
      rep(i, k)
      {
         if (new_plan[i].kind == 0 || new_plan[i].kind == 1)
         {
            if (new_plan[i].cost == 0)
            {
               if (chmax(mx_power, new_plan[i].power))
                  memo_id = i;
            }
            else
            {
               // 効率が1を下回る場合は買わない。
               if (ld(new_plan[i].power) / ld(new_plan[i].cost) >= 1)
               {
                  // そのカードを買わなければ完遂できないか、確実に利益が出るかを判定
                  if (new_plan[i].power + powl(2, L) * (t - 1) >= min_rest && money >= new_plan[i].cost && new_plan[i].cost < min_val && powl(2, L) * t < min_rest)
                  {
                     if (chmin(mi_cost, new_plan[i].power))
                        memo_id = i;
                  }
               }
            }
         }
      }
      cout << memo_id << endl;
      rep(i, n) if (plan[i].index == pre) plan[i].kind = new_plan[memo_id].kind, plan[i].power = new_plan[memo_id].power;
      return;
   }
   rep(i, k)
   {
      if (new_plan[i].kind == 0 || new_plan[i].kind == 1)
      {
         if (new_plan[i].cost > money)
            continue;

         if (new_plan[i].kind == 0)
         {
            if (chmax(mx_power, new_plan[i].power - new_plan[i].cost))
               mx_id = i;
         }
         else if (new_plan[i].kind == 1)
         {

            // 全力労働はターン数に応じて重み付けする
            lint weight = powl(2, L) * 7;

            if (t <= 40)
               weight += powl(2, L) * 100;
            else if (t <= 50)
               weight += powl(2, L) * 25;
            else if (t <= 100)
               weight += powl(2, L) * 20;
            else if (t <= 200)
               weight += powl(2, L) * 3;

            if (chmax(mx_power, new_plan[i].power * m - new_plan[i].cost - weight))
               mx_id = i;
         }
      }
   }
   cout << mx_id << endl;
   rep(i, n)
   {
      if (plan[i].index == pre)
         plan[i].kind = new_plan[mx_id].kind, plan[i].power = new_plan[mx_id].power;
   }
   return;
}
int main()
{
   srand(time(NULL));
   input_data();
   for (; t--;)
   {
      decide_plan();
      input_data2();
      buy_plan();
   }
}