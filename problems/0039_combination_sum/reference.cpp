/*
 * LeetCode 39: 组合总和 (Combination Sum)
 *
 * 【题目本质】
 * 从无重复元素的数组中，允许每个元素无限次使用，找出所有和为 target 的组合。
 * 本质是"完全背包"的方案枚举版，用回溯穷举所有可能。
 *
 * 【解法总览】
 * 解法1: 回溯（无排序基础版）     — O(n^(T/M)) / O(T/M) — 最直觉
 * 解法2: 回溯 + 排序剪枝          — O(n^(T/M)) / O(T/M) — 面试首选 ⭐
 * 解法3: 选/不选 视角的回溯        — O(n^(T/M)) / O(T/M) — 背包思维
 */

// ============================================================
// 解法1: 回溯（无排序基础版）
// 时间: O(n^(T/M))  空间: O(T/M) 递归深度
//
// 【思路】
// 要列举所有组合 → 回溯穷举。
// 关键问题：如何避免重复？（如 [2,3] 和 [3,2] 是同一组合）
// 方法：用 start 参数控制"只能从当前位置往后选"，不回头。
// 
// 决策树 (candidates=[2,3,6,7], target=7):
//
//                           target=7
//                  /        |        \         \
//              选2(t=5)   选3(t=4)  选6(t=1)  选7(t=0)✓
//             /   |   \      |   \      ✗
//          选2   选3  选6  选3  选6
//         (t=3) (t=2)(t<0)(t=1)(t<0)
//         / \     |        ✗
//       选2 选3  选2
//      (t=1)(t=0)✓(t=0)✓
//       |
//      选2
//     (t<0)✗
//
// 结果: [2,2,3], [7]
// ============================================================
class Solution1 {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<vector<int>> result;
        vector<int> path;
        backtrack(candidates, target, 0, path, result);
        return result;
    }

private:
    void backtrack(vector<int>& candidates, int remaining, int start,
                   vector<int>& path, vector<vector<int>>& result) {
        // 终止条件：剩余为0，说明凑够了
        if (remaining == 0) {
            result.push_back(path);
            return;
        }

        // 从 start 开始遍历，避免回头产生重复组合
        for (int i = start; i < (int)candidates.size(); i++) {
            // 跳过太大的数（但不能 break，因为没排序，后面可能有更小的数）
            if (candidates[i] > remaining) continue;

            path.push_back(candidates[i]);            // 做选择
            // 传 i（不是 i+1）：同一个数可以重复使用
            backtrack(candidates, remaining - candidates[i], i, path, result);
            path.pop_back();                           // 撤销选择
        }
    }
};

// ============================================================
// 解法2: 回溯 + 排序剪枝（面试首选 ⭐）
// 时间: O(n^(T/M))  空间: O(T/M) 递归深度
//
// 【思路】
// 解法1的问题：没排序，某个数太大只能 continue 跳过，后面还得继续检查。
// 优化：先排序，一旦 candidates[i] > remaining，后面所有数都更大，
//       直接 break 终止循环 → 砍掉整棵无效子树。
//
// 排序后的剪枝效果对比:
//   未排序 [7,6,3,2], target=4:
//     i=0: 7>4 → continue; i=1: 6>4 → continue; i=2: 选3...; i=3: 选2...
//     每次都要遍历到后面
//
//   排序后 [2,3,6,7], target=4:
//     i=0: 选2...; i=1: 选3...; i=2: 6>4 → break!  直接停止，省掉检查7
// ============================================================
class Solution2 {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<vector<int>> result;
        vector<int> path;
        sort(candidates.begin(), candidates.end()); // 排序，为 break 剪枝做准备
        backtrack(candidates, target, 0, path, result);
        return result;
    }

private:
    void backtrack(vector<int>& candidates, int remaining, int start,
                   vector<int>& path, vector<vector<int>>& result) {
        if (remaining == 0) {
            result.push_back(path);
            return;
        }

        for (int i = start; i < (int)candidates.size(); i++) {
            // 核心剪枝：排序后，当前数已超 remaining，后面更大的全部不可能
            if (candidates[i] > remaining) break;  // break 而非 continue！

            path.push_back(candidates[i]);
            // 传 i：允许重复选取同一个数
            backtrack(candidates, remaining - candidates[i], i, path, result);
            path.pop_back();
        }
    }
};

// ============================================================
// 解法3: "选/不选" 视角的回溯
// 时间: O(n^(T/M))  空间: O(T/M) 递归深度
//
// 【思路】
// 换一种角度：对于 candidates[idx]，做二叉决策：
//   分支1: 选它（remaining 减少，idx 不变，因为还可以再选）
//   分支2: 不选它（remaining 不变，idx+1 跳到下一个候选）
//
// 这种写法与完全背包 DP 的状态转移完全对应：
//   dp[i][j] = dp[i][j - candidates[i]]  (选)
//            + dp[i+1][j]                 (不选)
//
// 二叉决策树 (candidates=[2,3], target=5):
//
//                    idx=0, t=5
//                   /          \
//            选2(idx=0,t=3)   不选2(idx=1,t=5)
//            /        \         /          \
//     选2(idx=0,t=1) 不选2   选3(idx=1,t=2) 不选3
//       /     \     (idx=1,t=3)  /    \      (idx=2,t=5)→越界
//    选2(t<0) 不选2  ...    选3(t<0) 不选3
//      ✗    (idx=1,t=1)              (idx=2)→越界
//           ...
// ============================================================
class Solution3 {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<vector<int>> result;
        vector<int> path;
        sort(candidates.begin(), candidates.end()); // 排序用于剪枝
        dfs(candidates, target, 0, path, result);
        return result;
    }

private:
    void dfs(vector<int>& candidates, int remaining, int idx,
             vector<int>& path, vector<vector<int>>& result) {
        if (remaining == 0) {
            result.push_back(path);
            return;
        }
        // 越界或当前最小可选数已超 remaining → 无法继续
        if (idx >= (int)candidates.size() || candidates[idx] > remaining) return;

        // 分支1: 选 candidates[idx]，idx 不变（允许重复选）
        path.push_back(candidates[idx]);
        dfs(candidates, remaining - candidates[idx], idx, path, result);
        path.pop_back();

        // 分支2: 不选 candidates[idx]，跳到下一个候选
        dfs(candidates, remaining, idx + 1, path, result);
    }
};

// ============================================================
// 【解法对比】
//
// | 维度           | 解法1(无排序)   | 解法2(排序剪枝)⭐ | 解法3(选/不选)  |
// |----------------|-----------------|---------------------|-----------------|
// | 遍历策略       | 枚举选哪个      | 枚举选哪个          | 二叉决策        |
// | 排序           | 不需要          | 需要                | 需要            |
// | 剪枝           | continue(弱)    | break(强)           | return(强)      |
// | 代码风格       | 标准回溯        | 标准回溯+剪枝       | 类似背包递归    |
// | 面试推荐       | 初始版本        | ★首选★              | 追问时展示      |
//
// 解法2 是面试标准答案：代码简洁，剪枝效果好。
// 解法3 的价值在于：它与完全背包 DP 的递归形式完全对应，
// 如果面试官追问"这跟背包有什么关系"，可以自然过渡。
//
// ============================================================
// 【易错点】
//
// 1. 递归传 i 还是 i+1:
//    ✗ backtrack(candidates, remaining - candidates[i], i+1, path, result);
//      → 每个数只能用一次，变成了 LeetCode 40 的逻辑
//    ✓ backtrack(candidates, remaining - candidates[i], i, path, result);
//      → 允许重复选取同一个数
//
// 2. 排序后用 break 还是 continue:
//    ✗ if (candidates[i] > remaining) continue; // 排序后后面更大，continue 无意义
//    ✓ if (candidates[i] > remaining) break;    // 直接终止，砍掉所有无效子树
//
// 3. 没排序就用 break:
//    ✗ 没排序 + break → 后面可能有更小的有效数字被跳过，导致漏解
//    ✓ 排序是 break 剪枝的前提条件
//
// 4. pop_back 遗漏:
//    ✗ path.push_back(candidates[i]); 之后没有对应的 path.pop_back();
//    ✓ 回溯核心：push_back 和 pop_back 必须成对出现
//
// 5. remaining 判断位置:
//    ✗ 在 for 循环外面判断 if (remaining < 0) return; 
//       → 能 work 但多了一层无效递归
//    ✓ 在 for 循环里用 if (candidates[i] > remaining) break; 
//       → 在选择之前就过滤，更高效
//
// ============================================================
// 【面试追问】
//
// Q1: "时间复杂度怎么分析？"
//   → 决策树深度最多 T/M（T=target，M=最小候选数），每层最多 n 个分支，
//     上界 O(n^(T/M))。但因剪枝，实际远小于此。
//
// Q2: "如果每个数字只能使用一次？"（→ LeetCode 40）
//   → 两处改动：
//     1) 递归传 i+1 而非 i
//     2) 因为 40 题 candidates 有重复元素，同一层跳过相同值：
//        if (i > start && candidates[i] == candidates[i-1]) continue;
//
// Q3: "如果只求方案数，不要求列举？"（→ LeetCode 377/518）
//   → 用完全背包 DP：
//     dp[0] = 1;
//     for (int c : candidates)
//       for (int j = c; j <= target; j++)
//         dp[j] += dp[j - c];
//     注意：518(组合)是先遍历物品再遍历容量，377(排列)反过来。
//
// Q4: "如果 candidates 很大但 target 很小？"
//   → 排序后只保留 ≤ target 的候选数。排序后的 break 剪枝效果很好，
//     大量候选数会被直接跳过。
//
// 【相关题型】
// - LeetCode 40 组合总和 II: 每个元素只能用一次，有重复元素
//   → 本题代码改 i→i+1，加同层去重
// - LeetCode 216 组合总和 III: 1-9中选k个和为n
//   → 本题框架 + 个数限制
// - LeetCode 377 组合总和 IV: 求排列数（不同顺序算不同方案）
//   → DP，外层遍历容量内层遍历物品
// - LeetCode 518 零钱兑换 II: 求组合数
//   → DP，外层遍历物品内层遍历容量，与本题的 DP 视角对应
// - LeetCode 77 组合: 基础组合枚举
//   → 复用"从 start 开始"的去重框架
// ============================================================
