// =============================================
// LeetCode 573: Squirrel Simulation
// =============================================

// 【题目本质】
// 松鼠从起点出发收集所有坚果放到树下。只有第一个坚果的路径
// 不同（从松鼠位置出发），其余都从树出发来回。
// 关键：选哪个坚果作为第一个，使总距离最小。

// 【解法总览】
// 1. 贪心 O(n) ← 唯一最优解法

// =============================================
// 解法一: 贪心（推荐）
// 基准: 所有坚果来回 = sum(2 * dist(tree, nut_i))
// 选第 i 个坚果为首 → 节省 dist(tree, nut_i) - dist(squirrel, nut_i)
// 选节省量最大的 → 答案 = 基准 - maxSave
// 时间 O(n), 空间 O(1)
// =============================================
class Solution {
public:
    int minDistance(int height, int width, vector<int>& tree, vector<int>& squirrel, vector<vector<int>>& nuts) {
        int totalDist = 0;
        int maxSave = INT_MIN;

        for (auto& nut : nuts) {
            int d_tree = abs(nut[0] - tree[0]) + abs(nut[1] - tree[1]);
            int d_sq   = abs(nut[0] - squirrel[0]) + abs(nut[1] - squirrel[1]);
            totalDist += 2 * d_tree;
            maxSave = max(maxSave, d_tree - d_sq);
        }

        return totalDist - maxSave;
    }
};

// =============================================
// 公式推导:
// 基准总距离 = sum_i { 2 * dist(tree, nut_i) }
//   = 假设松鼠从树出发的总距离
//
// 实际选第 k 个坚果为首:
//   cost_k = dist(squirrel, nut_k) + dist(nut_k, tree)
//          + sum_{i!=k} { 2 * dist(tree, nut_i) }
//
// cost_k = totalDist - 2*dist(tree, nut_k) + dist(squirrel, nut_k) + dist(tree, nut_k)
//        = totalDist - dist(tree, nut_k) + dist(squirrel, nut_k)
//        = totalDist - (dist(tree, nut_k) - dist(squirrel, nut_k))
//
// 要 cost_k 最小 → 最大化 (dist(tree, nut_k) - dist(squirrel, nut_k))
// =============================================

// 【解法对比】
// 只有一种最优解法（贪心），暴力枚举也是 O(n)，但理解贪心的数学本质更重要。

// 【易错点】
// 1. 不是选离松鼠最近的坚果，而是选"节省量"最大的
// 2. maxSave 必须初始化为 INT_MIN（节省量可能全为负）
// 3. height/width 参数不参与计算，只描述网格大小
// 4. 曼哈顿距离: |r1-r2| + |c1-c2|，不是欧几里得距离

// 【面试追问】
// Q1: 为什么贪心正确？→ 只有第一个坚果路径不同，枚举第一个即可，无需排列组合
// Q2: 能否携带多个坚果？→ 变成路径规划/TSP 问题，NP-hard
// Q3: height/width 有用吗？→ 本题不需要，但如果有障碍物则需要 BFS 计算实际距离
