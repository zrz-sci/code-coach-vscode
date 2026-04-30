# 119. 杨辉三角 II

## 核心思路
返回杨辉三角的第 rowIndex 行，本质是用滚动数组或数学公式高效计算组合数 C(n, k)。

## 思维链
1. **暴力法**：逐行构建整个杨辉三角，然后返回第 rowIndex 行。O(n^2) 时间和空间
2. **空间优化**：只需要上一行来计算当前行，用两个数组交替使用。O(n) 空间
3. **原地滚动**：只用一个数组，从后往前更新（避免覆盖还需要的旧值）。O(n) 空间
4. **数学公式**：第 n 行第 k 个元素 = C(n, k)，利用递推公式 C(n,k) = C(n,k-1) * (n-k+1) / k
5. **最终方案**：原地滚动最实用，数学公式最优但要注意溢出

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 原地滚动DP | 单数组从后往前更新 | O(n^2) | O(n) | ⭐ 推荐 |
| 数学公式 | C(n,k) 递推计算 | O(n) | O(n) | ⭐ 进阶 |
| 逐行构建 | 完整杨辉三角 | O(n^2) | O(n^2) | 了解即可 |

## 关键提示
1. **从后往前更新的原因**：如果从前往后，`row[j] = row[j-1] + row[j]` 中 `row[j-1]` 已经被更新过了（是当前行的值而非上一行）
2. **组合数递推公式**：`C(n,k) = C(n,k-1) * (n-k+1) / k`，每次只需一次乘法和除法
3. **溢出风险**：rowIndex 最大为 33，C(33, 16) = 1166803110，在 int 范围内；但中间乘法可能溢出，用 long long

```
杨辉三角（0-indexed 行号）：

row 0:         1
row 1:        1 1
row 2:       1 2 1
row 3:      1 3 3 1
row 4:     1 4 6 4 1

滚动更新 row 3 -> row 4（从后往前）：
  [1, 3, 3, 1, 0]
           ↑ row[4] = 0+1 = 1
        ↑ row[3] = 1+3 = 4
     ↑ row[2] = 3+3 = 6
  ↑ row[1] = 3+1 = 4
  row[0] = 1（不变）
  结果: [1, 4, 6, 4, 1]
```

## 解法详解

### 解法1：原地滚动DP

**思考过程**：
- 杨辉三角第 i 行第 j 列 = 第 i-1 行第 j-1 列 + 第 i-1 行第 j 列
- 只保留一行数组，从后往前更新避免覆盖问题
- 每行长度增加 1，末尾补 1

```cpp
class Solution {
public:
    vector<int> getRow(int rowIndex) {
        vector<int> row(rowIndex + 1, 0);
        row[0] = 1;
        
        for (int i = 1; i <= rowIndex; i++) {
            // 从后往前更新，避免覆盖上一行的值
            for (int j = i; j >= 1; j--) {
                row[j] = row[j] + row[j - 1];
            }
        }
        
        return row;
    }
};
```

**关键点**：
- 初始化 row[0] = 1，其余为 0
- 外层循环 i 从 1 到 rowIndex
- 内层循环 j 从 i 到 1（从后往前），`row[j] += row[j-1]`

### 解法2：数学公式法

**思考过程**：
- 第 n 行第 k 个元素就是 C(n, k) = n! / (k! * (n-k)!)
- 递推：C(n, k) = C(n, k-1) * (n - k + 1) / k
- 从 C(n, 0) = 1 开始，逐个计算到 C(n, n)

```cpp
class Solution {
public:
    vector<int> getRow(int rowIndex) {
        vector<int> row(rowIndex + 1);
        row[0] = 1;
        
        for (int k = 1; k <= rowIndex; k++) {
            // C(n, k) = C(n, k-1) * (n - k + 1) / k
            // 用 long long 防止中间乘法溢出
            row[k] = (long long)row[k - 1] * (rowIndex - k + 1) / k;
        }
        
        return row;
    }
};
```

**关键点**：
- 必须先乘后除，保证整除
- C(n, k-1) * (n-k+1) 一定能被 k 整除（组合数性质）
- 用 long long 防止中间结果溢出

## 易错点

1. **滚动方向错误**
```cpp
// ✗ 错误：从前往后更新，row[j-1] 已经是当前行的值了
for (int j = 1; j <= i; j++) {
    row[j] = row[j] + row[j - 1];  // row[j-1] 已被污染！
}

// ✓ 正确：从后往前更新
for (int j = i; j >= 1; j--) {
    row[j] = row[j] + row[j - 1];
}
```

2. **数学公式溢出**
```cpp
// ✗ 错误：int 乘法溢出
row[k] = row[k - 1] * (rowIndex - k + 1) / k;

// ✓ 正确：强制转换为 long long
row[k] = (long long)row[k - 1] * (rowIndex - k + 1) / k;
```

3. **数组大小错误**
```cpp
// ✗ 错误：第 rowIndex 行有 rowIndex+1 个元素（0-indexed）
vector<int> row(rowIndex, 0);  // 少了一个！

// ✓ 正确
vector<int> row(rowIndex + 1, 0);
```

## 面试追问

**Q1: 为什么从后往前更新就不会覆盖？**
→ 因为 `row[j] = row[j] + row[j-1]`，我们先更新大索引。当计算 row[j] 时，row[j-1] 还没被更新，仍是上一行的值。如果从前往后，row[j-1] 已经变成当前行的值了。

**Q2: 数学公式法中为什么先乘后除能保证整除？**
→ 因为 C(n,k) = C(n,k-1) * (n-k+1) / k 中，连续 k 个整数 (n-k+1)...(n) 的乘积一定能被 k! 整除。递推过程中每步 `prev * (n-k+1)` 等于 `n! / ((k-1)! * (n-k)!)) * (n-k+1)`，分子恰好多了一个因子使得能被 k 整除。

**Q3: 如果 rowIndex 非常大（比如 10^6），怎么优化？**
→ 利用对称性 C(n,k) = C(n,n-k)，只算前半部分然后镜像。同时用 long long 或 __int128 处理溢出。如果需要模运算，可以用 Lucas 定理或预处理阶乘逆元。

## 相关题型
- [118. Pascal's Triangle](https://leetcode.com/problems/pascals-triangle/) - 返回完整杨辉三角
- [70. Climbing Stairs](https://leetcode.com/problems/climbing-stairs/) - 类似的滚动数组DP
- [62. Unique Paths](https://leetcode.com/problems/unique-paths/) - 本质也是组合数 C(m+n-2, m-1)
