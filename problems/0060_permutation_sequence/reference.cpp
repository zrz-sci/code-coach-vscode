/*
 * LeetCode 60: Permutation Sequence (排列序列)
 *
 * 【题目本质】
 * 给定 n 和 k，直接计算 [1..n] 的全排列中字典序第 k 个排列。
 * 核心：利用阶乘数系统（康托展开逆运算），逐位确定每一位数字。
 *
 * 【解法总览】
 * 解法1: 暴力 next_permutation  — O(n*n!)  — 超时/极慢
 * 解法2: 阶乘数系统(数学直算)  — O(n^2)   — 面试唯一正解 ⭐
 */


// ============================================================
// 解法1: 暴力 next_permutation
// 时间: O(k * n)  最坏 O(n * n!)  空间: O(n)
//
// 【思路】
// 从最小排列 "123...n" 开始，调用 k-1 次 next_permutation。
// 每次 O(n)，总计 O(kn)。
//
// 为什么不推荐？
//   n=9 时 k 最大 362880，O(kn) = O(362880 * 9) ≈ 3M，勉强能过。
//   但这完全是暴力搜索，没有利用数学性质。面试中需要展示 O(n^2) 解法。
//
// 示例: n=3, k=3
//   初始: "123"
//   next_permutation → "132"
//   next_permutation → "213" ← 第3个 ✓
// ============================================================
class Solution1 {
public:
    string getPermutation(int n, int k) {
        string s = "";
        for (int i = 1; i <= n; i++) s += to_string(i);

        // 调用 k-1 次 next_permutation
        for (int i = 1; i < k; i++) {
            next_permutation(s.begin(), s.end());
        }
        return s;
    }
};

// ============================================================
// 解法2: 阶乘数系统（康托展开逆运算）— 面试唯一正解 ⭐
// 时间: O(n^2)  空间: O(n)
//
// 【思路】
// n 个数的全排列共 n! 种。
// 固定第一位后，剩下 (n-1)! 种排列。
// 因此可以通过"除法+取模"逐位确定每一位数字。
//
// 算法步骤：
//   1. 预计算阶乘 factorial[0..n-1]
//   2. 候选数字 nums = [1, 2, ..., n]
//   3. k-- (转为 0-indexed)
//   4. for i = n-1 down to 0:
//        idx = k / factorial[i]      ← 当前位在候选中的下标
//        result += nums[idx]         ← 取出数字
//        nums.erase(idx)             ← 从候选中移除
//        k %= factorial[i]           ← 更新剩余排名
//
// 详细示例: n=4, k=14
//
//   阶乘: [1, 1, 2, 6]  (0!, 1!, 2!, 3!)
//   候选: [1, 2, 3, 4]
//   k = 14 - 1 = 13 (0-indexed)
//
//   第1位 (i=3): idx = 13 / 6 = 2 → 选候选[2]=3, 移除
//                k = 13 % 6 = 1, 候选=[1,2,4]
//
//   第2位 (i=2): idx = 1 / 2 = 0 → 选候选[0]=1, 移除
//                k = 1 % 2 = 1, 候选=[2,4]
//
//   第3位 (i=1): idx = 1 / 1 = 1 → 选候选[1]=4, 移除
//                k = 1 % 1 = 0, 候选=[2]
//
//   第4位 (i=0): idx = 0 / 1 = 0 → 选候选[0]=2, 移除
//                候选=[]
//
//   结果: "3142" ✓
//
// 验证: n=4, k=14 的排列
//   k=1: 1234, k=2: 1243, k=3: 1324, k=4: 1342,
//   k=5: 1423, k=6: 1432, k=7: 2134, k=8: 2143,
//   k=9: 2314, k=10: 2341, k=11: 2413, k=12: 2431,
//   k=13: 3124, k=14: 3142 ✓
//
// 为什么 k 要先减 1？
//   排列编号从 1 开始（k=1 是最小排列），但数组下标从 0 开始。
//   如果不减 1：n=3, k=1 → idx = 1/2 = 0 ✓ (偶然正确)
//              n=3, k=3 → idx = 3/2 = 1 ✗ (应该是 idx=1, 选出 2)
//              实际: k-1=2, idx = 2/2 = 1 ✓
//   不减 1 在边界情况（k是阶乘的倍数时）会出错。
// ============================================================
class Solution2 {
public:
    string getPermutation(int n, int k) {
        // 1. 预计算阶乘: factorial[i] = i!
        //    factorial[0] = 0! = 1
        //    factorial[1] = 1! = 1
        //    factorial[2] = 2! = 2
        //    ...
        vector<int> factorial(n, 1);
        for (int i = 1; i < n; i++) {
            factorial[i] = factorial[i - 1] * i;
        }

        // 2. 候选数字: [1, 2, 3, ..., n]
        vector<int> nums;
        for (int i = 1; i <= n; i++) {
            nums.push_back(i);
        }

        // 3. 转为 0-indexed
        k--;

        // 4. 逐位确定排列的每一位
        string result;
        for (int i = n - 1; i >= 0; i--) {
            // 当前位在候选数组中的下标
            int idx = k / factorial[i];
            // 取出数字追加到结果
            result += to_string(nums[idx]);
            // 从候选中移除已使用的数字
            nums.erase(nums.begin() + idx);
            // 更新剩余排名
            k %= factorial[i];
        }

        return result;
    }
};

// ============================================================
// 解法2b: 阶乘数系统 + 链表（概念优化）
// 时间: O(n^2)  空间: O(n)
//
// 【思路】
// 用 list 代替 vector 避免 erase 的元素移动。
// 但 list 不支持随机访问，advance 仍是 O(n)。
// 总体仍 O(n^2)，常数可能更大（链表 cache 不友好）。
// 面试中提到"可以用 BIT 做到 O(n log n)"即可。
// ============================================================
class Solution2b {
public:
    string getPermutation(int n, int k) {
        vector<int> factorial(n, 1);
        for (int i = 1; i < n; i++) factorial[i] = factorial[i - 1] * i;

        list<int> nums;
        for (int i = 1; i <= n; i++) nums.push_back(i);

        k--;
        string result;
        for (int i = n - 1; i >= 0; i--) {
            int idx = k / factorial[i];
            auto it = nums.begin();
            advance(it, idx);      // O(idx) ≤ O(n)
            result += to_string(*it);
            nums.erase(it);        // O(1) 删除
            k %= factorial[i];
        }
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法            | 时间       | 空间   | 适用场景        |
// |----------------|-----------|--------|----------------|
// | 暴力           | O(n*n!)   | O(n)   | n极小(≤5)       |
// | next_permutation| O(k*n)   | O(n)   | k 较小时       |
// | 阶乘数系统      | O(n^2)   | O(n)   | n≤10^4 ⭐      |
// | 阶乘+BIT       | O(nlogn) | O(n)   | n 极大时       |
//
// 面试策略:
// 1. 先说暴力 O(n!) 为什么不行
// 2. 推导出阶乘数系统的核心思路
// 3. 写出 O(n^2) 代码
// 4. 提及 BIT 优化到 O(n log n) 作为追问回答
//
// ============================================================
// 【易错点】
//
// 1. 忘记 k-1:
//    ✗ 直接用 k 做除法 → 边界情况出错（k是阶乘倍数时）
//    ✓ k-- 转为 0-indexed
//
// 2. 候选数字忘记移除:
//    ✗ 不 erase → 同一数字被多次选择
//    ✓ 选完后必须 nums.erase(nums.begin() + idx)
//
// 3. 阶乘下标搞混:
//    ✗ factorial[i] = (i+1)! → 除法结果全错
//    ✓ factorial[i] = i!, 其中 factorial[0] = 1
//
// 4. 大 n 阶乘溢出:
//    n=13 时 13! > INT_MAX (2^31 - 1)
//    本题 n≤9, 安全。更大 n 需要 long long。
//
// 5. to_string 忘了:
//    ✗ result += nums[idx]  → 追加的是 ASCII 码对应的字符
//    ✓ result += to_string(nums[idx]) → 追加数字的字符串形式
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1: 时间复杂度 O(n^2) 能优化吗？
//     erase 是 O(n)，如果用树状数组 (BIT) 维护候选集合：
//     - "查找第 idx 个存在的元素" → 二分 + BIT 前缀和 → O(log^2 n)
//     - "删除" → BIT 更新 → O(log n)
//     总体 O(n log^2 n)，或用线段树做到 O(n log n)。
//
// Q2: 给定排列求排名（康托展开正向）？
//     对排列每一位，统计候选中比它小的数字个数，乘以对应阶乘。
//     用 BIT 维护可以做到 O(n log n)。
//
// Q3: 如果有重复数字？
//     阶乘要除以重复元素的阶乘：n! / (c1! * c2! * ... * cm!)
//     其中 ci 是每个不同数字的出现次数。
//
// ============================================================
// 【相关题型】
//
// - 31. Next Permutation: O(n) 原地求下一个排列
// - 46. Permutations: 回溯生成所有排列
// - 47. Permutations II: 含重复元素的全排列
// - 556. Next Greater Element III: 数字版 Next Permutation
// ============================================================
