# 931. 下降路径最小和 (Minimum Falling Path Sum)

## 难度: Medium | 标签: Array, Dynamic Programming, Matrix

---

## 题目理解

给定一个 `n x n` 的整数矩阵 `matrix`，找到一条从**第一行任意位置**出发到**最后一行**的**下降路径**，使路径上元素之和最小。

**下降规则**: 从位置 `(row, col)` 出发，下一步只能走到:

```
                 (row, col)
                /    |    \
  (row+1, col-1)  (row+1, col)  (row+1, col+1)
      左下方          正下方          右下方
```

**注意**: 元素可以是**负数** (范围 -100 到 100)，不能贪心。

---

## 思维链 -- 从零推导

### Step 1: 为什么不能贪心？

```
矩阵:
[  1,  2, 100 ]
[  5,  3,   4 ]
[  6,  1,   7 ]

贪心: 从第一行最小值 1 开始 -> 1
  下一行相邻中最小: min(5, 3) = 3 -> 路径和 = 4
  下一行相邻中最小: min(6, 1) = 1  -> 路径和 = 5
贪心答案: 5

但是考虑: 起点2 -> 3 -> 1 = 6? 不如5。
起点1(0,0) -> 3(1,1) -> 1(2,1) = 5。路径合法吗？
  (0,0)->(1,1): col差=1，合法
  (1,1)->(2,1): col差=0，合法
路径和 = 1 + 3 + 1 = 5 (碰巧贪心对了)

但如果矩阵是:
[ 10,  1, 10 ]
[ 10, 10,  1 ]
[  1, 10, 10 ]
贪心: 1(0,1) -> min(10,10,1)=1(1,2) -> min(10,10)=10 -> 总和 = 12
最优: 10(0,0) -> 10(1,1) -> 1(2,0)? (0,0)->(1,1) OK, (1,1)->(2,0) OK. 和=21
或: 1(0,1) -> 1(1,2) -> 10(2,2)? 和=12. 另: 10(0,2) -> 1(1,2) -> 10(2,2) 和=21
实际最优就是 12。
```

关键点: 每一步的选择会**限制下一步的范围**。需要考虑所有路径 -> **动态规划**。

### Step 2: 定义子问题

> `dp[i][j]` = 从第一行某个位置出发，到达位置 `(i, j)` 的**最小路径和**

### Step 3: 推导转移方程

到达 `(i, j)` 的上一步只可能来自三个位置:

```
    dp[i-1][j-1]  dp[i-1][j]  dp[i-1][j+1]
               \       |       /
                \      |      /
                 dp[i][j]
```

转移方程:

```
dp[i][j] = matrix[i][j] + min(dp[i-1][j-1], dp[i-1][j], dp[i-1][j+1])
```

**边界处理**:
- `j == 0`: 没有 `dp[i-1][j-1]`，只从正上方和右上方转移
- `j == n-1`: 没有 `dp[i-1][j+1]`，只从左上方和正上方转移

### Step 4: 初始条件

第一行就是出发点，没有上一行:

```
dp[0][j] = matrix[0][j]  (对所有 j)
```

### Step 5: 答案

路径可以在最后一行**任意列**结束:

```
answer = min(dp[n-1][0], dp[n-1][1], ..., dp[n-1][n-1])
```

**不是 dp[n-1][n-1]!** 这是常见错误。

---

## DP 填表演示

以 `matrix = [[2,1,3],[6,5,4],[7,8,9]]` 为例:

### 原始矩阵:

```
     col0  col1  col2
row0:  2     1     3
row1:  6     5     4
row2:  7     8     9
```

### 第 0 行 (初始化 -- 直接复制):

```
dp[0][0] = 2    dp[0][1] = 1    dp[0][2] = 3
```

### 第 1 行 (逐格计算):

```
dp[1][0] = 6 + min(dp[0][0], dp[0][1])
                    ^正上方     ^右上方     (j=0没有左上方)
         = 6 + min(2, 1) = 6 + 1 = 7

dp[1][1] = 5 + min(dp[0][0], dp[0][1], dp[0][2])
                    ^左上方     ^正上方     ^右上方
         = 5 + min(2, 1, 3) = 5 + 1 = 6

dp[1][2] = 4 + min(dp[0][1], dp[0][2])
                    ^左上方     ^正上方     (j=n-1没有右上方)
         = 4 + min(1, 3) = 4 + 1 = 5
```

### 第 2 行 (逐格计算):

```
dp[2][0] = 7 + min(dp[1][0], dp[1][1])
         = 7 + min(7, 6) = 7 + 6 = 13

dp[2][1] = 8 + min(dp[1][0], dp[1][1], dp[1][2])
         = 8 + min(7, 6, 5) = 8 + 5 = 13

dp[2][2] = 9 + min(dp[1][1], dp[1][2])
         = 9 + min(6, 5) = 9 + 5 = 14
```

### 完整 DP 表:

```
     col0  col1  col2
row0:  2     1     3      <- 初始化 (复制 matrix 第0行)
row1:  7     6     5      <- 逐格计算
row2: 13    13    14      <- 逐格计算
             ^
答案 = min(13, 13, 14) = 13
```

### 回溯最优路径:

```
dp[2][0] = 13: 来自 dp[1][1]=6 -> dp[0][1]=1
  路径: matrix[0][1]=1 -> matrix[1][1]=5 -> matrix[2][0]=7 = 13

dp[2][1] = 13: 来自 dp[1][2]=5 -> dp[0][1]=1
  路径: matrix[0][1]=1 -> matrix[1][2]=4 -> matrix[2][1]=8 = 13

两条最优路径，和都是 13。
```

---

## 空间优化详解

### 观察: dp[i][j] 只依赖第 i-1 行

```
依赖关系:

row i-2:  [已经用完，不再需要]
row i-1:  [..., dp[i-1][j-1], dp[i-1][j], dp[i-1][j+1], ...]  <- 读取
row i:    [...,               dp[i][j],                 ...]  <- 写入

计算第 i 行时，只需要第 i-1 行的值。第 i-2 行及更早的行不再被访问。
```

### 优化路径: O(n^2) -> O(n) -> O(1)

```
二维 DP:   新建 n x n 的 dp 数组     -> 空间 O(n^2)
                  |
                  v  (dp[i] 只依赖 dp[i-1])
一维滚动:   只保留 prev[] 和 curr[]   -> 空间 O(n)
                  |
                  v  (读第 i-1 行、写第 i 行，行不同，不冲突)
原地修改:   直接在 matrix 上累加      -> 空间 O(1)
```

### 原地修改的正确性

```
处理第 i 行时:
  matrix[i-1][...]  已经是 dp 值 (上一轮累加过的结果)
  matrix[i][j]      还是原始值 (本轮尚未处理)

  matrix[i][j] += min(matrix[i-1][j-1], matrix[i-1][j], matrix[i-1][j+1])

  读的是第 i-1 行 (已处理完毕，不会再改)
  写的是第 i 行 (当前正在处理)
  读写在不同行，不存在"读到脏数据"的问题!
```

### 一维滚动数组方案 (不修改原矩阵)

```
prev = matrix[0] 的拷贝           // 上一行的 dp 值

for i in 1..n-1:
    curr = new array of size n    // 当前行的 dp 值
    for j in 0..n-1:
        best = prev[j]            // 正上方
        if j > 0:    best = min(best, prev[j-1])    // 左上方
        if j < n-1:  best = min(best, prev[j+1])    // 右上方
        curr[j] = matrix[i][j] + best
    prev = curr                   // 滚动: 当前行变成下一轮的"上一行"

answer = min(prev)
```

**注意**: 不能只用一个一维数组从左到右原地更新! 因为更新 `dp[j]` 后，计算 `dp[j+1]` 还需要 `prev[j]`（旧值已被覆盖）。必须用两个数组交替。

---

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| 原地修改 | 直接在 matrix 上累加 | O(n^2) | O(1) | 面试最佳 |
| 一维滚动数组 | prev + curr 两个数组 | O(n^2) | O(n) | 不改原数组时用 |
| 二维 DP | 新建 dp 数组 | O(n^2) | O(n^2) | 最易理解 |
| 记忆化递归 | Top-Down + memo | O(n^2) | O(n^2) | 递归思维 |

---

## 解法详解

### 解法1: 原地修改 -- O(1) 空间，面试最佳

```cpp
class Solution {
public:
    int minFallingPathSum(vector<vector<int>>& matrix) {
        int n = matrix.size();
        
        // 从第1行开始，逐行更新
        for (int i = 1; i < n; i++) {
            for (int j = 0; j < n; j++) {
                // 正上方一定存在
                int best = matrix[i - 1][j];
                // 左上方 (j > 0 时存在)
                if (j > 0)
                    best = min(best, matrix[i - 1][j - 1]);
                // 右上方 (j < n-1 时存在)
                if (j < n - 1)
                    best = min(best, matrix[i - 1][j + 1]);
                // 累加到当前格
                matrix[i][j] += best;
            }
        }
        
        // 答案: 最后一行的最小值 (不是 dp[n-1][n-1]!)
        return *min_element(matrix[n - 1].begin(), matrix[n - 1].end());
    }
};
```

### 解法2: 一维滚动数组 -- O(n) 空间，不修改原矩阵

```cpp
class Solution {
public:
    int minFallingPathSum(vector<vector<int>>& matrix) {
        int n = matrix.size();
        
        // prev: 上一行的 dp 值
        vector<int> prev(matrix[0].begin(), matrix[0].end());
        
        for (int i = 1; i < n; i++) {
            // curr: 当前行的 dp 值 (必须新开数组!)
            vector<int> curr(n);
            for (int j = 0; j < n; j++) {
                int best = prev[j];
                if (j > 0)     best = min(best, prev[j - 1]);
                if (j < n - 1) best = min(best, prev[j + 1]);
                curr[j] = matrix[i][j] + best;
            }
            // 滚动: 当前行变成下一轮的"上一行"
            prev = move(curr);
        }
        
        return *min_element(prev.begin(), prev.end());
    }
};
```

### 解法3: 记忆化递归 (Top-Down)

```cpp
class Solution {
public:
    int minFallingPathSum(vector<vector<int>>& matrix) {
        int n = matrix.size();
        // memo[i][j] = 从 (i,j) 出发到底部的最小和
        vector<vector<int>> memo(n, vector<int>(n, INT_MAX));
        
        int ans = INT_MAX;
        for (int j = 0; j < n; j++) {
            ans = min(ans, dfs(matrix, memo, 0, j));
        }
        return ans;
    }
    
private:
    int dfs(vector<vector<int>>& matrix, vector<vector<int>>& memo,
            int i, int j) {
        int n = matrix.size();
        if (j < 0 || j >= n) return INT_MAX;  // 越界
        if (i == n - 1) return matrix[i][j];   // 到达最后一行
        if (memo[i][j] != INT_MAX) return memo[i][j];  // 已计算
        
        int down      = dfs(matrix, memo, i + 1, j);
        int downLeft  = dfs(matrix, memo, i + 1, j - 1);
        int downRight = dfs(matrix, memo, i + 1, j + 1);
        
        memo[i][j] = matrix[i][j] + min({down, downLeft, downRight});
        return memo[i][j];
    }
};
```

---

## 易错点

### 1. 边界条件

```
j == 0:     没有左上方，只从 dp[i-1][0] 和 dp[i-1][1] 转移
j == n-1:   没有右上方，只从 dp[i-1][n-2] 和 dp[i-1][n-1] 转移
```

### 2. 答案是最后一行的最小值

```
错: return dp[n-1][n-1];          // 只看右下角
对: return min(dp[n-1][0..n-1]);  // 路径可以在任意列结束
```

### 3. 负数陷阱

```
矩阵元素范围 [-100, 100]。
错: 用 0 做 dp 初始值
对: dp[0][j] = matrix[0][j]  (直接用原始值)
```

### 4. 滚动数组必须用两个数组

```
错: 只用一个数组从左到右更新 dp[j]
    -> 更新 dp[0] 后，计算 dp[1] 时需要 prev[0] 但已被覆盖!
对: 用 prev[] 和 curr[] 两个数组交替
```

### 5. n=1 的特殊情况

```
矩阵只有一个元素，直接返回 matrix[0][0]。
循环 for(i=1..n-1) 不会执行，自然正确。
```

---

## 复杂度分析

| 解法 | 时间 | 空间 |
|------|------|------|
| 原地修改 | O(n^2) | O(1) |
| 一维滚动数组 | O(n^2) | O(n) |
| 二维 DP | O(n^2) | O(n^2) |
| 记忆化递归 | O(n^2) | O(n^2) |

空间优化路径: `O(n^2) -> O(n) -> O(1)`

---

## 面试追问

**Q1: 如果不允许修改原矩阵？**
> 用一维滚动数组，O(n) 空间。prev 和 curr 两个数组交替使用。

**Q2: 如果要输出具体路径？**
> 额外记录 `choice[i][j]` 表示 (i,j) 从哪个 (i-1, ?) 转移来的。最后从末行最小值位置向上回溯。

**Q3: 如果矩阵不是方阵 (m x n)？**
> 转移方程不变，只需修改外层循环为 m 行、内层循环为 n 列。

**Q4: 为什么不能贪心？**
> 每步选择限制了下一步的范围。局部最优不等于全局最优。需要 DP 考虑所有路径。

---

## 相关题目

| 题号 | 题目 | 关系 |
|------|------|------|
| 64 | Minimum Path Sum | 类似 DP，但只能向右/下 |
| 120 | Triangle | 三角形路径，经典下降 DP |
| 1289 | Minimum Falling Path Sum II | 进阶版，不能选同列 |
| 174 | Dungeon Game | 逆向 DP |
