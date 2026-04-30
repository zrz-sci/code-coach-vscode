/*
 * LCR 083: 全排列 (同主站 LeetCode 46: Permutations)
 *
 * 【题目本质】
 * 给定一个不含重复数字的整数数组 nums，返回其所有可能的全排列。
 * n 个不同元素有 n! 种排列。
 *
 * 【解法总览】
 * 解法1: 回溯 + used 数组  — O(n*n!) / O(n) — 标记已用元素 ⭐面试首选
 * 解法2: 回溯 + swap       — O(n*n!) / O(n) — 原地交换避免 used 数组
 */

// ============================================================
// 解法1: 回溯 + used 数组 — 标记已用元素 ⭐面试首选
// 时间: O(n * n!)  空间: O(n)
//
// 【思路】
// 回溯法：在决策树上做 DFS。
//   - 每个节点：当前已选择的路径 path
//   - 每条边：选择一个未使用的元素
//   - 叶节点：path.size() == n，收集结果
//
// 三步曲：选择 → 递归 → 撤销选择（回溯）
//
// 决策树 (nums = [1, 2, 3]):
//
//                           []
//                  /         |         \
//                [1]        [2]        [3]
//               / \         / \         / \
//            [1,2] [1,3] [2,1] [2,3] [3,1] [3,2]
//              |     |     |     |     |     |
//           [1,2,3][1,3,2][2,1,3][2,3,1][3,1,2][3,2,1]
//
//   6 个叶节点 = 3! = 6 种全排列
//
// used 数组追踪 (部分):
//
//   backtrack(path=[], used=[F,F,F]):
//     i=0: used=[T,F,F], path=[1]
//       backtrack(path=[1], used=[T,F,F]):
//         i=0: used[0]=T → skip
//         i=1: used=[T,T,F], path=[1,2]
//           backtrack(path=[1,2], used=[T,T,F]):
//             i=0: skip, i=1: skip
//             i=2: used=[T,T,T], path=[1,2,3]
//               → path.size()==3 → 收集 [1,2,3]
//             回溯: used=[T,T,F], path=[1,2]
//           回溯: used=[T,F,F], path=[1]
//         i=2: used=[T,F,T], path=[1,3]
//           → ... → 收集 [1,3,2]
//           回溯: used=[T,F,F], path=[1]
//       回溯: used=[F,F,F], path=[]
//     i=1: used=[F,T,F], path=[2]
//       → ... → 收集 [2,1,3] 和 [2,3,1]
//     i=2: used=[F,F,T], path=[3]
//       → ... → 收集 [3,1,2] 和 [3,2,1]
// ============================================================
class Solution1 {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> res;
        vector<int> path;
        vector<bool> used(nums.size(), false);
        backtrack(nums, used, path, res);
        return res;
    }

    void backtrack(vector<int>& nums, vector<bool>& used,
                   vector<int>& path, vector<vector<int>>& res) {
        // 结束条件: 路径长度等于数组长度 → 找到一个完整排列
        if (path.size() == nums.size()) {
            // C++ 中 push_back 会做深拷贝，不需要手动 copy
            // Java/Python 中需要: res.add(new ArrayList<>(path)) / res.append(path[:])
            res.push_back(path);
            return;
        }

        // 排列问题: 每层从 i=0 开始遍历（不是从 start 开始）
        // 因为 [1,2] 和 [2,1] 是不同的排列
        for (int i = 0; i < (int)nums.size(); i++) {
            // 剪枝: 跳过已经在路径中的元素
            if (used[i]) continue;

            // 做出选择
            used[i] = true;
            path.push_back(nums[i]);

            // 递归: 继续选择下一个元素
            backtrack(nums, used, path, res);

            // 撤销选择 (回溯)
            // 这一步是回溯法的精髓 — 恢复状态，让其他分支也能选择这个元素
            path.pop_back();
            used[i] = false;
        }
    }
};

// ============================================================
// 解法2: 回溯 + swap — 原地交换避免 used 数组
// 时间: O(n * n!)  空间: O(n) (仅递归栈)
//
// 【思路】
// 不使用额外的 used 数组和 path 数组。
// 通过 swap 操作，让 nums 自身充当"已选"和"待选"两个区域:
//   nums[0..idx-1]: 已选择的元素（当前排列的前缀）
//   nums[idx..n-1]: 待选择的元素
//
// 每一层将 nums[i] (i >= idx) 与 nums[idx] 交换:
//   → 相当于"选择 nums[i] 放到位置 idx"
//   → 递归处理 idx+1
//   → 交换回来（回溯）
//
// 注意: 这种方法生成的排列顺序不是字典序。
//
// 执行过程 (nums = [1, 2, 3]):
//
//   backtrack(nums=[1,2,3], idx=0):
//     i=0: swap(nums[0],nums[0]) → [1,2,3]
//       backtrack([1,2,3], idx=1):
//         i=1: swap → [1,2,3] → backtrack idx=2
//           i=2: swap → [1,2,3] → 收集! → swap back
//         i=2: swap(nums[1],nums[2]) → [1,3,2] → backtrack idx=2
//           i=2: [1,3,2] → 收集! → swap back
//         swap back → [1,2,3]
//     swap back → [1,2,3]
//
//     i=1: swap(nums[0],nums[1]) → [2,1,3]
//       backtrack([2,1,3], idx=1):
//         i=1: [2,1,3] → ... → 收集 [2,1,3]
//         i=2: swap → [2,3,1] → 收集 [2,3,1]
//     swap back → [1,2,3]
//
//     i=2: swap(nums[0],nums[2]) → [3,2,1]
//       backtrack([3,2,1], idx=1):
//         i=1: [3,2,1] → ... → 收集 [3,2,1]
//         i=2: swap → [3,1,2] → 收集 [3,1,2]
//     swap back → [1,2,3]
// ============================================================
class Solution2 {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> res;
        backtrack(nums, 0, res);
        return res;
    }

    void backtrack(vector<int>& nums, int idx, vector<vector<int>>& res) {
        // 结束条件: 所有位置都已选定
        if (idx == (int)nums.size()) {
            // nums 本身就是当前完整排列
            res.push_back(nums);
            return;
        }

        // 枚举待选区域 nums[idx..n-1] 中的每个元素
        for (int i = idx; i < (int)nums.size(); i++) {
            // 选择: 把 nums[i] 放到位置 idx
            swap(nums[i], nums[idx]);

            // 递归: 处理下一个位置
            backtrack(nums, idx + 1, res);

            // 撤销选择: 恢复 nums 到交换前的状态
            swap(nums[i], nums[idx]);
        }
    }
};

// ============================================================
// 【解法对比】
//
// | 解法          | 时间      | 额外空间   | 代码清晰度 | 面试推荐 |
// |--------------|-----------|-----------|-----------|---------|
// | used 数组    | O(n*n!)   | O(n) used | 直观       | ⭐⭐⭐  |
// | swap         | O(n*n!)   | 无额外数组 | 稍复杂     | ✓ 进阶  |
//
// 两种方法的输出集合完全相同，但顺序不同:
//   used 数组: 字典序（如果 nums 已排序）
//   swap:      非字典序
//
// 面试策略:
//   1. 先写 used 数组版本（最直观、最不易出错）
//   2. 面试官追问优化空间 → 提到 swap 方法
//   3. 面试官追问有重复元素 → 提到排序 + 剪枝 (LC47)
//   4. 画决策树来解释回溯过程（非常加分）
//
// ============================================================
// 【易错点】
//
// 1. 忘记撤销选择:
//    ✗ 递归后不 pop_back / 不恢复 used[i]
//    ✓ 必须 path.pop_back() + used[i] = false
//    → 否则后续分支会受前一个分支影响
//
// 2. 收集结果时不拷贝 (Java/Python):
//    ✗ Java: res.add(path) → 后续回溯会修改 path
//    ✓ Java: res.add(new ArrayList<>(path))
//    ✗ Python: res.append(path) → 同理
//    ✓ Python: res.append(path[:]) 或 res.append(list(path))
//    C++ 不受影响: push_back 自动做深拷贝
//
// 3. 排列问题误用 start 参数:
//    ✗ for(int i = start; ...) → 会漏掉排列
//    ✓ for(int i = 0; ...) + used[i] 检查
//    排列中 [1,2] 和 [2,1] 不同，所以每层从 0 开始
//    组合中 [1,2] 和 [2,1] 相同，才需要 start 参数
//
// 4. swap 方法中的 i 起始点:
//    ✗ for(int i = 0; ...) → 会重复选择已经选定的位置
//    ✓ for(int i = idx; ...) → 只在待选区域中选择
//
// 5. 时间复杂度误算为 O(n!):
//    ✗ O(n!) — 这只是排列的数量
//    ✓ O(n * n!) — 每个排列需要 O(n) 来复制到结果集
//
// ============================================================
// 【回溯三剑客对比】
//
// |          | 排列 (本题) | 组合 (LC77) | 子集 (LC78) |
// |----------|------------|------------|------------|
// | 结果数量  | n!        | C(n,k)     | 2^n        |
// | 元素顺序  | 有关      | 无关       | 无关        |
// | 遍历起点  | i=0       | i=start    | i=start    |
// | 去重方式  | used 数组  | start 参数  | start 参数  |
// | 收集时机  | 叶节点     | 叶节点     | 每个节点     |
// | 典型题    | LC46      | LC77       | LC78       |
// ============================================================

// ============================================================
// 【面试追问】
//
// Q1: 如果 nums 中有重复元素（LC47），如何修改？
// A1: 先对 nums 排序，然后在回溯中加剪枝条件：
//     if (i > 0 && nums[i] == nums[i-1] && !used[i-1]) continue;
//     含义: 相同值的元素，只让排在前面的先被选，防止生成重复排列。
//     swap 方法也能处理: 用 set 记录当前层已 swap 过的值。
//
// Q2: 如果要求按字典序输出所有排列？
// A2: used 数组版本天然保证字典序（前提是 nums 已排序）。
//     swap 版本不保证字典序。如果面试官要求字典序，
//     还可以用 next_permutation 思路（LC31）逐个生成：
//     从当前排列 O(n) 求下一个排列，循环 n! 次。
//     总复杂度同样 O(n * n!)，但常数更大。
//
// Q3: 如果 n 很大（比如 n=20），n! 太大怎么办？
// A3: 20! > 2*10^18，不可能枚举所有排列。
//     常见变体: 「求第 k 个排列」(LC60)，不需要枚举全部。
//     方法: 康托展开，从高位到低位，用 k / (n-1)! 确定每一位选哪个数。
//     时间 O(n^2)（可用 BIT 优化到 O(n log n)），空间 O(n)。
// ============================================================
