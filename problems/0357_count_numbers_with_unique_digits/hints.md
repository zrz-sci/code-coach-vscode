# 357. Count Numbers with Unique Digits - 统计各位数字都不同的数字个数

## 核心思路
**排列组合 / 数学计数**：统计 0 到 10^n 范围内各位数字互不相同的数的个数。对于 k 位数（k>=1），首位有 9 种选择（1-9），第二位有 9 种（0-9除首位），第三位 8 种...以此类推。将各位数的唯一数字个数累加即可。

## 思维链
1. n=0 时只有数字 0 → 答案为 1
2. 1 位数：0-9 共 10 个，全部满足
3. 2 位数（10-99）：首位 9 选择 × 第二位 9 选择 = 81 个
4. k 位数：9 × 9 × 8 × 7 × ... × (11-k) = 9 × P(9, k-1)
5. 总计 = 1 + sum(k位数的唯一数字数) for k=1..n
6. n > 10 时没有更多唯一数字的数（只有 0-9 共 10 个数字可用）

## 解法概览表⭐

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 数学排列 | O(N) | O(1) | 最优，直接计算 |
| DP | O(N) | O(N) | 思路直观，dp[i]表示i位的唯一数个数 |
| 回溯 | O(10!) | O(10) | 穷举验证，效率低 |

## 关键提示
1. **首位不能为 0**：k 位数的首位只有 1-9 共 9 种选择
2. **n 上限为 10**：超过 10 位时不可能所有位都不同（只有 0-9 十个数字）
3. **累加而非替换**：f(n) = f(n-1) + n位数中唯一数字的个数
4. **公式**：k位数的唯一数字数 = 9 * 9 * 8 * 7 * ... * (11-k)

## 解法详解

### 解法一：数学排列（推荐）

**思路**：直接用排列公式计算每种位数下的唯一数字数，累加即可。

```cpp
class Solution {
public:
    int countNumbersWithUniqueDigits(int n) {
        if (n == 0) return 1;
        
        int total = 10;   // 1位数：0-9 共10个
        int unique = 9;   // 当前位数的唯一数字数初始为9（1位数除0外）
        
        for (int i = 2; i <= min(n, 10); i++) {
            unique *= (11 - i);  // 第i位有(11-i)种选择
            total += unique;
        }
        return total;
    }
};
```

**复杂度**：时间 O(min(N,10))，空间 O(1)

### 解法二：DP

**思路**：dp[i] 表示 i 位数中各位不重复的数的个数。dp[i] = dp[i-1] * (11-i)。

```cpp
class Solution {
public:
    int countNumbersWithUniqueDigits(int n) {
        if (n == 0) return 1;
        
        vector<int> dp(min(n, 10) + 1);
        dp[1] = 9;  // 1位数（不含0）有9个唯一数字数
        
        for (int i = 2; i <= min(n, 10); i++) {
            dp[i] = dp[i - 1] * (11 - i);
        }
        
        int total = 1; // 包含数字 0
        for (int i = 1; i <= min(n, 10); i++) {
            total += dp[i];
        }
        return total;
    }
};
```

**复杂度**：时间 O(N)，空间 O(N)

### 解法三：回溯

**思路**：DFS 枚举每一位的数字选择，用 visited 数组标记已使用的数字。

```cpp
class Solution {
    int count = 0;
    
    void dfs(int n, int curLen, vector<bool>& used) {
        if (curLen == n) return;
        for (int d = 0; d <= 9; d++) {
            if (used[d]) continue;
            if (curLen == 0 && d == 0) continue; // 首位不能为0（排除前导零）
            used[d] = true;
            count++;
            dfs(n, curLen + 1, used);
            used[d] = false;
        }
    }
    
public:
    int countNumbersWithUniqueDigits(int n) {
        if (n == 0) return 1;
        count = 1; // 包含数字0
        vector<bool> used(10, false);
        dfs(n, 0, used);
        return count;
    }
};
```

**复杂度**：时间 O(10!)，空间 O(10)

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| 首位有 10 种选择 | 首位有 9 种选择（1-9） | 首位不能为 0（否则不是 k 位数） |
| n=0 返回 0 | n=0 返回 1 | 0 本身是一个唯一数字的数 |
| 忘记 n>10 的情况 | `min(n, 10)` | 只有 10 个数字，超过 10 位不可能不重复 |
| 只算 n 位数 | 累加 1 到 n 位数的结果 | 题目要求 0 <= x < 10^n 范围内所有满足条件的 |

## 面试追问

**Q1: 如何推导 k 位唯一数字数的公式？**

A: k 位数中各位不同：
- 第 1 位：9 种选择（1-9，不能是 0）
- 第 2 位：9 种选择（0-9 中除去第1位已选的）
- 第 3 位：8 种选择
- ...
- 第 k 位：(11-k) 种选择
- 总计：9 * 9 * 8 * 7 * ... * (11-k) = 9 * 9!/(10-k)!

**Q2: 如果范围不是 [0, 10^n) 而是 [L, R]，如何计算？**

A: 这是"数位 DP"经典问题。定义 f(x) = [0, x] 内唯一数字数的个数，答案 = f(R) - f(L-1)。数位 DP 按位枚举，维护当前是否紧贴上界、已选数字的 mask。

**Q3: 如果允许某些特定数字重复（比如 0 可以重复），如何修改？**

A: 修改排列计算的可选数字集。对于允许重复的数字 d，不将其标记为"已使用"。回溯法中将 `used[d]` 的设置条件修改为"仅对不可重复的数字设置"。数学方法需要容斥原理处理。

## 相关题型
- [233. Number of Digit One](../0233_number_of_digit_one/) - 数位计数
- [386. Lexicographical Numbers](../0386_lexicographical_numbers/) - 数位枚举
- [902. Numbers At Most N Given Digit Set](../0902_numbers_at_most_n_given_digit_set/) - 数位 DP
