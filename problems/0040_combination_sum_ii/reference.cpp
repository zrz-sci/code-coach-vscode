/*
 * LeetCode 40: 组合总和 II (Combination Sum II)
 *
 * 【题目本质】
 * 从含重复元素的数组中，选若干元素（每个最多一次），使和等于 target。
 * 结果不能有重复组合。核心难点是 **去重**。
 *
 * 【与 39 题的区别】
 * 39 题：无重复元素，每个可用无限次 → 递归传 i，无需去重
 * 40 题：有重复元素，每个只能用一次 → 递归传 i+1，需要去重
 *        代码改动：① backtrack(..., i+1, ...) ② 增加同层去重判断
 *
 * 【解法总览】
 * 解法1: 排序 + 回溯 + 同层去重 — O(2^n) / O(n) — ⭐面试首选
 * 解法2: 计数法回溯           — O(2^n) / O(n) — 去重思路更直观
 */

// ============================================================
// 解法1: 排序 + 回溯 + 同层去重 — ⭐面试首选
// 时间: O(2^n)  空间: O(n) 递归栈深度
//
// 【思路】
// 这道题和 39 题框架一样：回溯选数 + 剪枝。
// 但 candidates 有重复元素，不去重会产生重复组合。
//
// 去重关键：排序后，在同一递归层中，如果 candidates[i] == candidates[i-1]
// 且 i > start，说明前面那个相同的数已经作为本层选择探索过了，跳过。
//
// 决策树 (candidates=[1,1,2], target=3):
//
//                          []  remain=3
//                    /          |         \
//                 [1]          [1]✗       [2]
//               (i=0)        (i=1)      (i=2)
//              /    \        跳过!      remain=1
//          [1,1]   [1,2]              无可选→回溯
//         (i=1)   (i=2)
//        remain=1 remain=0✓
//        选2→[1,1,2]
//        remain=-1✗
//
//  [1](i=1) 被跳过，因为 i=1 > start=0 且 candidates[1]==candidates[0]
//  最终结果: [[1,2]]   (如果 target=4 则还有 [1,1,2])
//
// ============================================================
class Solution1 {
public:
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        vector<vector<int>> result;
        vector<int> path;
        // 排序是去重和剪枝的前提
        sort(candidates.begin(), candidates.end());
        backtrack(candidates, target, 0, path, result);
        return result;
    }

private:
    void backtrack(vector<int>& candidates, int remain, int start,
                   vector<int>& path, vector<vector<int>>& result) {
        // 找到一组合法解
        if (remain == 0) {
            result.push_back(path);
            return;
        }

        for (int i = start; i < (int)candidates.size(); i++) {
            // 剪枝：数组已排序，当前数 > remain，后面的更大，不可能凑出 target
            if (candidates[i] > remain) break;

            // 同层去重：i > start 表示不是本层第一个选择
            // 如果和前一个相同，说明前面已经探索过相同数值的分支，跳过
            // 注意：i > start 而不是 i > 0 ！
            //   i > 0 会阻止"不同层选相同值"，如 [1,1,6] 中第二层选第二个1
            //   i > start 只阻止"同层选相同值"，这才是正确的去重
            if (i > start && candidates[i] == candidates[i - 1]) continue;

            path.push_back(candidates[i]);
            // 传 i+1：每个元素只能用一次（39题传 i，可无限使用）
            backtrack(candidates, remain - candidates[i], i + 1, path, result);
            path.pop_back(); // 撤销选择
        }
    }
};

// ============================================================
// 解法2: 计数法回溯 — 去重逻辑更直观
// 时间: O(2^n)  空间: O(n)
//
// 【思路】
// 换一个视角去重：先统计每个数字的出现次数，
// 然后对每个不重复的数字，决定"选几个"（0 到 count 个）。
//
// 这样根本不会产生重复，因为决策维度是"数字种类"而非"下标"。
//
// 决策树 (candidates=[1,1,2], target=3):
// 去重后的数字: [(1,2次), (2,1次)]
//
//                        []  remain=3
//                   /    |    \
//          选0个1   选1个1  选2个1
//           []       [1]    [1,1]
//          /  \     /  \      |
//       选0个2 选1个2  ...    选0个2  选1个2
//        []    [2]         [1,1]   [1,1,2]✗remain=-1
//              remain=1     remain=1
//              不够          不够
//
// 有效路径: [1]→选1个2→[1,2] remain=0 ✓
//
// ============================================================
class Solution2 {
public:
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        // 统计频次，用 map 自动排序
        map<int, int> counter;
        for (int c : candidates) counter[c]++;

        // 转为 (数字, 频次) 列表
        vector<pair<int, int>> nums(counter.begin(), counter.end());

        vector<vector<int>> result;
        vector<int> path;
        backtrack(nums, target, 0, path, result);
        return result;
    }

private:
    void backtrack(vector<pair<int, int>>& nums, int remain, int idx,
                   vector<int>& path, vector<vector<int>>& result) {
        if (remain == 0) {
            result.push_back(path);
            return;
        }
        // 所有数字种类都考虑完了
        if (idx == (int)nums.size()) return;

        int num = nums[idx].first;
        int freq = nums[idx].second;

        // 当前数字最多能选多少个（受频次和剩余目标双重限制）
        int maxPick = min(freq, remain / num);

        // 选 0 个当前数字 → 直接跳到下一种数字
        backtrack(nums, remain, idx + 1, path, result);

        // 选 1, 2, ..., maxPick 个当前数字
        for (int pick = 1; pick <= maxPick; pick++) {
            path.push_back(num);
            backtrack(nums, remain - pick * num, idx + 1, path, result);
        }

        // 撤销：弹出所有加入的 num（最多加了 maxPick 个）
        for (int i = 0; i < maxPick; i++) {
            path.pop_back();
        }
    }
};

// ============================================================
// 【解法对比】
//
// | 维度         | 解法1: 排序+跳过       | 解法2: 计数法          |
// |-------------|----------------------|----------------------|
// | 去重方式     | 同层跳过 candidates[i]==candidates[i-1] | 按种类枚举，天然不重复 |
// | 代码复杂度   | 短，面试首选            | 稍长，逻辑更清晰        |
// | 适用偏好     | 当重复元素不多时         | 当重复元素特别多时       |
// | 决策维度     | 按下标选择              | 按数字种类选择          |
//
// 面试推荐：解法1，代码短，面试官最熟悉的写法。
//
// ============================================================
// 【易错点】
//
// 1. 去重条件 i > start 写成 i > 0：
//    ✗ if (i > 0 && candidates[i] == candidates[i-1]) continue;
//    ✓ if (i > start && candidates[i] == candidates[i-1]) continue;
//    错因: i > 0 会在不同递归层之间去重，导致 [1,1,6] 这种合法解被丢弃。
//    i > start 只在同一层（同一个for循环内）去重，第一个选择(i==start)不跳过。
//
// 2. 递归传 i 而不是 i+1（和 39 题搞混）：
//    ✗ backtrack(candidates, remain - candidates[i], i, path, result);
//    ✓ backtrack(candidates, remain - candidates[i], i + 1, path, result);
//    错因: 传 i 意味着同一元素可重复选择，这是 39 题的语义。
//
// 3. 剪枝用 continue 而不是 break：
//    ✗ if (candidates[i] > remain) continue;
//    ✓ if (candidates[i] > remain) break;
//    错因: 数组已排序，candidates[i] > remain 则 candidates[i+1] 更大，
//    continue 会继续无意义的循环，break 直接终止。
//
// 4. 忘记排序就做去重判断：
//    排序是 candidates[i] == candidates[i-1] 成立的前提。
//    不排序时相同数字不一定相邻，去重条件无法生效。
//
// ============================================================
// 【面试追问】
//
// Q1: 和 39 题的区别是什么？代码改了哪里？
// A1: 两处：① 递归传 i+1（每个元素只用一次）② 增加同层去重逻辑。
//     39 题无重复元素且可无限用，传 i 且不需要去重。
//
// Q2: 去重条件为什么是 i > start？举个反例。
// A2: candidates=[1,1,6], target=8。第一层选 candidates[0]=1，
//     进入第二层 start=1。选 candidates[1]=1 时 i==start=1，
//     不跳过 → 产生 [1,1,6]。如果用 i>0，第二层会跳过，丢失合法解。
//
// Q3: 如果 candidates 极大（比如 10^6 个元素）但值域很小（1~50），
//     哪种解法更好？
// A3: 计数法（解法2）更好。先统计频次 O(n)，然后回溯的搜索空间只取决于
//     不重复数字的种数（最多50种），与原数组大小无关。
//
// Q4: 能不能用 DP 解这道题？
// A4: 可以用类似 0/1 背包的思路求方案数，但要输出具体组合时仍需回溯。
//     如果只求"有多少种组合"，DP 更高效。
//
// ============================================================
