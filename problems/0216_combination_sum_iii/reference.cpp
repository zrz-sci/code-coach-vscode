/*
 * LeetCode 216: 组合总和 III (Combination Sum III)
 *
 * 【题目本质】
 * 从 {1,2,...,9} 中选恰好 k 个不重复的数，使和为 n。
 * 经典的"组合 + 约束条件"回溯问题。
 *
 * 【解法总览】
 * 解法1: 回溯（基础版）     — O(C(9,k)) / O(k) — 最直觉
 * 解法2: 回溯（剪枝优化）   — O(C(9,k)) / O(k) — 面试首选
 * 解法3: 位掩码枚举         — O(512×9) / O(k)  — 加分项
 *
 * 【与系列题的关系】
 * - LeetCode 77: 选 k 个数（无和限制）→ 本题多了 sum==n 的约束
 * - LeetCode 39: 可重复使用 → 本题改为 i+1（不可重复）
 * - LeetCode 40: 候选集有重复 → 本题候选 1~9 无重复，省去去重逻辑
 */

// ============================================================
// 解法1: 回溯（基础版） — 标准组合回溯模板
// 时间: O(C(9,k))  空间: O(k)
//
// 【思路】
// 从 1 到 9 依次考虑每个数字"选"或"不选"。
// 用 start 参数保证只向后选，避免产生重复组合。
// 选够 k 个时检查和是否为 n。
//
// 【决策树】以 k=3, n=7 为例:
//                          []  (sum=0, cnt=0)
//                /      |      \       \    ...
//              [1]     [2]     [3]    [4]
//            / | \     / \      |
//        [1,2][1,3].. [2,3][2,4] [3,4]
//        / \    |       |
//    [1,2,3][1,2,4] [1,3,5] [2,3,4]
//     sum=6  sum=7✓  sum=9✗  sum=9✗
// ============================================================
class Solution1 {
public:
    vector<vector<int>> combinationSum3(int k, int n) {
        vector<vector<int>> result;
        vector<int> path;
        backtrack(result, path, k, n, 1, 0);
        return result;
    }

private:
    void backtrack(vector<vector<int>>& result, vector<int>& path,
                   int k, int n, int start, int currentSum) {
        // 终止条件：已选够 k 个数
        if ((int)path.size() == k) {
            if (currentSum == n) {
                result.push_back(path);
            }
            return; // 无论是否满足，都不再继续选
        }

        // 从 start 到 9，依次尝试
        for (int i = start; i <= 9; i++) {
            path.push_back(i);              // 做选择
            backtrack(result, path, k, n,
                      i + 1,                // 下次从 i+1 开始，保证不重复使用
                      currentSum + i);      // 累加当前选的数
            path.pop_back();                // 撤销选择（回溯）
        }
    }
};

// ============================================================
// 解法2: 回溯（剪枝优化） — 面试首选
// 时间: O(C(9,k))  空间: O(k)
//
// 【思路】
// 在解法1的基础上加两个剪枝，大幅减少无效搜索：
//
// 剪枝1 — 个数剪枝:
//   还需要选 need = k - path.size() 个数，
//   从 i 到 9 至少要有 need 个数可选，
//   所以 i 最大为 9 - need + 1。
//   例如还需要3个数，i 最大为7（可选7,8,9）。
//
// 剪枝2 — 和剪枝:
//   如果 currentSum + i > n，由于 i 递增，
//   后面的 i 只会更大，直接 break 终止循环。
//
// 【剪枝效果示意】k=3, n=7:
//   不剪枝: 探索 C(9,3)=84 个组合
//   剪枝后: 只需探索约 10 个分支
//   (例如选了[1,2]后，i=5 时 1+2+5=8>7，直接break)
// ============================================================
class Solution2 {
public:
    vector<vector<int>> combinationSum3(int k, int n) {
        vector<vector<int>> result;
        vector<int> path;
        backtrack(result, path, k, n, 1, 0);
        return result;
    }

private:
    void backtrack(vector<vector<int>>& result, vector<int>& path,
                   int k, int n, int start, int currentSum) {
        if ((int)path.size() == k) {
            if (currentSum == n) {
                result.push_back(path);
            }
            return;
        }

        int need = k - (int)path.size(); // 还需要选几个数

        // 个数剪枝：i 最大为 9 - need + 1
        for (int i = start; i <= 9 - need + 1; i++) {
            // 和剪枝：currentSum + i > n → 后面更大，不可能凑出 n
            if (currentSum + i > n) break;

            path.push_back(i);
            backtrack(result, path, k, n, i + 1, currentSum + i);
            path.pop_back();
        }
    }
};

// ============================================================
// 解法3: 位掩码枚举 — 利用候选集极小的特性
// 时间: O(2^9 × 9) = O(4608)  空间: O(k)
//
// 【思路】
// 候选集只有 {1,...,9}，共 9 个元素。
// 用一个 9 位二进制数 mask 的每一位表示"是否选了该数字"。
// 第 0 位 → 数字 1，第 1 位 → 数字 2，...，第 8 位 → 数字 9
//
// 遍历 mask 从 0 到 511：
//   - popcount(mask) == k → 恰好选了 k 个
//   - 对应数字之和 == n → 满足和的要求
//
// 【示例】k=3, n=7
// mask = 0b000001011 = 11
//   第0位=1(选1), 第1位=1(选2), 第3位=1(选4)
//   popcount=3 ✓, sum=1+2+4=7 ✓ → 加入结果
// ============================================================
class Solution3 {
public:
    vector<vector<int>> combinationSum3(int k, int n) {
        vector<vector<int>> result;

        // 枚举所有 2^9 = 512 个子集
        for (int mask = 0; mask < (1 << 9); mask++) {
            // 快速检查：选的个数必须恰好为 k
            if (__builtin_popcount(mask) != k) continue;

            int sum = 0;
            vector<int> combo;
            for (int bit = 0; bit < 9; bit++) {
                if (mask & (1 << bit)) {
                    int num = bit + 1;  // 第 bit 位对应数字 bit+1
                    sum += num;
                    combo.push_back(num);
                }
            }

            if (sum == n) {
                result.push_back(combo);
            }
        }

        return result;
    }
};

// ============================================================
// 【解法对比】
//
// 解法1 vs 解法2:
//   - 代码几乎相同，解法2 多了两行剪枝条件
//   - 剪枝不影响正确性，只减少搜索量
//   - 面试中写出解法1后，主动加剪枝是加分项
//
// 解法2 vs 解法3:
//   - 回溯法是通用框架，候选集变大也能用
//   - 位掩码只适用于候选集 ≤ 20 左右的情况
//   - 面试中以回溯为主，位掩码作为补充提一嘴
//
// 【易错点】
//
// 1. 和剪枝用 continue 而不是 break:
//    ✗ if (currentSum + i > n) continue;  // 跳过但继续看更大的数
//    ✓ if (currentSum + i > n) break;     // i 递增，后面不可能更小
//    break 是因为候选数字 1~9 是递增的，continue 浪费时间。
//
// 2. 递归传 start+1 而不是 i+1:
//    ✗ backtrack(..., start + 1, ...);  // 例如选了3，下次从2开始？不对
//    ✓ backtrack(..., i + 1, ...);      // 选了3，下次从4开始
//    start 是循环开始值，i 是当前实际选的数字。
//
// 3. 个数剪枝上界算错:
//    ✗ i <= 9 - need;       // 少了 +1，例如 need=1 时 i 只能到 8
//    ✓ i <= 9 - need + 1;   // need=1 时 i 可以到 9
//    因为 i 本身就算一个，所以要 +1。
//
// 4. 忘记在 path.size()==k 时 return:
//    如果不 return，循环继续向 path 添加元素，
//    导致结果中出现长度 > k 的组合。
//
// 【面试追问 — 递进链】
//
// Q1: 为什么用 start 而不是 visited 数组去重？
// A1: 组合问题只关心"选了哪些元素"，不关心顺序。
//     start 保证只向后选，天然不会产生 [2,1,4] 这种和 [1,2,4] 重复的结果。
//     排列问题（如LC46）需要 visited，因为 [1,2] 和 [2,1] 是不同排列。
//
// Q2: 剪枝有几种？分别剪的是什么？
// A2: (1) 个数剪枝：剩余可选数不够 → 修改循环上界
//     (2) 和剪枝：当前和已超标 → break 终止循环
//     两种剪枝都利用了 1~9 递增的性质。
//
// Q3: 如果候选数字改为任意无重复正整数数组怎么办？
// A3: 先排序，然后将 "for i in [start,9]" 改为 "for i in [start, nums.size())"，
//     累加 nums[i] 而不是 i。排序保证和剪枝（break）仍然有效。
//
// Q4: 如果每个数字可以重复使用呢？
// A4: 递归时传 i 而不是 i+1，这就是 LeetCode 39 的做法。
//     同时需要加和剪枝防止无限递归。
// ============================================================
