/*
 * LeetCode 888: Fair Candy Swap (公平的糖果交换)
 *
 * 【题目本质】
 * 数学推导 + 查找：Alice 交出 a 收入 b 后双方总量相等，
 * 推导出 b = a + (SB - SA) / 2，问题转化为查找。
 *
 * 【解法总览】
 * 解法1: 暴力枚举        — O(mn) / O(1)           — 最直觉
 * 解法2: HashSet 查找     — O(m+n) / O(n)          — 面试首选
 * 解法3: 排序 + 二分查找  — O((m+n)log(n)) / O(1)  — 可选方案
 * 解法4: 排序 + 双指针    — O((m+n)log(m+n)) / O(1)— 加分项
 */

// ============================================================
// 解法1: 暴力枚举 — 双重循环
// 时间: O(mn)  空间: O(1)
//
// 【思路】
// 枚举 Alice 的每个盒子 a 和 Bob 的每个盒子 b，
// 检查交换后双方总量是否相等。
//
// 交换条件: SA - a + b == SB - b + a
// 化简:     2b == 2a + SB - SA
//           b == a + (SB - SA) / 2
//
// 虽然暴力 O(mn) 效率低，但它清晰展示了条件化简的过程，
// 是推导出优化解法的基础。
// ============================================================
class Solution1 {
public:
    vector<int> fairCandySwap(vector<int>& aliceSizes, vector<int>& bobSizes) {
        int sumA = 0, sumB = 0;
        for (int a : aliceSizes) sumA += a;
        for (int b : bobSizes) sumB += b;

        // 枚举所有 (a, b) 对
        for (int a : aliceSizes) {
            for (int b : bobSizes) {
                // 检查交换后双方是否相等
                if (sumA - a + b == sumB - b + a) {
                    return {a, b};
                }
            }
        }
        return {}; // 题目保证有解
    }
};

// ============================================================
// 解法2: HashSet 查找 — 数学推导 + O(1) 查找（⭐ 面试首选）
// 时间: O(m+n)  空间: O(n)
//
// 【思路】
// 从暴力解的条件 SA-a+b = SB-b+a 推导:
//   b = a + (SB - SA) / 2
// 令 delta = (SB - SA) / 2
//
// 遍历 Alice 的每个盒子 a，查找 Bob 中是否存在 a + delta。
// 用 HashSet 将 Bob 的值存起来，查找 O(1)。
//
// 推导过程:
//   SA - a + b = SB - b + a        (交换后双方相等)
//   SA + SB = 2a - 2b + 2SB        (错误展开，重新来)
//   SA - a + b = SB - b + a
//   SA - SB = 2a - 2b
//   b = a - (SA - SB) / 2
//   b = a + (SB - SA) / 2          (统一形式)
//
// 示例: aliceSizes=[1,1], bobSizes=[2,2]
//   SA=2, SB=4, delta=(4-2)/2=1
//   a=1: target=1+1=2, bobSet 中有 2 → 返回 [1, 2] ✓
// ============================================================
class Solution2 {
public:
    vector<int> fairCandySwap(vector<int>& aliceSizes, vector<int>& bobSizes) {
        int sumA = 0, sumB = 0;
        for (int a : aliceSizes) sumA += a;
        for (int b : bobSizes) sumB += b;

        // 关键公式: b = a + delta
        int delta = (sumB - sumA) / 2;

        // 把 Bob 的盒子放入 HashSet（被查找方）
        unordered_set<int> bobSet(bobSizes.begin(), bobSizes.end());

        for (int a : aliceSizes) {
            int target = a + delta;
            if (bobSet.count(target)) {
                return {a, target};
            }
        }

        return {}; // 题目保证有解
    }
};

// ============================================================
// 解法3: 排序 + 二分查找 — O(1) 额外空间替代方案
// 时间: O((m+n)log(n))  空间: O(1)（原地排序）
//
// 【思路】
// 和解法2 相同的数学推导，但用二分查找代替 HashSet。
// 对 Bob 排序后，对 Alice 的每个 a 二分查找 a + delta。
//
// 优势: 不需要额外 O(n) 空间（如果面试官限制空间）
// 劣势: 时间多了 log(n) 因子
// ============================================================
class Solution3 {
public:
    vector<int> fairCandySwap(vector<int>& aliceSizes, vector<int>& bobSizes) {
        int sumA = 0, sumB = 0;
        for (int a : aliceSizes) sumA += a;
        for (int b : bobSizes) sumB += b;

        int delta = (sumB - sumA) / 2;

        // 对 Bob 排序以支持二分查找
        sort(bobSizes.begin(), bobSizes.end());

        for (int a : aliceSizes) {
            int target = a + delta;
            // 二分查找 target 是否在 bobSizes 中
            if (binary_search(bobSizes.begin(), bobSizes.end(), target)) {
                return {a, target};
            }
        }

        return {};
    }
};

// ============================================================
// 解法4: 排序 + 双指针 — 同步扫描
// 时间: O((m+n)log(m+n))  空间: O(1)
//
// 【思路】
// 两个数组都排序后，用双指针 i, j 同步扫描。
// 我们寻找满足 a - b == -delta 的对 (a, b)。
//
// 指针移动规则:
// - 如果 a - b < -delta: a 太小，移动 i（增大 a）
// - 如果 a - b > -delta: b 太小，移动 j（增大 b）
// - 如果 a - b == -delta: 找到答案
//
// 类似两数之和的排序双指针解法。
//
// 示例: aliceSizes=[1,2], bobSizes=[2,3]
//   SA=3, SB=5, delta=1, 寻找 a - b == -1
//   排序后: alice=[1,2], bob=[2,3]
//   i=0,j=0: 1-2=-1 == -delta → 返回 [1,2] ✓
// ============================================================
class Solution4 {
public:
    vector<int> fairCandySwap(vector<int>& aliceSizes, vector<int>& bobSizes) {
        int sumA = 0, sumB = 0;
        for (int a : aliceSizes) sumA += a;
        for (int b : bobSizes) sumB += b;

        int delta = (sumB - sumA) / 2;
        // 我们寻找 a - b == -delta，即 b = a + delta

        sort(aliceSizes.begin(), aliceSizes.end());
        sort(bobSizes.begin(), bobSizes.end());

        int i = 0, j = 0;
        int m = aliceSizes.size(), n = bobSizes.size();

        while (i < m && j < n) {
            int diff = aliceSizes[i] - bobSizes[j];

            if (diff == -delta) {
                // 找到: a + delta == b
                return {aliceSizes[i], bobSizes[j]};
            } else if (diff < -delta) {
                // a - b 太小 → a 太小 → 增大 a
                i++;
            } else {
                // a - b 太大 → b 太小 → 增大 b
                j++;
            }
        }

        return {};
    }
};

// ============================================================
// 【解法对比】
//
// | 解法         | 时间              | 空间  | 特点               |
// |-------------|------------------|------|-------------------|
// | 暴力枚举     | O(mn)            | O(1) | 展示条件推导        |
// | HashSet 查找 | O(m+n)           | O(n) | ⭐面试首选，最快    |
// | 排序+二分    | O((m+n)log(n))   | O(1) | 空间限制时用        |
// | 排序+双指针  | O((m+n)log(m+n)) | O(1) | 双指针练习         |
//
// 面试建议:
//   先推导数学公式 b = a + (SB-SA)/2
//   → 然后写 HashSet 解法（最优）
//   → 追问空间优化 → 排序 + 二分查找
//
// 【易错点】
// 1. delta 方向搞反:
//    ✗ delta = (sumA - sumB) / 2，然后 target = a + delta
//      当 sumA > sumB 时 delta > 0，Alice 交出 a 要找更大的 b，
//      但实际应该找更小的 → 答案错误
//    ✓ 统一: delta = (sumB - sumA) / 2, target = a + delta
//
// 2. HashSet 存错数组:
//    ✗ 把 Alice 放入 Set 遍历 Bob → 公式要改成 a = b - delta
//    ✓ 被查找方放入 Set（Bob），遍历方逐个查找（Alice）
//
// 3. 双指针移动方向:
//    ✗ diff < -delta 时移动 j → a 更小 b 更大，差距扩大
//    ✓ diff < -delta 时移动 i → 增大 a，缩小差距
//
// 4. 忘记题目保证有解:
//    ✗ 加了大量无解处理代码 → 代码臃肿
//    ✓ 题目保证至少一个答案，循环内必定 return
//
// 【面试追问 Interview Follow-ups】
// Q1: (SB-SA) 一定是偶数吗？
// A1: 题目保证有解。如果有解，SA-a+b = SB-b+a 意味着 SB-SA = 2(b-a)，
//     所以 SB-SA 一定是偶数。如果不保证有解，需要先检查奇偶性。
//
// Q2: 如果交换 k 个盒子呢？
// A2: 变成在 Alice 中选 k 个、Bob 中选 k 个使差值恰好弥补。
//     k=1 就是本题；k 任意是 NP 问题（子集和的变种）。
//
// Q3: HashSet vs 排序，哪个更好？
// A3: HashSet 时间更优 O(m+n) vs O((m+n)log(n))，
//     但排序空间更优 O(1) vs O(n)。面试通常首选 HashSet。
// ============================================================
