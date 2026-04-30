# 483. 最小好进制 (Smallest Good Base)

## 核心思路

找到最小的 k >= 2，使得 n 在 k 进制下的表示全为 1。即 n = 1 + k + k^2 + ... + k^(m-1)。
关键观察：m 越大则 k 越小。因此从最大可能的 m 开始枚举，对每个 m 二分搜索 k。

## 思维链

1. **数学建模**：n = 1 + k + k^2 + ... + k^(m-1) = (k^m - 1) / (k - 1)，要求最小的 k
2. **确定 m 的范围**：k >= 2 时 m <= log2(n+1)，n <= 10^18 所以 m <= 60；m >= 2（否则任何 n-1 都是答案）
3. **贪心策略**：m 越大 k 越小，所以从大到小枚举 m
4. **对每个 m 二分搜索 k**：k 的范围是 [2, n^(1/(m-1))]，等比数列和是关于 k 的单调递增函数
5. **计算等比求和**：对候选 k 计算 sum = 1 + k + k^2 + ... + k^(m-1)，比较是否等于 n
6. **兜底结果**：如果所有 m 都找不到，返回 n-1（对应 m=2，即 n = 1 + (n-1)）

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 枚举m+二分k | O(log^2 n) | O(1) | ⭐ 数学+二分，最优解 |
| 暴力枚举 | O(n) | O(1) | 不可行，n 太大 |

## 关键提示

1. n 最大 10^18，必须用 `long long` 或 `unsigned long long`
2. 计算等比求和时要防止溢出，可用 `__int128` 或在乘法时提前判断
3. m 从大到小枚举，找到第一个合法 k 就返回（保证 k 最小）
4. 等比求和公式：sum = (k^m - 1) / (k - 1)，但直接累加更安全

## 解法详解

### 解法1: 枚举位数 + 二分搜索基数

```cpp
class Solution1 {
public:
    string smallestGoodBase(string n) {
        long long num = stoll(n);
        // m 从大到小枚举：m 越大 k 越小
        // k >= 2 => num >= 1+2+4+...+2^(m-1) = 2^m - 1
        // 所以 m <= log2(num+1) <= 60
        for (int m = 60; m >= 2; m--) {
            // 二分搜索 k: k^(m-1) <= num，所以 k <= num^(1/(m-1))
            long long lo = 2, hi = (long long)pow(num, 1.0 / (m - 1)) + 1;
            while (lo <= hi) {
                long long mid = lo + (hi - lo) / 2;
                // 计算 1 + mid + mid^2 + ... + mid^(m-1)
                long long sum = 0;
                long long cur = 1;
                bool overflow = false;
                for (int i = 0; i < m; i++) {
                    sum += cur;
                    if (sum > num) { overflow = true; break; }
                    if (i < m - 1) {
                        if (cur > num / mid) { overflow = true; break; }
                        cur *= mid;
                    }
                }
                if (overflow || sum > num) {
                    hi = mid - 1;
                } else if (sum < num) {
                    lo = mid + 1;
                } else {
                    return to_string(mid);
                }
            }
        }
        // 兜底：m = 2 时 k = n - 1
        return to_string(num - 1);
    }
};
```

### 解法2: 数学优化（直接计算 k 候选值）

```cpp
class Solution2 {
public:
    string smallestGoodBase(string n) {
        long long num = stoll(n);
        for (int m = 60; m >= 2; m--) {
            // k = floor(num^(1/(m-1)))
            long long k = (long long)pow(num, 1.0 / (m - 1));
            if (k < 2) continue;
            // 验证 1 + k + k^2 + ... + k^(m-1) == num
            long long sum = 0, cur = 1;
            bool valid = true;
            for (int i = 0; i < m; i++) {
                sum += cur;
                if (sum > num) { valid = false; break; }
                if (i < m - 1) cur *= k;
            }
            if (valid && sum == num) return to_string(k);
            // 浮点精度: 也检查 k+1
            k++;
            sum = 0; cur = 1; valid = true;
            for (int i = 0; i < m; i++) {
                sum += cur;
                if (sum > num) { valid = false; break; }
                if (i < m - 1) cur *= k;
            }
            if (valid && sum == num) return to_string(k);
        }
        return to_string(num - 1);
    }
};
```

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 原因 |
|-----------|-----------|------|
| `int num = stoi(n)` | `long long num = stoll(n)` | n 最大 10^18，int 放不下 |
| `cur *= mid` 不检查溢出 | 先判断 `cur > num / mid` | 防止 long long 溢出 |
| `pow(num, 1.0/(m-1))` 直接用 | 对结果 +1 后二分/验证邻近值 | 浮点精度问题 |
| 从小到大枚举 m | 从大到小枚举 m | 题目要求最小的 k，对应最大的 m |

## 面试追问

**Q1: 为什么 m 从大到小枚举就能保证找到最小的 k？**
A: n = 1 + k + ... + k^(m-1) 中，m 固定时 k 唯一确定（等比求和单调）。m 越大则 k 越小（数位越多，基数越小）。所以从最大 m 开始找到的第一个合法 k 就是最小的。

**Q2: 如何处理 10^18 级别计算中的溢出？**
A: 两种策略：(1) 累加时判断 sum > n 就提前退出；(2) 乘法前判断 cur > n/mid 来防止乘法溢出；(3) 也可用 `__int128` 或 `unsigned long long`。

**Q3: 这道题的时间复杂度怎么分析？**
A: 外层枚举 m 最多 60 次，内层二分 k 最多 O(log n) 次，每次验证 O(m)。总复杂度 O(m * log(n) * m) = O(log^2(n) * log(n))，实际由于 m 大时 k 范围极小，远快于此。

## 相关题型

- [372. 超级次方](../0372_super_pow/) - 数学+快速幂
- [326. 3的幂](../0326_power_of_three/) - 数学判断
- [69. x 的平方根](../0069_sqrtx/) - 二分搜索
- [29. 两数相除](../0029_divide_two_integers/) - 位运算+数学
