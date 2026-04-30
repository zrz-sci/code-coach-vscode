/*
 * LeetCode 46: 全排列 (Permutations)
 *
 * 【题目本质】
 * 给定 n 个不重复的数，生成所有可能的排列（共 n! 种）。
 * 经典的回溯法入门题，理解"做选择 → 递归 → 撤销选择"的核心框架。
 *
 * 【解法总览】
 * 解法1: 回溯 + used 数组   — O(n × n!) / O(n)     — 最直觉，面试首选
 * 解法2: 回溯 + swap 原地   — O(n × n!) / O(n)递归栈 — 空间更省，进阶写法
 * 解法3: next_permutation   — O(n × n!) / O(1)额外  — 迭代法，字典序输出
 */

// ============================================================
// 解法1: 回溯 + used 数组 — 面试首选
// 时间: O(n × n!)  空间: O(n)（递归深度 + used数组 + path）
//
// 【思路】
// 把排列想象成"n 个空位依次填数"：
// - 第1个位置有 n 种选择
// - 第2个位置有 n-1 种选择（排除已选的）
// - ...以此类推
//
// 用 used[i] 标记 nums[i] 是否已经被选入当前路径。
// 每层 for 循环从 i=0 开始遍历所有数（排列关心顺序！）
//
// 决策树（以 [1,2,3] 为例）:
//
//                          []
//                /          |          \
//              [1]         [2]         [3]        ← 第1层: 3种选择
//             /   \       /   \       /   \
//          [1,2] [1,3] [2,1] [2,3] [3,1] [3,2]   ← 第2层: 各2种
//            |     |     |     |     |     |
//        [1,2,3][1,3,2][2,1,3][2,3,1][3,1,2][3,2,1]  ← 叶子: 收集结果
//
// 共 3! = 6 个叶子节点 = 6 种排列
// ============================================================
class Solution1 {
public:
    vector<vector<int>> result;
    vector<int> path;

    vector<vector<int>> permute(vector<int>& nums) {
        vector<bool> used(nums.size(), false);
        backtrack(nums, used);
        return result;
    }

    void backtrack(vector<int>& nums, vector<bool>& used) {
        // 终止条件：所有数都选完了
        if (path.size() == nums.size()) {
            result.push_back(path);
            return;
        }

        // 排列：每层从 i=0 开始（不是 start！）
        for (int i = 0; i < (int)nums.size(); i++) {
            if (used[i]) continue;  // 已选的数跳过

            // —— 做选择 ——
            path.push_back(nums[i]);
            used[i] = true;

            // —— 递归：填下一个位置 ——
            backtrack(nums, used);

            // —— 撤销选择 ——
            path.pop_back();
            used[i] = false;
        }
    }
};

// ============================================================
// 解法2: 回溯 + swap 原地交换 — 空间优化
// 时间: O(n × n!)  空间: O(n) 仅递归栈
//
// 【思路】
// 解法1 需要额外的 used[] 和 path，能否省掉？
// 核心观察：把 nums 分成两部分：
//   - nums[0..idx-1]：已经确定的排列前缀
//   - nums[idx..n-1]：还没选的候选池
//
// 每次从候选池中选一个 swap 到 idx 位置，就完成了"做选择"。
// 递归处理 idx+1 后，再 swap 回来，就完成了"撤销选择"。
//
// swap 过程示例（[1,2,3]）:
//
//  idx=0: 数组 [1,2,3]
//    i=0: swap(0,0) → [1,2,3], 递归 idx=1
//      i=1: swap(1,1) → [1,2,3], 递归 idx=2 → 收集 [1,2,3]
//      i=2: swap(1,2) → [1,3,2], 递归 idx=2 → 收集 [1,3,2]
//           swap回来   → [1,2,3]
//    i=1: swap(0,1) → [2,1,3], 递归 idx=1
//      i=1: swap(1,1) → [2,1,3], 递归 idx=2 → 收集 [2,1,3]
//      i=2: swap(1,2) → [2,3,1], 递归 idx=2 → 收集 [2,3,1]
//           swap回来   → [2,1,3]
//    swap回来 → [1,2,3]
//    i=2: swap(0,2) → [3,2,1], 递归 idx=1
//      i=1: swap(1,1) → [3,2,1], 递归 idx=2 → 收集 [3,2,1]
//      i=2: swap(1,2) → [3,1,2], 递归 idx=2 → 收集 [3,1,2]
//           swap回来   → [3,2,1]
//    swap回来 → [1,2,3]
// ============================================================
class Solution2 {
public:
    vector<vector<int>> result;

    vector<vector<int>> permute(vector<int>& nums) {
        backtrack(nums, 0);
        return result;
    }

    void backtrack(vector<int>& nums, int idx) {
        // 终止条件：所有位置都已确定
        if (idx == (int)nums.size()) {
            result.push_back(nums);  // 此时 nums 本身就是一个完整排列
            return;
        }

        // 从 idx 到末尾，每个数都有机会放在 idx 位置
        for (int i = idx; i < (int)nums.size(); i++) {
            swap(nums[idx], nums[i]);   // 做选择：把 nums[i] 放到 idx 位置
            backtrack(nums, idx + 1);   // 递归：确定下一个位置
            swap(nums[idx], nums[i]);   // 撤销选择：恢复原状
        }
    }
};

// ============================================================
// 解法3: next_permutation 迭代法 — 字典序输出
// 时间: O(n × n!)  空间: O(1) 额外空间
//
// 【思路】
// 另一种完全不同的思路：不用递归，而是迭代生成。
// 先排序得到最小排列，然后不断调用 next_permutation
// 直到所有排列生成完毕（回到最小排列时 next_permutation 返回 false）。
//
// 这个方法的优势：输出是字典序的。
// 劣势：面试中可能被要求手写 next_permutation（→ LeetCode 31）。
// ============================================================
class Solution3 {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        sort(nums.begin(), nums.end());  // 从字典序最小的排列开始
        vector<vector<int>> result;
        do {
            result.push_back(nums);
        } while (next_permutation(nums.begin(), nums.end()));
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法     | 核心机制         | 额外空间 | 输出顺序 | 面试推荐 |
// |----------|-----------------|----------|----------|---------|
// | used数组 | 布尔数组标记     | O(n)     | 决策树序 | ⭐首选   |
// | swap原地 | 交换已选/未选区  | O(1)*    | 决策树序 | 追问时用 |
// | 迭代法   | next_permutation | O(1)     | 字典序   | 知道即可 |
// * swap 解法的 O(1) 不计递归栈，递归栈仍为 O(n)
//
// 面试策略：
// - 先写解法1，清晰展示回溯框架
// - 被问"能否更省空间"时，切换到解法2
// - 被问"还有其他方法吗"时，提解法3
//
// 【易错点】
//
// 1. 排列写成了组合：
//    ✗ for (int i = start; ...) — 这是组合的写法，会漏排列
//    ✓ for (int i = 0; ...) + if (used[i]) continue;
//    为什么：排列中 [2,1] 和 [1,2] 是不同的，必须从 0 遍历
//
// 2. 忘记撤销选择：
//    ✗ 只有 path.push_back + used[i]=true，没有对应的 pop + false
//    ✓ 递归后必须成对撤销
//    为什么：不撤销的话，回到上一层时状态是错的，后续选择都会出问题
//
// 3. swap 解法中 i 从 0 开始：
//    ✗ for (int i = 0; ...) — 会把已确定的位置也重新交换
//    ✓ for (int i = idx; ...) — 只在待选区中选择
//
// 4. 终止条件用 > 而不是 ==：
//    ✗ if (path.size() > nums.size()) — 多递归了一层
//    ✓ if (path.size() == nums.size()) — 恰好选满时收集
//
// 【面试追问】
//
// Q1: 排列和组合的回溯有什么本质区别？
// → 排列：每层从 i=0 开始 + used 数组，因为顺序不同算不同结果
//   组合：每层从 start 开始，保证只往后选，避免重复
//
// Q2: 能否不用额外的 used 数组？
// → 解法2 的 swap 方法。用交换把数组分为已选区和候选区，
//   不需要额外标记。递归结束后 swap 回来恢复状态。
//
// Q3: 如果 nums 有重复元素怎么办？（→ LeetCode 47）
// → 在解法1基础上：先排序 nums，在同一层的 for 循环中加：
//   if (i > 0 && nums[i] == nums[i-1] && !used[i-1]) continue;
//   这保证同一层不会选相同的值，避免生成重复排列。
//   代码只多了一行排序 + 一行剪枝条件。
//
// Q4: 时间复杂度为什么是 O(n × n!) 而不是 O(n!)？
// → n! 是排列总数。每个排列生成时需要 O(n) 时间复制到结果中。
//   所以总时间 = n! × O(n) = O(n × n!)。
//   回溯过程本身的工作量也是 O(n × n!)（决策树节点数约为 e × n!）。
// ============================================================
