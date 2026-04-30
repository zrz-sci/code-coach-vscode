# 552. Student Attendance Record II

[LeetCode 链接](https://leetcode.com/problems/student-attendance-record-ii/)

## 核心思路

**状态机 DP**: 将出勤记录的两个合法约束 (A < 2 次, 连续 L < 3 次) 编码为 DP 状态 `dp[已用A次数][末尾连续L次数]`。共 2*3=6 个状态, 每天从上一天的 6 个状态转移到新的 6 个状态。用滚动数组将空间优化到 O(1)。

这是 551 的"计数版": 551 检查一个记录是否合法, 552 计数所有合法记录数。暴力枚举 3^n 种组合不可行 (n 可达 10^5), 状态机 DP 在 O(n) 时间内解决。

---

## 思维链

```
输入: 整数 n (记录长度)
输出: 长度为 n 的合法记录总数 (mod 10^9+7)

Step 1  定义状态:
        dp[a][l] = 方案数, 其中 a = 已用 A 次数 (0或1), l = 末尾连续 L 次数 (0,1,2)
        共 2 * 3 = 6 个状态

Step 2  初始化: dp[0][0] = 1 (空串: 0个A, 0个连续L)

Step 3  转移 (对每一天, 选 P / L / A):
        ┌─ 放 P: dp[a][l] → ndp[a][0]    (连续 L 重置为 0)
        ├─ 放 L: dp[a][l] → ndp[a][l+1]  (仅当 l+1 < 3)
        └─ 放 A: dp[0][l] → ndp[1][0]    (仅当 a == 0, 连续 L 重置)

Step 4  答案: 所有 6 个状态之和 = sum(dp[a][l]) for a∈{0,1}, l∈{0,1,2}

时间: O(n) — 每天转移 6 个状态, O(6n)
空间: O(1) — 只需两组 dp[2][3] (或 6 个变量)
```

**手工验证 n=2:**
```
初始: dp[0][0]=1

Day 1: (0,0)=1
  P → (0,0)=1  L → (0,1)=1  A → (1,0)=1

Day 2:
  从(0,0)=1: P→(0,0)+=1  L→(0,1)+=1  A→(1,0)+=1
  从(0,1)=1: P→(0,0)+=1  L→(0,2)+=1  A→(1,0)+=1
  从(1,0)=1: P→(1,0)+=1  L→(1,1)+=1  A→X(a=1)

结果: dp = {{2,1,1},{3,1,0}} → 总和 = 8 ✓
```

---

## 解法概览

| 解法 | 时间 | 空间 | 推荐 | 说明 |
|------|------|------|------|------|
| DP 滚动数组 (2*3) | O(n) | O(1) | ⭐ | 面试推荐, 简洁清晰 |
| DP 6变量展开 | O(n) | O(1) | | 更快, 无数组开销 |
| 矩阵快速幂 (6x6) | O(log n) | O(1) | | n 极大时 (如 10^18), 进阶加分 |

---

## 关键提示

1. **状态空间极小**: 只有 `2 * 3 = 6` 个状态, 可以用 6 个变量或 `dp[2][3]` 数组。
2. **每步只依赖上一步**: 空间优化到 O(1), 用 `ndp` 和 `dp` 交替。
3. **取模**: 每步 `% (10^9+7)`, 用 long 防止中间相加溢出。
4. **放 A 的约束**: 只有 a==0 时才能放 A; 放 A 后 a 变 1, 同时连续 L 重置。

---

## 解法详解

### 解法一: DP 滚动数组 (推荐)

```cpp
class Solution {
public:
    int checkRecord(int n) {
        const int MOD = 1e9 + 7;
        // dp[a][l]: 当前已用 a 个 A, 末尾连续 l 个 L 的方案数
        long dp[2][3] = {};
        dp[0][0] = 1;  // 空串初始状态

        for (int i = 0; i < n; i++) {
            long ndp[2][3] = {};
            for (int a = 0; a < 2; a++) {
                for (int l = 0; l < 3; l++) {
                    if (dp[a][l] == 0) continue;
                    // 放 P: 连续 L 重置为 0
                    ndp[a][0] = (ndp[a][0] + dp[a][l]) % MOD;
                    // 放 L: 连续 L +1 (不超过 2)
                    if (l + 1 < 3) {
                        ndp[a][l + 1] = (ndp[a][l + 1] + dp[a][l]) % MOD;
                    }
                    // 放 A: A 次数 +1 (必须 a==0), 连续 L 重置
                    if (a + 1 < 2) {
                        ndp[a + 1][0] = (ndp[a + 1][0] + dp[a][l]) % MOD;
                    }
                }
            }
            memcpy(dp, ndp, sizeof(dp));
        }

        long ans = 0;
        for (int a = 0; a < 2; a++)
            for (int l = 0; l < 3; l++)
                ans = (ans + dp[a][l]) % MOD;
        return (int)ans;
    }
};
```

### 解法二: 6 变量展开 (更高效)

```cpp
class Solution {
public:
    int checkRecord(int n) {
        const int MOD = 1e9 + 7;
        // 6 个状态: (a次A, l个连续L)
        long a0l0 = 1, a0l1 = 0, a0l2 = 0;
        long a1l0 = 0, a1l1 = 0, a1l2 = 0;

        for (int day = 0; day < n; day++) {
            long n_a0l0 = (a0l0 + a0l1 + a0l2) % MOD;  // P after any (0,*)
            long n_a0l1 = a0l0;                           // L after (0,0)
            long n_a0l2 = a0l1;                           // L after (0,1)
            long n_a1l0 = (a0l0 + a0l1 + a0l2            // A after any (0,*)
                         + a1l0 + a1l1 + a1l2) % MOD;     // + P after any (1,*)
            long n_a1l1 = a1l0;                           // L after (1,0)
            long n_a1l2 = a1l1;                           // L after (1,1)

            a0l0 = n_a0l0; a0l1 = n_a0l1; a0l2 = n_a0l2;
            a1l0 = n_a1l0; a1l1 = n_a1l1; a1l2 = n_a1l2;
        }

        return (int)((a0l0 + a0l1 + a0l2 + a1l0 + a1l1 + a1l2) % MOD);
    }
};
```

### 解法三: 矩阵快速幂 (进阶)

6 个状态之间的转移是线性的, 可表示为 6x6 转移矩阵 M。`dp_n = M^n * dp_0`, 用矩阵快速幂在 O(6^3 * log n) = O(log n) 内求解。当 n 极大 (如 10^18) 时是唯一可行方案。

```cpp
class Solution {
public:
    // 矩阵乘法 (6x6)
    typedef vector<vector<long>> Mat;
    int MOD = 1e9 + 7;

    Mat multiply(const Mat& A, const Mat& B) {
        int sz = A.size();
        Mat C(sz, vector<long>(sz, 0));
        for (int i = 0; i < sz; i++)
            for (int k = 0; k < sz; k++)
                if (A[i][k])
                    for (int j = 0; j < sz; j++)
                        C[i][j] = (C[i][j] + A[i][k] * B[k][j]) % MOD;
        return C;
    }

    Mat matpow(Mat M, int p) {
        int sz = M.size();
        Mat result(sz, vector<long>(sz, 0));
        for (int i = 0; i < sz; i++) result[i][i] = 1; // 单位矩阵
        while (p > 0) {
            if (p & 1) result = multiply(result, M);
            M = multiply(M, M);
            p >>= 1;
        }
        return result;
    }

    int checkRecord(int n) {
        // 状态顺序: (0,0),(0,1),(0,2),(1,0),(1,1),(1,2)
        // 转移矩阵 M: ndp = M * dp
        Mat M = {
            {1, 1, 1, 0, 0, 0},  // (0,0) ← P from (0,0),(0,1),(0,2)
            {1, 0, 0, 0, 0, 0},  // (0,1) ← L from (0,0)
            {0, 1, 0, 0, 0, 0},  // (0,2) ← L from (0,1)
            {1, 1, 1, 1, 1, 1},  // (1,0) ← A from (0,*) + P from (1,*)
            {0, 0, 0, 1, 0, 0},  // (1,1) ← L from (1,0)
            {0, 0, 0, 0, 1, 0},  // (1,2) ← L from (1,1)
        };
        Mat result = matpow(M, n);
        long ans = 0;
        for (int i = 0; i < 6; i++)
            ans = (ans + result[i][0]) % MOD;
        return (int)ans;
    }
};
```

---

## 易错点

| 错误写法 (✗) | 正确写法 (✓) | 说明 |
|-------------|-------------|------|
| ✗ 忘记每步取模 | ✓ 每步 `(x + y) % MOD` | 不取模会溢出, 即使 long 也不够 |
| ✗ `ndp[a][l+1]` 不检查 `l+1 < 3` | ✓ `if (l + 1 < 3)` | 连续 L 最多 2, 达到 3 即非法 |
| ✗ 放 A 时不重置连续 L | ✓ 放 A → `ndp[a+1][0]`, L 重置为 0 | A 不是 L, 打断连续 L 计数 |
| ✗ 用 int 做中间计算 | ✓ 用 long | 两个接近 10^9 的数相加会溢出 int |
| ✗ 初始状态 dp 全 0 | ✓ `dp[0][0] = 1` (空串) | 初始有一种方案 (空记录) |

---

## 面试追问

**Q1: 为什么空间可以优化到 O(1)?**

第 i 天的状态只依赖第 i-1 天的 6 个值, 不需要保留更早的历史。用两组 `dp[2][3]` 交替 (或直接 6 个变量) 即可。这和斐波那契数列只保留前两项是相同的思路。

**Q2: 矩阵快速幂的思路和复杂度?**

6 个状态之间的转移是线性变换, 可表示为 6x6 转移矩阵 M。`dp_n = M^n * dp_0`, 用快速幂在 O(6^3 * log n) = O(216 * log n) 时间内求解。当 n 极大 (如 10^18, 超出 O(n) 承受范围) 时是唯一可行方案。面试中 DP 即可, 矩阵快速幂是加分项。

**Q3: 如果限制改为"A 最多 k 次, 连续 L 最多 m 个", 怎么泛化?**

状态变为 `dp[k+1][m+1]`, 共 `(k+1) * (m+1)` 个状态, 转移逻辑不变。时间 O(n * k * m), 空间 O(k * m)。矩阵快速幂变为 `(k+1)(m+1)` 阶矩阵, 时间 O(((k+1)(m+1))^3 * log n)。

---

## 相关题型

| 题号 | 题目 | 关联 |
|------|------|------|
| 551 | Student Attendance Record I | 本题的简化版 (判断单个记录是否合法) |
| 935 | Knight Dialer | 类似的状态机 DP, 固定状态转移 |
| 790 | Domino and Tromino Tiling | 状态机 DP, 滚动数组 |
| 70 | Climbing Stairs | 基础线性 DP (状态机的最简形式) |
| 1220 | Count Vowels Permutation | 类似的状态转移计数 |
