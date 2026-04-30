/*
 * 517. Super Washing Machines
 * 难度: Hard | 标签: Greedy, Array
 *
 * 【题目本质】
 * n 台洗衣机排成一行，每步可同时选任意多台各传一件给相邻机器，
 * 求使所有机器衣服数相等的最少步数。
 * 核心：转化为每个位置的"净流量瓶颈" — 前缀和 + 单机输出上限。
 *
 * 【解法总览】
 * Solution1: 贪心 — 前缀和 + 净流量分析 — O(n) 时间, O(1) 空间 ⭐⭐⭐
 */

// ===================== Solution1: 贪心 — 前缀和 =====================
// 思路：
// 1. diff[i] = machines[i] - avg 为每台机器的"盈余"
// 2. prefix = sum(diff[0..i]) 为"前 i+1 台整体的净需求"
//    → |prefix| 是穿过边 (i, i+1) 的最小流量
// 3. 瓶颈 = max(max(|prefix|), max(diff[i] when > 0))
//    - |prefix|: 边的流量瓶颈（可双向并行）
//    - diff[i]>0: 点的输出瓶颈（单机每步只能输出1件）
class Solution1 {
public:
    int findMinMoves(vector<int>& machines) {
        int n = machines.size();
        int sum = 0;
        for (int x : machines) sum += x;
        if (sum % n != 0) return -1;

        int avg = sum / n;
        int res = 0, prefix = 0;
        for (int i = 0; i < n; i++) {
            int diff = machines[i] - avg;
            prefix += diff;
            // 取两个瓶颈的较大值
            res = max(res, max(abs(prefix), diff));
        }
        return res;
    }
};

/*
 * 【解法对比】
 * 本题只有贪心前缀和这一种最优解法，O(n) 时间 O(1) 空间。
 *
 * 【易错点】
 * 1. diff 取 max 时不能取 abs(diff)：缺衣服可以并行接收，不是瓶颈
 * 2. prefix 取 max 时必须取 abs(prefix)：净流量可以向左或向右
 * 3. 忘记判断 sum % n != 0 返回 -1
 *
 * 【面试追问】
 * Q1: 为什么 diff<0 不算瓶颈？→ 接收可并行，输出不可并行
 * Q2: prefix 物理意义？→ 穿过边(i,i+1)的最小净流量
 * Q3: 每步只能选1台？→ 变成 sum(|diff|)/2，完全不同的问题
 */
