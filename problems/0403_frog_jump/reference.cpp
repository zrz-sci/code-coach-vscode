/*
 * 403. Frog Jump
 *
 * 【题目本质】
 * 给定严格递增的石头位置数组，青蛙从 0 出发第一跳为 1，
 * 上一跳 k 步则下一跳只能 k-1/k/k+1 步，判断能否到达最后石头。
 * 本质是状态为 (位置, 步数) 的 DP/BFS 问题。
 *
 * 【解法总览】
 * Solution1: HashMap DP  — O(n^2) 时间, O(n^2) 空间
 * Solution2: 记忆化搜索   — O(n^2) 时间, O(n^2) 空间
 */

// ===================== Solution 1: HashMap DP =====================
// 思路：dp[pos] = 到达 pos 时所有可能的步数集合，从前往后转移
// 时间：O(n^2) | 空间：O(n^2)
class Solution1 {
public:
    bool canCross(vector<int>& stones) {
        int n = stones.size();
        if (n >= 2 && stones[1] != 1) return false;  // 剪枝

        unordered_map<int, unordered_set<int>> dp;
        for (int s : stones) dp[s] = {};
        dp[0].insert(0);

        for (int s : stones) {
            for (int k : dp[s]) {
                for (int next : {k - 1, k, k + 1}) {
                    if (next > 0 && dp.count(s + next)) {
                        dp[s + next].insert(next);
                    }
                }
            }
        }
        return !dp[stones.back()].empty();
    }
};

// ===================== Solution 2: 记忆化搜索 =====================
// 思路：DFS(idx, k) 表示在第 idx 块石头、上一跳 k 步，能否到达终点
// 时间：O(n^2) | 空间：O(n^2)
class Solution2 {
    unordered_map<int, int> posToIdx;
    unordered_map<long long, bool> memo;

    bool dfs(vector<int>& stones, int idx, int k) {
        if (idx == (int)stones.size() - 1) return true;
        long long key = (long long)idx * 2001 + k;
        if (memo.count(key)) return memo[key];

        for (int next : {k - 1, k, k + 1}) {
            if (next > 0) {
                int target = stones[idx] + next;
                if (posToIdx.count(target)) {
                    if (dfs(stones, posToIdx[target], next))
                        return memo[key] = true;
                }
            }
        }
        return memo[key] = false;
    }

public:
    bool canCross(vector<int>& stones) {
        if (stones.size() >= 2 && stones[1] != 1) return false;

        for (int i = 0; i < (int)stones.size(); i++)
            posToIdx[stones[i]] = i;

        return dfs(stones, 0, 0);
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 特点 |
 * |------|------|------|------|
 * | HashMap DP | O(n^2) | O(n^2) | 迭代，不怕栈溢出 |
 * | 记忆化搜索 | O(n^2) | O(n^2) | 代码自然，自动剪枝 |
 *
 * 【易错点】
 * 1. stone 值最大 2^31-1，不能用数组索引，必须用哈希表
 * 2. 步数 k-1 可能为 0，跳 0 步无意义必须过滤
 * 3. 第一块石头必须在 0，第二块必须在 1，否则直接 false
 * 4. memo key 设计：(idx, k) 对，k 最大约 n，用 idx*2001+k 编码
 *
 * 【面试追问】
 * Q1: 时间复杂度为何是 O(n^2)？=> 每个石头最多 O(n) 种步数到达
 * Q2: 能否用 BFS？=> 可以，状态 (pos, k)，visited 去重
 * Q3: 输出路径？=> DP 中记录前驱，终点回溯
 */
