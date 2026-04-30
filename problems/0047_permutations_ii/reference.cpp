/*
 * LeetCode 47: 全排列 II (Permutations II)
 *
 * 【题目本质】
 * 给含重复元素的数组生成所有不重复的全排列。
 * 核心是在 46 题全排列的回溯框架上，加上"同层去重"逻辑。
 *
 * 【解法总览】
 * 解法1: 暴力去重 — O(n! × n) / O(n! × n) — 生成所有排列再 set 去重
 * 解法2: 排序 + 回溯剪枝 — O(n! × n) / O(n) — ⭐ 面试首选
 * 解法3: swap + 局部 set 去重 — O(n! × n) / O(n) — 不需排序的变体
 */

// ============================================================
// 解法1: 暴力去重 — 生成所有排列 + set 去重
// 时间: O(n! × n)  空间: O(n! × n)
//
// 【思路】
// 最直觉的方法：完全忽略重复问题，像 46 题一样生成所有排列，
// 最后用 set<vector<int>> 自动去重。
//
// 为什么不好：对于 [1,1,1]，会生成 6 个相同排列然后去到 1 个，
// 浪费了大量计算。但作为起点，让我们理解"重复发生在哪里"。
//
// 决策树（以 [1₁, 1₂, 2] 为例，不去重时）:
//                          []
//                /          |          \
//             [1₁]        [1₂]        [2]
//            /   \        /   \       /   \
//        [1₁,1₂][1₁,2] [1₂,1₁][1₂,2] [2,1₁][2,1₂]
//           |      |       |      |       |      |
//       [1,1,2] [1,2,1] [1,1,2] [1,2,1] [2,1,1] [2,1,1]
//                         ↑ 重复!  ↑ 重复!          ↑ 重复!
//
// 可以看到：同层选了值相同的元素，子树完全重复。
// ============================================================
class Solution1 {
public:
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        set<vector<int>> resultSet;
        vector<int> path;
        vector<bool> used(nums.size(), false);
        backtrack(nums, used, path, resultSet);
        return vector<vector<int>>(resultSet.begin(), resultSet.end());
    }

private:
    void backtrack(vector<int>& nums, vector<bool>& used,
                   vector<int>& path, set<vector<int>>& resultSet) {
        if (path.size() == nums.size()) {
            resultSet.insert(path); // set 自动去重，但插入代价 O(n)
            return;
        }
        for (int i = 0; i < (int)nums.size(); i++) {
            if (used[i]) continue;
            used[i] = true;
            path.push_back(nums[i]);
            backtrack(nums, used, path, resultSet);
            path.pop_back();
            used[i] = false;
        }
    }
};

// ============================================================
// 解法2: 排序 + 回溯剪枝 — ⭐ 面试首选
// 时间: O(n! × n)  空间: O(n) 递归栈 + used 数组
//
// 【思路】
// 从解法1我们知道，重复发生在"决策树同一层选了相同值的元素"。
// 所以：先排序让相同元素相邻，然后在同一层跳过重复值。
//
// 核心去重条件: i > 0 && nums[i] == nums[i-1] && !used[i-1]
//
// 为什么是 !used[i-1]？
//   - used[i-1] = true：前一个相同元素在当前路径中（更深层选的），合法
//   - used[i-1] = false：前一个相同元素在同层刚刚撤销了，说明同层已经
//                        尝试过这个值，再选就重复了 → 跳过！
//
// 决策树（[1₁, 1₂, 2] 排序后，带去重）:
//                          []
//              /            |             \
//           [1₁]          [1₂]✗          [2]
//          /    \           ↑            /   \
//      [1₁,1₂] [1₁,2]   跳过!       [2,1₁] [2,1₂]✗
//         |       |    (同层重复)       |        ↑
//     [1,1,2]  [1,2,1]              [2,1,1]   跳过!
//
// 结果: [1,1,2], [1,2,1], [2,1,1] ✓
//
// 与 46 题的代码差异（仅两处）:
//   差异1: 加 sort(nums.begin(), nums.end());
//   差异2: 加 if (i > 0 && nums[i]==nums[i-1] && !used[i-1]) continue;
// ============================================================
class Solution2 {
public:
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        vector<vector<int>> result;
        vector<int> path;
        vector<bool> used(nums.size(), false);

        // 【差异1】排序：让相同元素相邻，为去重做准备
        sort(nums.begin(), nums.end());

        backtrack(nums, used, path, result);
        return result;
    }

private:
    void backtrack(vector<int>& nums, vector<bool>& used,
                   vector<int>& path, vector<vector<int>>& result) {
        // 终止条件：构造出一个完整排列
        if (path.size() == nums.size()) {
            result.push_back(path);
            return;
        }

        for (int i = 0; i < (int)nums.size(); i++) {
            // 当前元素已在路径中，不能重复使用同一个位置
            if (used[i]) continue;

            // 【差异2】同层去重：
            // 当前值等于前一个值，且前一个值在同层已经回溯了（!used[i-1]）
            // 说明选当前值会产生与之前完全相同的子树 → 跳过
            if (i > 0 && nums[i] == nums[i - 1] && !used[i - 1]) continue;

            // 做选择
            used[i] = true;
            path.push_back(nums[i]);

            backtrack(nums, used, path, result);

            // 撤销选择
            path.pop_back();
            used[i] = false;
        }
    }
};

// ============================================================
// 解法3: swap + 局部 set 去重 — 不需要排序的变体
// 时间: O(n! × n)  空间: O(n) 每层的 set
//
// 【思路】
// 如果不想排序，可以用另一种方式：
// 用 swap 确定每个位置放什么元素（和 46 题 swap 解法相同），
// 在每一层用一个 unordered_set 记录"已经选过的值"，
// 遇到重复值直接跳过。
//
// 优点：不需要排序，不需要 used 数组和 path
// 缺点：每层创建 set 有常数开销，不如解法2直观
//
// 决策树同解法2，但去重通过 set 实现而非排序+条件判断
// ============================================================
class Solution3 {
public:
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        vector<vector<int>> result;
        backtrack(nums, 0, result);
        return result;
    }

private:
    void backtrack(vector<int>& nums, int start, vector<vector<int>>& result) {
        if (start == (int)nums.size()) {
            result.push_back(nums); // nums 本身就是当前排列
            return;
        }

        // 当前层用 set 记录已经尝试过的值
        unordered_set<int> seen;

        for (int i = start; i < (int)nums.size(); i++) {
            // 这个值在当前层已经选过了 → 跳过（避免重复分支）
            if (seen.count(nums[i])) continue;
            seen.insert(nums[i]);

            // 把 nums[i] 交换到 start 位置，确定当前位置的选择
            swap(nums[start], nums[i]);
            backtrack(nums, start + 1, result);
            swap(nums[start], nums[i]); // 撤销
        }
    }
};

// ============================================================
// 【解法对比】
//
// 解法1 vs 解法2:
//   解法1事后去重，构造了大量无效排列再丢弃；
//   解法2在构造时就剪枝，从根源避免重复，效率高得多。
//
// 解法2 vs 解法3:
//   解法2需要排序 + used数组 + 条件判断，是最经典的模板；
//   解法3不排序，用 set 实现同层去重，代码更短但有额外 set 开销。
//   面试中优先写解法2，被追问"不排序怎么办"时给出解法3。
//
// 【易错点】
// 1. 忘记排序：
//    ✗ 直接写 nums[i] == nums[i-1] 但没 sort → 相同值不相邻，漏去重
//    ✓ 第一步 sort(nums.begin(), nums.end())
//
// 2. 去重条件搞混：
//    ✗ if (i > 0 && nums[i] == nums[i-1]) continue;
//       → 缺少 !used[i-1]，会错误跳过路径中合法的重复使用
//       比如 [1,1,2] 中 [1,1,2] 这个排列第二个位置选 1₂ 时，
//       1₁ 已被选(used[0]=true)，不该跳过
//    ✓ if (i > 0 && nums[i] == nums[i-1] && !used[i-1]) continue;
//
// 3. swap 解法混用排序去重：
//    ✗ 先排序再用 swap 解法 + nums[i]==nums[i-1] 判断
//       → swap 会打乱排序顺序，导致判断失效
//    ✓ swap 解法配合 set 去重（解法3），或排序解法配合 used 去重（解法2）
//
// 4. used 数组忘记撤销：
//    ✗ 回溯时只 pop_back 没有 used[i] = false
//    ✓ pop_back 和 used[i] = false 必须成对出现
//
// 【面试追问】
// Q1: 与46题全排列相比改了什么？
//     → 两处改动：① sort ② 一行去重 if。框架完全一样。
//
// Q2: !used[i-1] 和 used[i-1] 都能AC，哪个更好？
//     → !used[i-1] 更好，在浅层就剪枝。
//     used[i-1] 要求重复元素从右到左使用，深层才能剪枝，做了更多无效递归。
//
// Q3: 不排序能去重吗？
//     → 可以，用解法3（swap + 每层 set），或用频次计数法。
//
// Q4: 时间复杂度能否低于 O(n!)?
//     → 不能。最坏情况（无重复时）本身就有 n! 个排列需要输出，
//        输出大小就是 O(n! × n)，这是下界。
// ============================================================
