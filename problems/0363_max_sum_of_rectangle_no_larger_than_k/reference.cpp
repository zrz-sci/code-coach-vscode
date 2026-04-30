/*
 * LeetCode 363: Max Sum of Rectangle No Larger Than K
 * 难度: Hard | 标签: Array, Binary Search, Matrix, Ordered Set, Prefix Sum
 *
 * 核心思路: 列压缩降维 + 前缀和 + 有序集合二分
 *
 * 降维示意:
 *   固定左列 l=0, 右列 r=2
 *   matrix:  [1,  0,  1]     colSum[0] = 1+0+1 = 2
 *            [0, -2,  3]     colSum[1] = 0-2+3 = 1
 *
 *   问题 -> 在 colSum=[2,1] 中找子数组和 <= k 的最大值
 *
 * 前缀和二分原理:
 *   prefix[j] - prefix[i] <= k
 *   => prefix[i] >= prefix[j] - k
 *   => set.lower_bound(prefix[j] - k) 找最小满足值
 */


// ================================================================
// 解法1: 列压缩 + 有序集合二分 -- 面试首选
// ================================================================
// 时间: O(n^2 * m * log m)  n=列数, m=行数
// 空间: O(m)
//
// 步骤:
//   1. 枚举左列 l: 0 -> n-1
//   2. 枚举右列 r: l -> n-1, 累加每行 l~r 列的和到 colSum[]
//   3. 在 colSum[] 中找子数组和 <= k 的最大值:
//      - 维护前缀和 prefix
//      - 用 set 存储已遍历的前缀和
//      - lower_bound(prefix - k) 找满足条件的最优 prev
//      - 矩形和 = prefix - prev <= k
//
// 过程可视化 (matrix=[[1,0,1],[0,-2,3]], k=2):
//   c1=0,c2=2: colSum=[2,1]
//     prefix: 0 -> 2 -> 3
//     r=0: pref=2, lb(0)->0, 2-0=2  ans=2
//     r=1: pref=3, lb(1)->2, 3-2=1
class Solution {
public:
    int maxSumSubmatrix(vector<vector<int>>& matrix, int k) {
        int m = matrix.size(), n = matrix[0].size();
        int ans = INT_MIN;

        // 枚举左列边界
        for (int l = 0; l < n; l++) {
            // colSum[i] = matrix[i][l..r] 的行和
            vector<int> colSum(m, 0);

            // 枚举右列边界 (增量更新)
            for (int r = l; r < n; r++) {
                for (int i = 0; i < m; i++) {
                    colSum[i] += matrix[i][r];
                }

                // 一维子问题: 前缀和 + 有序集合
                set<int> prefixSet;
                prefixSet.insert(0);  // 空前缀
                int prefix = 0;

                for (int i = 0; i < m; i++) {
                    prefix += colSum[i];
                    // 找最小的 prev 使得 prev >= prefix - k
                    auto it = prefixSet.lower_bound(prefix - k);
                    if (it != prefixSet.end()) {
                        ans = max(ans, prefix - *it);
                    }
                    prefixSet.insert(prefix);
                }
            }
        }
        return ans;
    }
};


// ================================================================
// 解法2: 行列优化 + Kadane剪枝 -- Follow-up最优
// ================================================================
// 时间: O(min(m,n)^2 * max(m,n) * log(max(m,n)))  最坏
//       O(min(m,n)^2 * max(m,n))                    Kadane剪枝后
// 空间: O(max(m,n))
//
// 优化:
//   1. 让短边做外层枚举 -> O(min^2) 取代 O(n^2)
//   2. Kadane先检查: 若最大子数组和 <= k，直接用，省去set开销
class Solution2 {
public:
    int maxSumSubmatrix(vector<vector<int>>& matrix, int k) {
        int m = matrix.size(), n = matrix[0].size();
        int ans = INT_MIN;
        bool transposed = (m < n);
        if (transposed) swap(m, n);

        for (int l = 0; l < n; l++) {
            vector<int> colSum(m, 0);
            for (int r = l; r < n; r++) {
                for (int i = 0; i < m; i++) {
                    colSum[i] += transposed ? matrix[r][i] : matrix[i][r];
                }

                // Kadane快速检查
                int kadaneMax = colSum[0], cur = colSum[0];
                for (int i = 1; i < m; i++) {
                    cur = max(colSum[i], cur + colSum[i]);
                    kadaneMax = max(kadaneMax, cur);
                }
                if (kadaneMax <= k) {
                    ans = max(ans, kadaneMax);
                    if (ans == k) return k;  // 恰好等于k，最优
                    continue;  // 跳过有序集合
                }

                // 有序集合方法
                set<int> prefixSet;
                prefixSet.insert(0);
                int prefix = 0;
                for (int i = 0; i < m; i++) {
                    prefix += colSum[i];
                    auto it = prefixSet.lower_bound(prefix - k);
                    if (it != prefixSet.end()) {
                        ans = max(ans, prefix - *it);
                        if (ans == k) return k;
                    }
                    prefixSet.insert(prefix);
                }
            }
        }
        return ans;
    }
};


// ================================================================
// 解法3: 列压缩 + 暴力枚举子数组 -- 简单但慢
// ================================================================
// 时间: O(n^2 * m^2)
// 空间: O(m)
//
// 不用有序集合，直接枚举所有起止行
class Solution3 {
public:
    int maxSumSubmatrix(vector<vector<int>>& matrix, int k) {
        int m = matrix.size(), n = matrix[0].size();
        int ans = INT_MIN;

        for (int l = 0; l < n; l++) {
            vector<int> colSum(m, 0);
            for (int r = l; r < n; r++) {
                for (int i = 0; i < m; i++) {
                    colSum[i] += matrix[i][r];
                }
                // 暴力枚举所有子数组 [i..j]
                for (int i = 0; i < m; i++) {
                    int sum = 0;
                    for (int j = i; j < m; j++) {
                        sum += colSum[j];
                        if (sum <= k) {
                            ans = max(ans, sum);
                        }
                    }
                }
            }
        }
        return ans;
    }
};


/*
 * ================================================================
 * 复杂度对比:
 *
 * | 解法               | 时间                    | 空间   |
 * |-------------------|------------------------|--------|
 * | 列压缩+有序集合     | O(n^2 * m * log m)     | O(m)   |
 * | 行列优化+Kadane    | O(min^2 * max * logmax)| O(max) |
 * | 列压缩+暴力        | O(n^2 * m^2)           | O(m)   |
 *
 * ================================================================
 * 易错点:
 *
 * 1. set 忘记插入 0 -> 漏掉从第0行开始的矩形
 * 2. lower_bound 方向错 -> 应该找 >= prefix-k 的最小值
 * 3. ans 初始化为 0 -> 全负矩阵时答案错误
 * 4. colSum 在 r 循环内重新初始化 -> 丢失增量累加
 * ================================================================
 */
