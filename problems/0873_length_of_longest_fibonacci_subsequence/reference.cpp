/*
 * LeetCode 873: Length of Longest Fibonacci Subsequence
 * (最长的斐波那契子序列的长度)
 *
 * 【题目本质】
 * 在严格递增数组中，找最长的类斐波那契子序列（每个元素等于前两个之和）。
 * 核心：斐波那契序列由"最后两个数"唯一决定后续走向。
 *
 * 【解法总览】
 * 解法1: 暴力枚举+HashSet — O(n^2 logM) / O(n)  — 直观，枚举起始对
 * 解法2: DP+HashMap        — O(n^2) / O(n^2)     — 面试首选，状态转移
 */

// ============================================================
// 解法1: 暴力枚举 + HashSet
// 时间: O(n^2 * log M)  空间: O(n)  (M = max element)
//
// 【思路】
// 枚举所有可能的 (arr[i], arr[j]) 作为序列的前两个元素，
// 然后不断查找 arr[i]+arr[j] 是否存在于集合中。
// 斐波那契数列增长极快（指数级），所以 while 循环次数 ≤ log(max_val)。
//
// 示例: arr = [1,2,3,4,5,6,7,8]
//   (1,2): 查3✓→查5✓→查8✓→查13✗ → 长度5 [1,2,3,5,8]
//   (1,3): 查4✓→查7✓→查11✗       → 长度4 [1,3,4,7]
//   (2,3): 查5✓→查8✓→查13✗       → 长度4 [2,3,5,8]
//   ...
//   最终答案: 5
// ============================================================
class Solution1 {
public:
    int lenLongestFibSubseq(vector<int>& arr) {
        // 用 HashSet 存所有元素，支持 O(1) 查找
        unordered_set<int> s(arr.begin(), arr.end());
        int n = arr.size();
        int ans = 0;

        // 枚举起始的两个元素 arr[i], arr[j]
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                // 当前链的最后两个数
                int a = arr[i], b = arr[j];
                int len = 2; // 当前链长度（至少2）

                // 不断查找下一个斐波那契数
                while (s.count(a + b)) {
                    int c = a + b; // 下一个数
                    a = b;         // 向前滑动
                    b = c;
                    len++;
                }

                // 只有长度 >= 3 才算有效的斐波那契子序列
                if (len >= 3) {
                    ans = max(ans, len);
                }
            }
        }

        return ans; // 如果没有 >= 3 的链，ans 保持 0
    }
};

// ============================================================
// 解法2: DP + HashMap — 面试首选
// 时间: O(n^2)  空间: O(n^2)
//
// 【思路】
// 定义 dp[j][k] = 以 arr[j], arr[k] 结尾的最长斐波那契子序列长度。
//
// 转移方程：
//   对于每个 (j, k)，计算 val = arr[k] - arr[j]
//   如果 val < arr[j] 且 val 在数组中（设其索引为 i），则：
//     dp[j][k] = dp[i][j] + 1
//   否则 dp[j][k] = 2（只有 arr[j], arr[k] 两个元素）
//
// 为什么 val < arr[j]？
//   数组严格递增，前驱值 val 必须比 arr[j] 小才能保证 i < j。
//   如果 val >= arr[j]，那 val 不可能是 arr[j] 前面的元素。
//
// 状态转移示意:
//   arr = [1, 2, 3, 5, 8]
//   索引:  0  1  2  3  4
//
//   dp[1][2]: val=3-2=1, idx[1]=0, dp[0][1]+1=3  → [1,2,3]
//   dp[2][3]: val=5-3=2, idx[2]=1, dp[1][2]+1=4  → [1,2,3,5]
//   dp[3][4]: val=8-5=3, idx[3]=2, dp[2][3]+1=5  → [1,2,3,5,8]
//
// 遍历顺序：
//   外层 k 从 2 到 n-1，内层 j 从 1 到 k-1。
//   这样当我们计算 dp[j][k] 时，dp[i][j]（i < j < k）已经算好了。
// ============================================================
class Solution2 {
public:
    int lenLongestFibSubseq(vector<int>& arr) {
        int n = arr.size();
        int ans = 0;

        // 值 → 索引 的映射（数组严格递增，值唯一）
        unordered_map<int, int> idx;
        for (int i = 0; i < n; i++) {
            idx[arr[i]] = i;
        }

        // dp[j][k]: 以 arr[j], arr[k] 结尾的最长斐波那契子序列长度
        // 初始值全部为 2（任意两个数天然组成长度 2 的序列）
        vector<vector<int>> dp(n, vector<int>(n, 2));

        for (int k = 2; k < n; k++) {
            for (int j = 1; j < k; j++) {
                int val = arr[k] - arr[j]; // 期望的前驱值

                // val 必须 < arr[j]（保证前驱在 j 前面）
                // 且 val 必须存在于数组中
                if (val < arr[j] && idx.count(val)) {
                    int i = idx[val]; // 前驱的索引
                    dp[j][k] = dp[i][j] + 1;
                    ans = max(ans, dp[j][k]);
                }
            }
        }

        // 如果 ans < 3，说明不存在有效的斐波那契子序列
        return ans >= 3 ? ans : 0;
    }
};

// ============================================================
// 解法2 变体: 用 unordered_map 代替二维数组节省空间
// 时间: O(n^2)  空间: O(n^2) 最坏，但实际稀疏时更省
//
// 当 n 较大但有效的 (j,k) 对较少时，用 map 比二维数组更省空间。
// 用 j*n+k 作为 key 编码两个索引。
// ============================================================
class Solution2b {
public:
    int lenLongestFibSubseq(vector<int>& arr) {
        int n = arr.size();
        int ans = 0;

        unordered_map<int, int> idx;
        for (int i = 0; i < n; i++) {
            idx[arr[i]] = i;
        }

        // 用 map 替代二维数组，只存有值的状态
        unordered_map<long long, int> dp;

        for (int k = 2; k < n; k++) {
            for (int j = 1; j < k; j++) {
                int val = arr[k] - arr[j];
                if (val < arr[j] && idx.count(val)) {
                    int i = idx[val];
                    long long key_ij = (long long)i * n + j;
                    long long key_jk = (long long)j * n + k;
                    int prev = dp.count(key_ij) ? dp[key_ij] : 2;
                    dp[key_jk] = prev + 1;
                    ans = max(ans, dp[key_jk]);
                }
            }
        }

        return ans >= 3 ? ans : 0;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法          | 时间           | 空间    | 优势                      |
// |--------------|----------------|---------|--------------------------|
// | 暴力+HashSet  | O(n^2 log M)   | O(n)   | 代码直观，空间小           |
// | DP+HashMap    | O(n^2)         | O(n^2) | 时间最优，适合 n 较大      |
// | DP+稀疏Map    | O(n^2)         | O(有效对)| 实际稀疏时更省空间         |
//
// 面试选择：建议讲解法1直观思路，然后优化到解法2。
// 解法2的状态设计 dp[j][k] 是关键考点。
//
// ============================================================
// 【易错点总结】
//
// 1. 忘记 val < arr[j] 的前驱条件:
//    ✗ 直接查 idx.count(arr[k]-arr[j]) → 可能找到 j 右边的元素
//    ✓ 加 val < arr[j] 条件，保证前驱在 j 左边
//
// 2. 答案不足 3 时返回 0:
//    ✗ return ans; → 如果只有两个数配对，返回 2 是错的
//    ✓ return ans >= 3 ? ans : 0;
//
// 3. dp 初始值不是 0:
//    ✗ dp 全部初始化为 0 → dp[i][j]+1 变成 1，链长计算错误
//    ✓ dp 全部初始化为 2，表示任意两个元素构成最短的链
//
// ============================================================
// 【相关题型】
//
// - 509. 斐波那契数: 基础斐波那契计算。
// - 1027. 最长等差数列: dp[i][d] 表示以 arr[i] 结尾、公差 d 的最长等差序列。
//   同样是"二维状态"的 DP 设计思路。
// - 446. 等差数列划分 II: dp[i][d] 记录子序列个数，核心类似。
// - 368. 最大整除子集: 子序列+整除关系约束，排序+DP。
// ============================================================
