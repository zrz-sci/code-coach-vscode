# 363. 矩形区域不超过 K 的最大数值和

## 核心思路
将二维矩阵问题降维为一维问题：固定左右列边界后将行方向压缩为一维数组，转化为"子数组和不超过K的最大值"问题，再用有序集合二分查找求解。

## 思维链

1. **暴力法**: 枚举所有可能的矩形(左上角+右下角)，用前缀和计算面积，O(m^2 * n^2)
2. **降维思想**: 固定左列l和右列r，将每行l~r的元素求和压成一维数组colSum[]
3. **一维子问题**: 在colSum[]中找最大子数组和 <= k，这就是经典的"最大子数组和有上界"问题
4. **前缀和+二分**: 求prefix[j] - prefix[i] <= k中的最大值，等价于找prefix[i] >= prefix[j] - k的最小值
5. **有序集合**: 用set维护已遍历的前缀和，`lower_bound(prefix[j] - k)` 即可O(log m)找到
6. **优化**: 若行数 > 列数，转置矩阵让短边做列枚举，总复杂度 O(min(m,n)^2 * max(m,n) * log(max(m,n)))

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 列压缩+有序集合 | 固定左右列，行压缩为1D，set二分 | O(n^2 * m log m) | O(m) | ⭐ |
| 列压缩+Kadane剪枝 | 先Kadane检查，需要时再用set | O(min^2 * max log max) | O(max) | |
| 列压缩+暴力 | 固定左右列，枚举所有子数组和 | O(n^2 * m^2) | O(m) | |
| 2D前缀和暴力 | 枚举所有矩形 | O(m^2 * n^2) | O(mn) | |

## 关键提示

1. **降维示意图**:
```
固定列边界 l=0, r=2:
  col: 0  1  2
      [1, 0, 1]      colSum[0] = 1+0+1 = 2
      [0,-2, 3]      colSum[1] = 0+(-2)+3 = 1

问题 -> 在 colSum = [2, 1] 中找子数组和 <= k 的最大值
```

2. **前缀和二分查找原理**:
```
prefix[j] - prefix[i] <= k
=> prefix[i] >= prefix[j] - k
=> 在已有的 prefix 集合中找 >= (prefix[j] - k) 的最小值
=> set.lower_bound(prefix[j] - k)

例: prefix=5, k=2, set={0,1,3,4,7}
    need prev >= 5-2 = 3
    lower_bound(3) -> 3
    sum = 5-3 = 2 <= k
```

3. **为什么不能只用Kadane算法**: Kadane找最大子数组和，但这里需要 <= k 的约束，Kadane无法处理上界限制。可以先用Kadane检查：若最大和 <= k，直接采用，省去O(log n)的set开销

4. **set初始化必须插入0**: 代表"从头开始的子数组"，否则漏掉从第0行开始的矩形

5. **行列优化**: 若 m >> n，转置矩阵让短边做外层枚举。时间O(n^2)中n取min(m,n)

## 解法详解

### 解法1: 列压缩 + 有序集合二分 (最优)

```cpp
class Solution {
public:
    int maxSumSubmatrix(vector<vector<int>>& matrix, int k) {
        int m = matrix.size(), n = matrix[0].size();
        int ans = INT_MIN;

        for (int l = 0; l < n; l++) {
            vector<int> colSum(m, 0);
            for (int r = l; r < n; r++) {
                for (int i = 0; i < m; i++)
                    colSum[i] += matrix[i][r];

                set<int> prefixSet;
                prefixSet.insert(0);
                int prefix = 0;
                for (int i = 0; i < m; i++) {
                    prefix += colSum[i];
                    auto it = prefixSet.lower_bound(prefix - k);
                    if (it != prefixSet.end())
                        ans = max(ans, prefix - *it);
                    prefixSet.insert(prefix);
                }
            }
        }
        return ans;
    }
};
```

### 解法2: 行列优化 + Kadane剪枝

```cpp
class Solution {
public:
    int maxSumSubmatrix(vector<vector<int>>& matrix, int k) {
        int m = matrix.size(), n = matrix[0].size();
        int ans = INT_MIN;
        bool transposed = m < n;
        if (transposed) swap(m, n);

        for (int l = 0; l < n; l++) {
            vector<int> colSum(m, 0);
            for (int r = l; r < n; r++) {
                for (int i = 0; i < m; i++)
                    colSum[i] += transposed ? matrix[r][i] : matrix[i][r];

                // Kadane快速检查
                int kadaneMax = colSum[0], cur = colSum[0];
                for (int i = 1; i < m; i++) {
                    cur = max(colSum[i], cur + colSum[i]);
                    kadaneMax = max(kadaneMax, cur);
                }
                if (kadaneMax <= k) { ans = max(ans, kadaneMax); continue; }

                set<int> prefixSet;
                prefixSet.insert(0);
                int prefix = 0;
                for (int i = 0; i < m; i++) {
                    prefix += colSum[i];
                    auto it = prefixSet.lower_bound(prefix - k);
                    if (it != prefixSet.end())
                        ans = max(ans, prefix - *it);
                    prefixSet.insert(prefix);
                }
            }
        }
        return ans;
    }
};
```

## 易错点

1. **忘记在set中插入初始值0**:
```cpp
// ✗ 错误: 没有初始0，无法找到从头开始的子数组
set<int> prefixSet;
int prefix = 0;
for (int i = 0; i < m; i++) { ... }
// ✓ 正确: 先插入0代表空前缀
set<int> prefixSet;
prefixSet.insert(0);
```

2. **lower_bound方向搞反**:
```cpp
// ✗ 错误: 找的是 <= prefix-k 的值
auto it = prefixSet.upper_bound(k - prefix);
// ✓ 正确: 找 >= prefix-k 的最小值
auto it = prefixSet.lower_bound(prefix - k);
```

3. **ans初始值错误**:
```cpp
// ✗ 错误: 矩阵可能全为负数，0可能不是合法答案
int ans = 0;
// ✓ 正确: 初始化为最小整数
int ans = INT_MIN;
```

4. **colSum在错误位置重置**:
```cpp
// ✗ 错误: colSum在r循环内重新初始化，丢失增量累加
for (int r = l; r < n; r++) {
    vector<int> colSum(m, 0);  // 每次清零了!
    ...
}
// ✓ 正确: colSum在l循环内初始化，r循环内增量累加
vector<int> colSum(m, 0);
for (int r = l; r < n; r++) {
    for (int i = 0; i < m; i++) colSum[i] += matrix[i][r];
    ...
}
```

## 面试追问

**Q1: 为什么要先固定列边界再压缩行？**
> 固定左列l和右列r后，每行l到r的和形成一维数组。问题降维为"一维子数组和不超过K的最大值"，这个子问题有高效的前缀和+二分解法。

**Q2: 如果行数远大于列数怎么优化？**
> 转置矩阵，让短边做列枚举。外层循环O(n^2)中n取min(m,n)，内层O(max(m,n) log max(m,n))。还可以加Kadane剪枝：先O(n)求无约束最大和，若 <= k直接用。

**Q3: 如果去掉"不超过K"的约束，最大子矩阵和怎么求？**
> 列压缩后用Kadane算法求一维最大子数组和即可，不需要有序集合。总复杂度O(n^2 * m)。

## 相关题型

| 题号 | 题目 | 关系 |
|------|------|------|
| 53 | Maximum Subarray | 一维最大子数组和 (Kadane) |
| 85 | Maximal Rectangle | 二维降一维+单调栈 |
| 304 | Range Sum Query 2D | 二维前缀和 |
| 560 | Subarray Sum Equals K | 前缀和+哈希 |
| 862 | Shortest Subarray with Sum at Least K | 前缀和+单调队列 |
